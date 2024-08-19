#pragma once
#include "google/protobuf/service.h"
#include "logger.h"
#include <string>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <unordered_map>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

//
class RPCProvider
{
public:
    //
    void NotifyService(google::protobuf::Service* service);

    //
    void Run();
private:
    
    //
    muduo::net::EventLoop m_eventLoop;
    //
    struct ServiceInfo{
        google::protobuf::Service *m_service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;
    };
    //  
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;

    void OnConnection(const muduo::net::TcpConnectionPtr&);
    void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*, muduo::Timestamp);
    void SendRPCResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
};