#ifndef ASIO_IOCONTEXT_POOL_H
#define ASIO_IOCONTEXT_POOL_H


#include <boost/asio.hpp>
#include <vector>
#include "Singleton.h"
#include <thread>

class AsioIOContextPool:public Singleton<AsioIOContextPool>
{
    friend Singleton<AsioIOContextPool>;
public:
    using IOContext = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;

    ~AsioIOContextPool();

	AsioIOContextPool(const AsioIOContextPool&) = delete;
	AsioIOContextPool& operator=(const AsioIOContextPool&) = delete;

    //轮询获取io_context
	boost::asio::io_context& GetIOContext();

    //停止所有的io_context
	void Stop();

private:
    //构造函数
    AsioIOContextPool(std::size_t size = std::thread::hardware_concurrency());
    //多个io_context的运行函数
    std::vector<IOContext> _ioContexts;
    //多个works，防止io_context退出，因为在没有任务的时候，io_context会退出
	std::vector<WorkPtr> _works;
    //多个线程
	std::vector<std::thread> _threads;
    //下一个io_context的索引
	std::size_t                        _nextIOContext;

};

#endif // ASIO_IOCONTEXT_POOL_H
