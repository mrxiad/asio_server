#ifndef LOGIC_SYSREM_H
#define LOGIC_SYSREM_H


#include "Singleton.h"
#include <queue>
#include <thread>
#include "CSession.h"
#include <condition_variable>
#include <queue>
#include <map>
#include <functional>
#include "const.h"
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

typedef function<void(shared_ptr<CSession>, short msg_id, string msg_data)> FunCallBack;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(shared_ptr <LogicNode> msg);
private:
	LogicSystem();
	void DealMsg();//入口，解耦合
	void RegisterCallBacks();
	void HelloWordCallBack(shared_ptr<CSession>, short msg_id, string msg_data);
	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;//互斥锁
	std::condition_variable _consume;//条件变量
	bool _b_stop;//是否停止
	std::map<short, FunCallBack> _fun_callbacks;//消息id和回调函数的映射
};


#endif