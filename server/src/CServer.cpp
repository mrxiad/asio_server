#include "CServer.h"
#include "CSession.h"
#include <iostream>

CServer::CServer(boost::asio::io_service& io_service, short port):_io_service(io_service), _port(port),
_acceptor(io_service, tcp::endpoint(tcp::v4(),port))
{
	cout << "Server start success, listen on port : " << _port << endl;
	StartAccept();
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error){
	if (!error) {
		new_session->Start();
		_sessions.insert(make_pair(new_session->GetUuid(), new_session));
	}
	else {
		cout << "session accept failed, error is " << error.message() << endl;
	}

	StartAccept();
}

void CServer::StartAccept() {
	shared_ptr<CSession> new_session = std::make_shared<CSession>(_io_service, this);
	_acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
}

void CServer::ClearSession(std::string uuid) {
	_sessions.erase(uuid);
}
