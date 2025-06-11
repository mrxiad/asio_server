#include"AsioIOContextPool.h"
#include <iostream>
using namespace std;


//初始化_ioContexts和_works(两个vector)
AsioIOContextPool::AsioIOContextPool(std::size_t size):_ioContexts(size),
_works(size), _nextIOContext(0){

    //初始化works
    for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::unique_ptr<Work>(new Work(_ioContexts[i]));
	}


    //遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioContexts.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioContexts[i].run();
			});
	}

    cout<<"AsioIOContextPool init"<<endl;
}

AsioIOContextPool:: ~AsioIOContextPool(){
	std::cout << "AsioIOContextPool destruct" << endl;
}


boost::asio::io_context& AsioIOContextPool::GetIOContext(){
	auto&service= _ioContexts[_nextIOContext++];
	if(_nextIOContext == _ioContexts.size()){
		_nextIOContext = 0;
	}
	return service;
}


void AsioIOContextPool::Stop(){
	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
	// 这个是为什么呢？？
	for (auto& work : _works) {
		//把服务先停止
		work->get_io_context().stop();
		work.reset();
	}

	for (auto& t : _threads) {
		t.join();
	}
}
