#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplicate.h"
#include "rpcprovider.h"
#include "logger.h"
//using namespace fixbug;

/* 本地服务，提供了两个进程内的本地方法 */
//rpc service provider
class UserService: public fixbug::UserServiceRPC{
public:
    bool Login(std::string name, std::string pwd){
        std::cout<<"doing local service:Login"<<std::endl;
        std::cout<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
        return true;
    }
    /* 
    重写基类UserserviceRpc的虚函数 下面这些方法都是框架直接调用的 
    1. caller ==> login(LoginRequest) ==> muduo ==> callee
    2. callee ==> login(LoginRequest) ==> 
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //
        std::string name = request->name();
        std::string pwd = request->pwd();
        //
        bool login_result=Login(name, pwd);
        //
        fixbug::ResultCode *code=response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);
        //
        done->Run();
    }
};

int main(int argc, char **argv){
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__,__FUNCTION__,__LINE__);
    
    //调用框架的初始化操作
    mpRPCApplication::Init(argc, argv);
    
    //provider是一个rpc网络服务对象，把userService对象发布到rpc节点上
    RPCProvider provider;
    provider.NotifyService(new UserService());
    
    //启动一个rpc服务发布节点，Run以后，进程进入阻塞状态，等待远端rpc调用
    provider.Run();
    return 0;
}