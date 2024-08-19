#include <iostream>
#include "mprpcapplicate.h"
#include "user.pb.h"

int main(int argc, char** argv)
{
    // 程序启动后，想要使用，mprpc框架调用mprpc的服务调用
    mpRPCApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法
    fixbug::UserServiceRPC_Stub stub(new mpRPCChannel());
    fixbug::LoginRequest request;
    request.set_name("Zs");
    request.set_pwd("1234");

    fixbug::LoginResponse response;

    // RPCChannel->RPCChannel::callmethod 集中进行所有rpc方法调用的参数序列化和网络发送
    mpRPCController controller;
    stub.Login(&controller, &request, &response, nullptr);

    // rpc调用完成，读取调用结果response
    if(controller.Failed()){
        std::cout<<controller.ErrorText()<<std::endl;
    }else
    {
        if(!response.result().errcode()){

            std::cout<<"rpc login response:"<<response.success()<<std::endl; 
        }else{
            std::cout<<"rpc login response error"<<response.result().errmsg()<<std::endl;
        }
    }
    return 0;
}