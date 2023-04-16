#include <google/protobuf/service.h>
#include <sstream>
#include <atomic>

#include "tinyrpc/net/tcp/tcp_server.h"
#include "tinyrpc/net/net_address.h"
#include "tinyrpc/net/mutex.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_dispatcher.h"
#include "tinyrpc/comm/log.h"
#include "tinyrpc/comm/start.h"
// #include "test_tinypb_server.pb.h"
#include "test_chatGPT.pb.h"

#include "include/openai/openai.hpp"

tinyrpc::CoroutineMutex g_cor_mutex;

class callChatGPT{
public:
    static std::string call(std::string& promptText){
      
      nlohmann::json prompt = {
          {"model", "text-davinci-003"}, 
          {"prompt", promptText}, 
          {"max_tokens", 1000},
          {"temperature", 0}
      };
      auto completion = openai::completion().create(prompt);

      return completion["choices"][0]["text"];
    }

};

class ChatGPTServiceImpl : public ChatGPTService{
public:
    ChatGPTServiceImpl() {}
    // ~ChatGPTServiceImpl() {}

    void chatGPT(
        google::protobuf::RpcController* controller,
        const ::queryChatReq* request,
        ::queryChatRes* respose,
        ::google::protobuf::Closure* done){
            std::string s = request->prompt();

            respose->set_res_info(callChatGPT::call(s));
            respose->set_ret_code(0);

            if(done) done->Run();
    }

};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Start TinyRPC server error, input argc is not 2!");
    printf("Start TinyRPC server like this: \n");
    printf("./server a.xml\n");
    return 0;
  }
  openai::start();

  // 1. 读取配置文件
  tinyrpc::InitConfig(argv[1]);
  // 2. 注册 service
  REGISTER_SERVICE(ChatGPTServiceImpl);
  // 3. 启动 RPC 服务
  tinyrpc::StartRpcServer();
  
  return 0;
}