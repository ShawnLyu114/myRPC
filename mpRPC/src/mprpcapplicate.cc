#include "mprpcapplicate.h"
#include <iostream>
#include <unistd.h>
#include <string>

mpRPCConfig mpRPCApplication::m_config;

void ShowArgHelp(){
    std::cout<<"command -i <configfile>" << std::endl;
}

void mpRPCApplication::Init(int argc, char **argv)
{
    if(argc<2){
        ShowArgHelp();
        exit(EXIT_FAILURE);
    }

    int c=0;
    std::string config_file;
    while ((c=getopt(argc, argv, "i:"))!=-1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    
    //loading configfile
    m_config.LoadConfigFile(config_file.c_str());
    std::cout<<"rpcserverip:"<<m_config.Load("rpcserverip")<<std::endl;
    std::cout<<"rrpcserverport:"<<m_config.Load("rpcserverport")<<std::endl;
    std::cout<<"zookeeperip:"<<m_config.Load("zookeeperip")<<std::endl;
    std::cout<<"zookeeperport:"<<m_config.Load("zookeeperport")<<std::endl;

}

mpRPCApplication& mpRPCApplication::GetInstance()
{
    static mpRPCApplication rpc;
    return rpc;
}

mpRPCConfig& mpRPCApplication::GetConfig(){
    return m_config;
}