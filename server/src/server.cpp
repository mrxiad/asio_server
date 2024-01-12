#include<iostream>
#include<boost/asio.hpp>
#include<set>
#include<cstdlib>
using namespace std;
using namespace boost;

const int MAX_LENGTH = 1024;//最大长度
typedef std::shared_ptr<asio::ip::tcp::socket> socket_ptr;
std::set<std::shared_ptr<std::thread>> threads;//线程集合


//会话
void session(socket_ptr sock){
    try{
        while(true){
            char data[MAX_LENGTH];
            memset(data, 0, MAX_LENGTH);
            boost::system::error_code error;//错误码

            //接受数据，但是如果客户端断开连接，也会读到数据，但是长度为0
            size_t length = sock->read_some(asio::buffer(data), error);//接收数据
            if(error == asio::error::eof){//客户端断开连接
                cout << "client close" << endl;
                break;
            }
            else if(error){
                throw boost::system::system_error(error);
            }
            cout << "receive from " << sock->remote_endpoint().address().to_string() << endl;
            cout << "receive message is " << data << endl;

            //回传信息值
            boost::asio::write(*sock, boost::asio::buffer(data, length));
        }
    }catch(std::exception& e){
        std::cerr << "Exception: " << e.what() << endl;
    }
}

void server(boost::asio::io_context& ioc, unsigned short port){
    try{
        //创建acceptor
        asio::ip::tcp::acceptor acceptor(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
        while(true){
            //创建socket
            socket_ptr sock(new asio::ip::tcp::socket(ioc));
            //等待连接
            acceptor.accept(*sock);
            //创建线程
            std::shared_ptr<std::thread> thread(new std::thread(session, sock));
            threads.insert(thread);
        }
    }catch(std::exception& e){
        std::cerr << "Exception: " << e.what() << endl;
    }
}

int main()
{
    try{
        boost::asio::io_context  ioc;
        server(ioc, 10086);
        for (auto &t : threads) {//等待其他线程结束，否则主线程结束，其他线程也会结束
            t->join();
        }
    }catch(std::exception& e){
        std::cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}