#include "HTTPServer.h"
#include<iostream>
int main() {
    try {
        // 创建服务器实例，监听8080端口
        std::cout << "Server started at port 8080\n";
        HTTPServer server(8080);
        // 启动服务器
        server.start();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
