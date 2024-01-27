#include <iostream>
#include "CServer.h"
#include "Singleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	try {
		//获取连接池
		auto pool = AsioIOServicePool::GetInstance();
		//定义上下文，给acceptor使用
		boost::asio::io_service  io_service;

		//注册信号
		boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);

		//注册信号回调
		signals.async_wait([&io_service,pool](auto, auto) {
			io_service.stop();
			pool->Stop();
			});

		//启动逻辑线程
		CServer s(io_service, 10086);
		//启动逻辑线程
		io_service.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}

}