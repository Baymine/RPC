#include <google/protobuf/service.h>
#include <atomic>
#include <future>
#include "tinyrpc/comm/start.h"
#include "tinyrpc/net/http/http_request.h"
#include "tinyrpc/net/http/http_response.h"
#include "tinyrpc/net/http/http_servlet.h"
#include "tinyrpc/net/http/http_define.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_async_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_controller.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_closure.h"
#include "tinyrpc/net/net_address.h"
#include "test_tinypb_server.pb.h"

const char* html = "<html><body><h1>Welcome to TinyRPC, just enjoy it!</h1><p>%s</p></body></html>";

tinyrpc::IPAddress::ptr addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 20000);

class BlockCallHttpServlet : public tinyrpc::HttpServlet{
public:
    BlockCallHttpServlet() = default;
    ~BlockCallHttpServlet() = default;

    void handle(tinyrpc::HttpRequest* req, tinyrpc::HttpResponse* res){
        setHttpCode(res, tinyrpc::HTTP_OK);
        setHttpContentType(res, "text/html;charset=utf-8");

        queryAgeReq rpc_req;
        queryAgeRes rpc_res;

        rpc_req.set_id(std::atoi(req->m_query_maps["id"].c_str()));

        tinyrpc::TinyPbRpcChannel channel(addr);
        QueryService_Stub stub(&channel);

        tinyrpc::TinyPbRpcController rpc_controller;
        rpc_controller.SetTimeout(5000);
        stub.query_age(&rpc_controller, &rpc_req, &rpc_res, nullptr);

        if (rpc_controller.ErrorCode() != 0){
            char buf[512];
            sprintf(buf, html, "failed to call QueryServer rpc server");
            setHttpBody(res, std::string(buf));
            return;
        }

        if(rpc_res.ret_code() != 0){
            std::stringstream ss;
            ss << "QueryServer rpc server return bad result, ret = " << rpc_res.ret_code() << ", and res_info = " << rpc_res.res_info();
            AppDebugLog << ss.str();
            char buf[512];
            sprintf(buf, html, ss.str().c_str());
            setHttpBody(res, std::string(buf));
            return;
        }

        std::stringstream ss;
        ss << "Success!! Your age is," << rpc_res.age() << " and Your id is " << rpc_res.id();

        char buf[512];
        sprintf(buf, html, ss.str().c_str());
        setHttpBody(res, std::string(buf));
    }

    std::string getServletName() {
        return "BlockCallHttpServlet";
    }
};