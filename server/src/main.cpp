#include <iostream>
#include "CServer.h"
using namespace std;

int main()
{
    try {
        boost::asio::io_service  io_service;
        CServer s(io_service, 10086);
        cout<<"10086端口"<<endl;
        io_service.run();
    }
    catch(std::exception & e){
        std::cerr << "Exception: " << e.what() <<  endl;
    }
    boost::asio::io_service io_service;
}