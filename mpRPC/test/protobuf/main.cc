#include "test.pb.h"
#include <string>
#include <iostream>
using namespace fixbug;

int main(){
    GetFriendListsResponse rsp;
    ResultCode *rc=rsp.mutable_result();
    rc->set_errcode(1);

    User*user1=rsp.add_friend_list();
    user1->set_name("Zs");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    std::cout<<rsp.friend_list_size()<<std::endl;

    return 0;
}

int main1()
{
    //封装了login请求对象的数据
    LoginRequest req;
    req.set_name("shawn");
    req.set_pwd("123456");

    //对象数据序列化
    std::string sen_str;
    if(req.SerializeToString(&sen_str)){
        std::cout<<sen_str<<std::endl;
    }
    
    //从sen_str反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(sen_str)){
        std::cout<<reqB.name()<<std::endl;
        std::cout<<reqB.pwd()<<std::endl;
    }
    return 0;
}