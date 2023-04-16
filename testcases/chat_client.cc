// #include "test_tinypb_server.pb.h"
#include "test_chatGPT.pb.h"
#include "tinyrpc/net/net_address.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_async_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_channel.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_closure.h"
#include "tinyrpc/net/tinypb/tinypb_rpc_controller.h"
#include <google/protobuf/service.h>
#include <iostream>

void chat(std::string text){
    tinyrpc::IPAddress::ptr addr = std::make_shared<tinyrpc::IPAddress>("127.0.0.1", 19999);

    tinyrpc::TinyPbRpcChannel channel(addr);
    ChatGPTService_Stub stub(&channel);

    tinyrpc::TinyPbRpcController rpc_controller;
    rpc_controller.SetTimeout(5000);

    queryChatReq chat_req;
    queryChatRes chat_res;

    chat_req.set_prompt(text);

    stub.chatGPT(&rpc_controller, &chat_req, &chat_res, nullptr);

   if (rpc_controller.ErrorCode() != 0) {
        std::cout << "Failed to call tinyrpc server, error code: " << rpc_controller.ErrorCode() << ", error info: " << rpc_controller.ErrorText() << std::endl;
        return;
    }

    std::cout << "Success get response frrom tinyrpc server " << addr->toString() << ", response body: " << chat_res.ShortDebugString() << std::endl;
    std::cout << "The responses from chatGPT are: " << "\n---------------------------------\n" << 
    chat_res.res_info() << std::endl;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Error! You need add your prompt like this:\n");
        printf("./chat_client \"Your prompt information\"\n");
    }

    chat(argv[1]);
    return 0;
}