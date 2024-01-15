#ifndef _SERVER_INCLUDE_CSERVER_H_
#define _SERVER_INCLUDE_CSERVER_H_

#include <boost/asio.hpp>
#include "CSession.h"
#include <memory>
#include <map>
using namespace std;
using boost::asio::ip::tcp;

class CServer
{
public:
    //初始化接收器和端口
	CServer(boost::asio::io_context& io_context, short port);
	void ClearSession(std::string);
private:
	void HandleAccept(shared_ptr<CSession>, const boost::system::error_code & error);
	void StartAccept();
	boost::asio::io_context &_io_context;//io上下文
	short _port;//端口
	tcp::acceptor _acceptor;//接收器
	std::map<std::string, shared_ptr<CSession>> _sessions;//会话列表
};

#endif // !_SERVER_INCLUDE_CSERVER_H_