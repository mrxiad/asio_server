#include "HTTPServer.h"
#include<iostream>
// 构造函数实现
HTTPServer::HTTPServer(int port) 
    : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

// 启动服务器的实现
void HTTPServer::start() {
    for (;;) {
        boost::asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        handle_request(socket);
    }
}

// 处理请求的实现
void HTTPServer::handle_request(boost::asio::ip::tcp::socket& socket) {
    try {
        boost::asio::streambuf request_buffer;
        boost::asio::read_until(socket, request_buffer, "\r\n\r\n");

        // 从请求中提取头部和主体
        std::istream request_stream(&request_buffer);
        std::string header_line, body;
        while (std::getline(request_stream, header_line) && header_line != "\r") {
            // 可以处理头部信息，如果需要
        }
        body.assign((std::istreambuf_iterator<char>(request_stream)), std::istreambuf_iterator<char>());

        // 解析 JSON
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(body, root)) {
            std::cerr << "Error in parsing JSON\n";
            return;
        }

        // 添加 "data": "success"
        root["data"] = "success";

        // 将 JSON 转换为字符串
        Json::StreamWriterBuilder builder;
        const std::string response_json = Json::writeString(builder, root);

        // 构建并发送 HTTP 响应
        std::string response = 
            "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(response_json.length()) + 
            "\r\nContent-Type: application/json\r\n\r\n" + response_json;
        boost::asio::write(socket, boost::asio::buffer(response));
    } catch (const std::exception& e) {
        std::cerr << "Exception in handle_request: " << e.what() << "\n";
    }
}