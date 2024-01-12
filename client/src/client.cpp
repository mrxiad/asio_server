#include <iostream>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio::ip;
const int MAX_LENGTH = 1024;//最大长度

//客户端,用于测试(socket用于发送和接受数据)
int main()
{
    try {
        //创建上下文服务
        boost::asio::io_context   ioc;
        //构造endpoint
        tcp::endpoint  remote_ep(address::from_string("127.0.0.1"), 10086);
        //创建socket
        tcp::socket  sock(ioc);
        boost::system::error_code   error = boost::asio::error::host_not_found; ;
        //连接
        sock.connect(remote_ep, error);
        if (error) {
            cout << "connect failed, code is " << error.value() << " error msg is " << error.message();
            return 0;
        }

        //发送数据
        std::cout << "Enter message: ";
        char request[MAX_LENGTH];
        std::cin.getline(request, MAX_LENGTH);
        size_t request_length = strlen(request);
        boost::asio::write(sock, boost::asio::buffer(request, request_length));
        char reply[MAX_LENGTH];

        //接收数据
        size_t reply_length = boost::asio::read(sock,
            boost::asio::buffer(reply, request_length));
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
