#ifndef CODE_H
#define CODE_H

enum{
    CODE_REGISTER_SUCCESS=1001,     //注册成功
    CODE_REGISTER_FAILED,           //注册失败
    CODE_LOGIN_SUCCESS,             //登录成功
    CODE_LOGIN_FAILED,              //登录失败
    CODE_LOGOUT_SUCCESS,            //登出成功
    CODE_LOGOUT_FAILED,             //登出失败
    CODE_ADD_FRIEND_SUCCESS,        //添加好友成功
    CODE_ADD_FRIEND_FAILED,         //添加好友失败
    CODE_CREATE_GROUP_SUCCESS,      //创建群组成功
    CODE_CREATE_GROUP_FAILED,       //创建群组失败
    CODE_JOIN_GROUP_SUCCESS,        //加入群组成功
    CODE_JOIN_GROUP_FAILED,         //加入群组失败
    CODE_ONE_CHAT_SUCCESS,          //一对一聊天成功
    CODE_ONE_CHAT_FAILED,           //一对一聊天失败
};


#endif // CODE_H