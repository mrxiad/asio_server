#ifndef CSESSION_H
#define CSESSION_H

#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <queue>
#include <mutex>
#include <memory>
#include "const.h"
#include "MsgNode.h"
using namespace std;

using boost::asio::ip::tcp;
class CServer;

class CSession: public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_service& io_service, CServer* server);
	~CSession();
	tcp::socket& GetSocket();
	std::string& GetUuid();
	void Start();//这个函数很重要，是解耦合的关键
	void Send(char* msg,  short max_length, short msgid);
	void Send(std::string msg, short msgid);
	void Close();
	std::shared_ptr<CSession> SharedSelf();
private:
	void HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<CSession> shared_self);
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);
	tcp::socket _socket;
	std::string _uuid;
	char _data[MAX_LENGTH];
	CServer* _server;//方便server删除自己
	bool _b_close;
	std::queue<shared_ptr<SendNode> > _send_que;//这个是用来存储要发送的消息的
	std::mutex _send_lock;
	//收到的消息结构
	std::shared_ptr<RecvNode> _recv_msg_node;//这个是用来存储收到的消息的
	bool _b_head_parse;
	//收到的头部结构
	std::shared_ptr<MsgNode> _recv_head_node;//这个是用来存储收到的头部的
};


class LogicNode {
    friend class LogicSystem;
public:
    LogicNode(shared_ptr<CSession>session, 
    shared_ptr<RecvNode> recvnode):
	_session(session),_recvnode(recvnode) {
	}
private:
    shared_ptr<CSession> _session;
    shared_ptr<RecvNode> _recvnode;
};
#endif