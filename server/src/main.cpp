#include <iostream>
#include "CServer.h"
#include "Singleton.h"
#include "LogicSystem.h"
#include <csignal>
#include <thread>
#include <mutex>
using namespace std;
bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;
void sig_handler(int sig)//实现优雅退出
{
	if (sig == SIGINT||sig == SIGTERM)
	{
		std::unique_lock<std::mutex>  lock_quit(mutex_quit);
		bstop = true;
		lock_quit.unlock();
		cond_quit.notify_one();
	}
}

int main()
{
	try {
		boost::asio::io_service  io_service;
		std::thread  net_work_thread([&io_service] {
			CServer s(io_service, 10086);
			io_service.run();
			});
		signal(SIGINT, sig_handler);
		while (!bstop) {
			std::unique_lock<std::mutex>  lock_quit(mutex_quit);
			cond_quit.wait(lock_quit);
		}
		io_service.stop();
		net_work_thread.join();

	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << endl;
	}
	
}