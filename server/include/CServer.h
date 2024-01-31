#ifndef _SERVER_INCLUDE_CSERVER_H_
#define _SERVER_INCLUDE_CSERVER_H_

#include <boost/asio.hpp>
#include <memory>
#include <map>
#include <string> // Include the missing header file
#include "const.h"


using namespace std;
using boost::asio::ip::tcp;
class CSession;
class CServer
{
public:
	CServer(boost::asio::io_service& io_service, short port);
	void ClearSession(std::string);
	std::shared_ptr<CSession> GetSession(std::string uuid){
		if(_sessions.find(uuid) != _sessions.end()){
			return _sessions[uuid];
		}else{
			return nullptr;
		}
	}
private:
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code & error);
	void StartAccept();
	boost::asio::io_service& _io_service;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
};

#endif // !_SERVER_INCLUDE_CSERVER_H_