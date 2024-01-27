#ifndef ASIO_IOSERVICE_POOL_H
#define ASIO_IOSERVICE_POOL_H


#include <boost/asio.hpp>
#include <vector>
#include "Singleton.h"
#include <thread>

class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
    friend Singleton<AsioIOServicePool>;
public:
    using IOService = boost::asio::io_service;
	using Work = boost::asio::io_service::work;
	using WorkPtr = std::unique_ptr<Work>;

    //如果用智能指针，必须放到public，否则智能指针类访问不到
    ~AsioIOServicePool();

	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;

    //轮询获取io_service
	boost::asio::io_service& GetIOService();

    //停止所有的io_service
	void Stop();

private:
    //构造函数
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
    //多个io_service的运行函数
    std::vector<IOService> _ioServices;
    //多个works，防止io_service退出，因为在没有任务的时候，io_service会退出
	std::vector<WorkPtr> _works;
    //多个线程
	std::vector<std::thread> _threads;
    //下一个io_service的索引
	std::size_t                        _nextIOService;

};

#endif // ASIO_IOSERVICE_POOL_H