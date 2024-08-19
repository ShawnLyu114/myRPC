#include "rpcprovider.h"
#include "mprpcapplicate.h"
#include "rpcheader.pb.h"
#include "zookeeperutil.h"


void RPCProvider::NotifyService(google::protobuf::Service* service){

    ServiceInfo service_info;

    //protobuf
    const google::protobuf::ServiceDescriptor* pserviceDesc=service->GetDescriptor();
    //get service name
    std::string service_name = pserviceDesc->name();

    int methodCnt=pserviceDesc->method_count();
    //

    for(int i=0; i<methodCnt; i++){
        const google::protobuf::MethodDescriptor* pmethodDesc=pserviceDesc->method(i);
        std::string method_name=pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        LOG_INFO("method_name:%s",method_name.c_str());
    }

    service_info.m_service=service;
    m_serviceMap.insert({service_name, service_info});

}

void RPCProvider::Run(){
    std::string ip = mpRPCApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(mpRPCApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    //TCP_server
    muduo::net::TcpServer server(&m_eventLoop, address, "RPCProvider");
    //bind
    server.setConnectionCallback(std::bind(&RPCProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RPCProvider::OnMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    //set
    server.setThreadNum(4);
    ZkClient zkCli;
    zkCli.Start();//链接zkserver
    for(auto &sp:m_serviceMap)
    {
        //service_name
        std::string service_path ="/"+sp.first;//拼接路径
        zkCli.Create(service_path.c_str(),nullptr,0);//创建临时性节点
        for(auto &mp:sp.second.m_methodMap)
        {
            //service_name/method_name
            std::string method_path=service_path+"/"+mp.first;//拼接服务器路径和方法路径
            char method_path_data[128]={0};
            sprintf(method_path_data,"%s:%d",ip.c_str(),port);//向data中写入路径

            //创建节点,ZOO_EPHEMERAL表示临时节点
            zkCli.Create(method_path.c_str(),method_path_data,strlen(method_path_data),ZOO_EPHEMERAL);
        }
    }
    //std::cout<<"PRCProvider start service at ip: "<<ip<<", port: "<<port<<std::endl;
    LOG_INFO("PRCProvider start service at ip:%s, port:%d", (char*)&ip, port);
    //run
    server.start();
    m_eventLoop.loop();
}

void RPCProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){
    if(!conn->connected()){
        //
        conn->shutdown();
    }
}

//
void RPCProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp){
    
    std::string recv_buff=buffer->retrieveAllAsString();

    //4 bytes
    uint32_t header_size=0;
    recv_buff.copy((char*)&header_size, 4, 0);

    //
    std::string rpc_header_str=recv_buff.substr(4, header_size);
    //
    mprpc::RPCHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if(rpcHeader.ParseFromString(rpc_header_str)){
        std::cout<<"rpc header parse success"<<std::endl;
        service_name=rpcHeader.service_name();
        method_name=rpcHeader.method_name();
        args_size=rpcHeader.args_size();
    }else{
        std::cout<<"rpc header parse error"<<std::endl;
        return;
    }
    std::string args_str=recv_buff.substr(4+header_size, args_size);

    // std::cout<<"================"<<std::endl;
    // std::cout<<"header size:"<<header_size<<std::endl;
    // std::cout<<"================"<<std::endl;

    auto it=m_serviceMap.find(service_name);
    if(it==m_serviceMap.end()){
        std::cout<<service_name<<"is not exit"<<std::endl;
        return;
    }
    
    auto m_it=it->second.m_methodMap.find(method_name);
    if(m_it==it->second.m_methodMap.end()){
        std::cout<<service_name<<":"<<method_name<<"is not exit"<<std::endl;
        return;
    } 

    google::protobuf::Service *service=it->second.m_service;
    const google::protobuf::MethodDescriptor* method=m_it->second;


    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)){
        std::cout<<"request parse error"<<std::endl;
        return;
    }
    google::protobuf::Message *response= service->GetResponsePrototype(method).New();

    //
    google::protobuf::Closure* done = 
            google::protobuf::NewCallback<RPCProvider, 
                                        const muduo::net::TcpConnectionPtr&, 
                                        google::protobuf::Message*>
                                            (this, &RPCProvider::SendRPCResponse, conn, response);
    //

    service->CallMethod(method, nullptr, request, response, done);
}

void RPCProvider::SendRPCResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response){
    std::string response_str;
    if(response->SerializeToString(&response_str)){
        conn->send(response_str);
    }else{
        std::cout<<"serialize response error"<<std::endl;
    }
    conn->shutdown(); //closed
}