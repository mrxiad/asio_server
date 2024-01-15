#include "CServer.h"
#include <iostream>

//初始化并且开启监听
CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(),port))
{
	StartAccept();
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Start();
		_sessions.insert(make_pair(new_session->GetUuid(), new_session));
	}
	else {
		cout << "session accept failed, error is " << error.value() << endl;
	}

	StartAccept();
}

//开始接收，接收成功后调用HandleAccept
void CServer::StartAccept() {
	shared_ptr<CSession> new_session = make_shared<CSession>(_io_context, this);
    //这一步是异步的，所以不会阻塞？
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}

void CServer::ClearSession(std::string uuid) {
	_sessions.erase(uuid);
}