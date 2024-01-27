#ifndef MSG_NODE_H
#define MSG_NODE_H

#include <string>
#include"const.h"
#include<iostream>
#include<boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

class MsgNode
{
    friend class CSession;
public:
    MsgNode(short max_len){
        _total_len=max_len;
        _cur_len=0;
        _data=new char[_total_len+1];
        _data[_total_len]='\0';
    }
    ~MsgNode(){
        delete[] _data;
    }
    void Clear(){
        ::memset(_data,0,_total_len);
        _cur_len=0;
    }
public:
    short _cur_len;
    short _total_len;
    char* _data;
};
/*
    _msg_id
    _data
    _total_len
    _cur_len
*/
class RecvNode:public MsgNode
{
    friend class LogicSystem;
public:

    //这个传递的参数是正常的，接受消息方不存转化为字节序，因为要处理这些消息
    RecvNode(short max_len,short id);
private:
    short _msg_id;
};

class SendNode:public MsgNode
{
    friend class LogicSystem;
public:

    //这个传递的参数是正常的，内部会自动转换成字节序
    SendNode(const char* msg,short max_len,short id);
private:
    short _msg_id;
};

#endif 