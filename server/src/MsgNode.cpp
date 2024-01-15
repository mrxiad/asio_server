#include "MsgNode.h"

RecvNode::RecvNode(short max_len,short id):MsgNode(max_len){
    _msg_id=id;
}


SendNode::SendNode(const char* msg,short max_len,short id):MsgNode(max_len+HEAD_TOTAL_LEN){
    _msg_id=id;
    //头部id,转化为网络字节序
    short msg_id_host=boost::asio::detail::socket_ops::host_to_network_short(_msg_id);
    ::memcpy(_data,&msg_id_host,HEAD_ID_LEN);
    //头部数据长度,转化为网络字节序
    short msg_len_host=boost::asio::detail::socket_ops::host_to_network_short(max_len);
    ::memcpy(_data+HEAD_ID_LEN,&msg_len_host,HEAD_DATA_LEN);
    //数据
    ::memcpy(_data+HEAD_TOTAL_LEN,msg,max_len);
}