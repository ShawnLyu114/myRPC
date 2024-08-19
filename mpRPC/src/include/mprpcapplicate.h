#pragma once
#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

//mpRPC
class mpRPCApplication
{
private:
    static mpRPCConfig m_config;

    mpRPCApplication(){}
    mpRPCApplication(const mpRPCApplication&)=delete;
    mpRPCApplication(mpRPCApplication&&)=delete;

public:
    static void Init(int argc, char **argv);
    static mpRPCApplication& GetInstance();
    static mpRPCConfig& GetConfig();
};

