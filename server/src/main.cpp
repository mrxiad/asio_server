#include <iostream>
#include "CServer.h"
#include "Singleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOContextPool.h"
using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	try {
                //获取连接池,单例模式，这里 io_context 直接供 session 使用
		auto pool = AsioIOContextPool::GetInstance();

                //定义上下文，给acceptor使用
                boost::asio::io_context  io_context;

		//注册信号
                boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

		//注册信号回调
                signals.async_wait([&io_context,pool](auto, auto) {
                        io_context.stop();
                        pool->Stop();
                        });

		//启动逻辑线程
                CServer s(io_context, 10086);
                //启动逻辑线程
                io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}

}