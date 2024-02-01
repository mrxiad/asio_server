#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include<thread>
#include <fstream>
#include <json/json.h>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

const int MAX_LENGTH = 1024 * 2;
const int HEAD_LENGTH = 2;
const int HEAD_TOTAL = 4;

void read_thread(tcp::socket& sock) {
    try {
        while (true) {
            char reply_head[HEAD_TOTAL];
            size_t reply_length = read(sock, buffer(reply_head, HEAD_TOTAL));
            if (reply_length == 0) break; // 检查连接是否关闭

            // 读取头部
            int msgid;
            memcpy(&msgid, reply_head, HEAD_LENGTH);
            short msglen;
            memcpy(&msglen, reply_head + HEAD_LENGTH, HEAD_LENGTH);
            msglen = boost::asio::detail::socket_ops::network_to_host_short(msglen);
            msgid = boost::asio::detail::socket_ops::network_to_host_short(msgid);

            // 读取消息体
            char msg[MAX_LENGTH] = {0};
            size_t msg_length = read(sock, buffer(msg, msglen));
            Json::Reader reader;
            Json::Value response;
            reader.parse(string(msg, msg_length), response);
            cout << "收到消息 ID: " << msgid << ", 数据: " << response.toStyledString() << endl;
        }
    } catch (std::exception& e) {
        cerr << "读取线程异常: " << e.what() << "\n";
    }
}

int main() {
    try {
        io_service ioc;
        tcp::endpoint remote_ep(ip::address::from_string("127.0.0.1"), 10086);
        tcp::socket sock(ioc);
        boost::system::error_code error = boost::asio::error::host_not_found;
        sock.connect(remote_ep, error);
        if (error) {
            cout << "连接失败: " << error.message() << "\n";
            return 0;
        }

        // 启动读取线程
        std::thread readThread(std::bind(read_thread, std::ref(sock)));

        while (true) {
            cout << "请输入要执行的操作（1: 注册, 2: 登录, 3: 退出, 4: 添加好友, 5:一对一聊天）: ";
            int operation;
            cin >> operation;

            Json::Value root;
            int msgid;
            switch (operation) {
                case 1: // 注册
                    {
                        string username, password;
                        cout << "请输入用户名: ";
                        cin >> username;
                        cout << "请输入密码: ";
                        cin >> password;
                        root["username"] = username;
                        root["password"] = password;
                        msgid = 1002; // 注册接口的 msgid
                        break;
                    }
                case 2: // 登录
                    {
                        string id, username, password;
                        cout << "请输入用户ID: ";
                        cin >> id;
                        cout << "请输入用户名: ";
                        cin >> username;
                        cout << "请输入密码: ";
                        cin >> password;
                        root["id"] = id;
                        root["username"] = username;
                        root["password"] = password;
                        msgid = 1003; // 登录接口的 msgid
                        break;
                    }
                case 3: // 退出
                    {
                        msgid = 1004; // 退出接口的 msgid
                        string id;
                        cout << "请输入用户ID: ";
                        cin >> id;
                        root["id"] = id;
                        break;
                    }
                case 4: // 添加好友
                    {
                        string userid, friendid;
                        cout << "请输入您的用户ID: ";
                        cin >> userid;
                        cout << "请输入要添加的好友ID: ";
                        cin >> friendid;
                        root["id"] = userid;
                        root["friendid"] = friendid;
                        msgid = 1005; // 添加好友接口的 msgid
                        break;
                    }
                case 5: // 一对一聊天
                    {
                        string userid, toid, message;
                        cout << "请输入您的用户ID: ";
                        cin >> userid;
                        cout << "请输入对方ID: ";
                        cin >> toid;
                        cout << "请输入消息内容: ";
                        cin.ignore();  // 忽略前一个cin的换行符
                        getline(cin, message);  // 使用getline以允许输入空格
                        root["id"] = userid;
                        root["toid"] = toid;
                        root["msg"] = message;
                        msgid = 1008; // 一对一聊天接口的msgid
                        break;
                    }
                default:
                    cout << "无效的操作！" << endl;
                    continue;
            }

            string request = root.toStyledString();
            size_t request_length = request.length();
            char send_data[MAX_LENGTH] = {0};

            // 设置 msgid 和 length
            int msgid_host = boost::asio::detail::socket_ops::host_to_network_short(msgid);
            memcpy(send_data, &msgid_host, HEAD_LENGTH);
            int request_host_length = boost::asio::detail::socket_ops::host_to_network_short(request_length);
            memcpy(send_data + HEAD_LENGTH, &request_host_length, HEAD_LENGTH);
            memcpy(send_data + HEAD_TOTAL, request.c_str(), request_length);
            write(sock, buffer(send_data, request_length + HEAD_TOTAL));

            if (operation == 3) break; // 退出操作
        }

        readThread.join();
    } catch (std::exception& e) {
        cerr << "异常: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
