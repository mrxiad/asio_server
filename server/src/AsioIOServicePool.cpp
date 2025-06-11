#include"AsioIOServicePool.h"
#include <iostream>
using namespace std;


//初始化_ioServices和_works(两个vector)
AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
_works(size), _nextIOService(0){

    //初始化works
    for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::unique_ptr<Work>(new Work(_ioServices[i]));
	}


    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}

    cout<<"AsioIOServicePool init"<<endl;
}

AsioIOServicePool:: ~AsioIOServicePool(){
	std::cout << "AsioIOServicePool destruct" << endl;
}


boost::asio::io_service& AsioIOServicePool::GetIOService(){
	auto&service= _ioServices[_nextIOService++];
	if(_nextIOService == _ioServices.size()){
		_nextIOService = 0;
	}
	return service;
}


void AsioIOServicePool::Stop(){
	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
	// 这个是为什么呢？？
	for (auto& work : _works) {
		//把服务先停止
		work->get_io_service().stop();
		work.reset();
	}

	for (auto& t : _threads) {
		t.join();
	}
}
