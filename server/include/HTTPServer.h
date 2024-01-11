#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <boost/asio.hpp>
#include <string>
#include <json.h>

class HTTPServer {
public:
    HTTPServer(int port); // 构造函数声明
    void start();         // 启动服务器的方法

private:
    void handle_request(boost::asio::ip::tcp::socket& socket); // 处理请求的方法

    boost::asio::io_context io_context;  // Boost.Asio I/O 上下文
    boost::asio::ip::tcp::acceptor acceptor;  // 用于接受连接的 acceptor
};

#endif 
