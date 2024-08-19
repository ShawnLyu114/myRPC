#include"mprpccontroller.h"

//类的构造函数
mpRPCController::mpRPCController()
{
    m_failed=false;
    m_errtext="";
}

void mpRPCController::Reset()
{
    m_failed=false;
    m_errtext="";
}

bool mpRPCController::Failed()const
{
    return m_failed;
}

std::string mpRPCController::ErrorText()const
{
    return m_errtext;
}

void mpRPCController::SetFailed(const std::string& reason)
{
    m_failed=true;
    m_errtext=reason;
}

void mpRPCController::StartCancel(){}
bool mpRPCController::IsCanceled()const{return false;}
void mpRPCController::NotifyOnCancel(google::protobuf::Closure *callback){}