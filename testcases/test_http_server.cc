#include <iostream>
#include <string>
#include <memory>
#include <sstream>

#include <google/protobuf/service.h>

#include "tinyrpc/comm/start.h"
#include "tinyrpc/net/http/http_define.h"
#include "tinyrpc/net/http/http_request.h"
#include "tinyrpc/net/http/http_response.h"
#include "tinyrpc/net/http/http_servlet.h"
#include "tinyrpc/net/net_address.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_async_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_closure.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_controller.h"
#include "test_tinypb_server.pb.h"

#include <fstream>

using namespace tinyrpc;

const char* kHtml = "<html><body><h1>Welcome to TinyRPC, just enjoy it!</h1><p>%s</p></body></html>";

IPAddress::ptr addr = std::make_shared<IPAddress>("127.0.0.1", 20000);


class QueryServlet : public HttpServlet {
 public:
  QueryServlet() = default;
  ~QueryServlet() override = default;

  void handle(HttpRequest* req, HttpResponse* res) override {
    setHttpCode(res, HTTP_OK);    // HTTP status code
    setHttpContentType(res, "text/html;charset=utf-8");  // content type of the response

    std::stringstream ss;
    ss << "QueryServlet: received ID " << req->m_query_maps["id"];
    AppDebugLog << ss.str();

    queryAgeReq rpc_req;
    rpc_req.set_id(std::stoi(req->m_query_maps["id"]));

    std::shared_ptr<queryAgeRes> rpc_res = std::make_shared<queryAgeRes>();

    TinyPbRpcController rpc_controller;
    TinyPbRpcChannel channel(addr);

    QueryService_Stub stub(&channel);
    stub.query_age(&rpc_controller, &rpc_req, rpc_res.get(), nullptr);

    if (rpc_controller.ErrorCode() != 0 || rpc_res->ret_code() != 0) {
      ss << ": failed to query age: " << rpc_controller.ErrorText();
      AppDebugLog << ss.str();

      char buf[512];
      std::snprintf(buf, sizeof(buf), kHtml, "failed to query age");
      setHttpBody(res, std::string(buf));
    } else {
      ss << ": success, your age is " << rpc_res->age() << " and your id is " << rpc_res->id();
      AppDebugLog << ss.str();

      char buf[512];
      std::snprintf(buf, sizeof(buf), kHtml, ss.str().c_str());
      setHttpBody(res, std::string(buf));
    }
  }

  std::string getServletName() override {
    return "QueryServlet";
  }
};

class NonBlockQueryServlet : public HttpServlet {
 public:
    NonBlockQueryServlet() = default;
    ~NonBlockQueryServlet() override = default;

    void handle(HttpRequest* req, HttpResponse* res) override {
        setHttpCode(res, HTTP_OK);
        setHttpContentType(res, "text/html;charset=utf-8");

        std::stringstream ss;
        ss << "NonBlockQueryServlet: received ID " << req->m_query_maps["id"];
        AppDebugLog << ss.str();

        auto rpc_req = std::make_shared<queryAgeReq>();
        rpc_req->set_id(std::stoi(req->m_query_maps["id"]));

        auto rpc_res = std::make_shared<queryAgeRes>();

        std::shared_ptr<TinyPbRpcController> rpc_controller = 
                std::make_shared<TinyPbRpcController>();

        // TinyPbRpcController rpc_controller;
        rpc_controller->SetTimeout(50000);

        TinyPbRpcAsyncChannel::ptr async_channel = std::make_shared<TinyPbRpcAsyncChannel>(addr);

        auto callback = [rpc_res, res, this]() {
            std::stringstream ss;
            ss << "NonBlockQueryServlet: callback, rpc result: " << rpc_res->ShortDebugString();
            AppDebugLog << ss.str();

            if (rpc_res->ret_code() != 0) {
                ss << ": failed to query age: ret_code=" << rpc_res->ret_code() << ", res_info=" << rpc_res->res_info();
                AppDebugLog << ss.str();

                char buf[512];
                std::snprintf(buf, sizeof(buf), kHtml, ss.str().c_str());
                setHttpBody(res, std::string(buf));
            } else {
                ss << ": success, your age is " << rpc_res->age() << " and your id is " << rpc_res->id();
                AppDebugLog << ss.str();

                char buf[512];
                std::snprintf(buf, sizeof(buf), kHtml, ss.str().c_str());
                setHttpBody(res, std::string(buf));
            }
        };

        auto closure = std::make_shared<TinyPbRpcClosure>(callback);
        async_channel->saveCallee(rpc_controller, rpc_req, rpc_res, closure);

        QueryService_Stub stub(async_channel.get());
        stub.query_age(rpc_controller.get(), rpc_req.get(), rpc_res.get(), nullptr);

        async_channel->wait();

        ss << "NonBlockQueryServlet: RPC completed";
        AppDebugLog << ss.str();
    }

    std::string getServletName() override {
        return "NonBlockQueryServlet";
    }
};

class EchoServlet : public HttpServlet {
public:
    EchoServlet() = default;
    ~EchoServlet() override = default;

    void handle(HttpRequest* req, HttpResponse* res) override {
    setHttpCode(res, HTTP_OK);
    setHttpContentType(res, "text/html;charset=utf-8");

    std::stringstream ss;
    ss << "EchoServlet: received ID " << req->m_query_maps["id"];
    AppDebugLog << ss.str();

    char buf[512];
    std::snprintf(buf, sizeof(buf), kHtml, ss.str().c_str());
    setHttpBody(res, std::string(buf));

    }

    std::string getServletName() override {
    return "EchoServlet";
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
    std::cerr << "Start TinyRPC server error: input argc is not 2!" << std::endl;
    std::cerr << "Start TinyRPC server like this: " << std::endl;
    std::cerr << "./server a.xml" << std::endl;
    return 0;
    }

    // auto readHTMLFile = [](std::string&& fileName){
    //   std::ifstream file(fileName);
    //   if(!file.is_open()){
    //     std::cerr << "Fail to open file" << std::endl;
    //     return 1;
    //   }
    //   file.seekg(0, std::ios::end);
    //   std::streamsize length = file.tellg();
    //   file.seekg(0, std::ios::beg);

    //   kHtml = new char[length];
    //   file.read(kHtml, length);
    //   file.close();
    //   return 0;
    // };

    // readHTMLFile("./index.html");
    InitConfig(argv[1]);

    REGISTER_HTTP_SERVLET("/query", QueryServlet);
    REGISTER_HTTP_SERVLET("/nonblock_query", NonBlockQueryServlet);
    REGISTER_HTTP_SERVLET("/echo", EchoServlet);

    StartRpcServer();

    delete[] kHtml;
    return 0;
}


