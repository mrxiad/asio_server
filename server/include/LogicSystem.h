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
#include "userModel.h"
#include "friendModel.h"

typedef function<void(shared_ptr<CSession>, short msg_id, string msg_data)> FunCallBack;
class LogicSystem:public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();

	//session层调用，需要传递LogicNode
	void PostMsgToQue(shared_ptr <LogicNode> msg);

private:
	//注册消息函数
	void RegisterCallBacks();

	//处理hello_word类型的消息
	void HelloWordCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理注册类型的消息
	void RegisterCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理登录消息
	void LoginCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理登出消息
	void logoutCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理添加好友类型的消息
	void addFriendCallBack(shared_ptr<CSession> session, short msg_id, string msg_data);
	 
	//处理一对一聊天消息
	void oneChatCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理加入群组消息
	void JoinGroupCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理创建群组消息
	void CreateGroupCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

	//处理群组聊天消息
	void groupChatCallBack(shared_ptr<CSession>, short msg_id, string msg_data);

private:
	LogicSystem();
	void DealMsg();//入口，解耦合
	std::thread _worker_thread;
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;//互斥锁
	std::condition_variable _consume;//条件变量
	bool _b_stop;//是否停止
	std::map<short, FunCallBack> _fun_callbacks;//消息id和回调函数的映射


	//操作用户表的对象
	UserModel _user_model;
	FriendModel _friend_model;
	
};


#endif