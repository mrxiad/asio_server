#include "LogicSystem.h"

using namespace std;

LogicSystem::LogicSystem():_b_stop(false){
	RegisterCallBacks();
	_worker_thread = std::thread (&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem(){
	_b_stop = true;
	_consume.notify_one();
	_worker_thread.join();
}

//session中read后调用，投递到消息队列里,用于读取消息
void LogicSystem::PostMsgToQue(shared_ptr<LogicNode> msg) {
	std::unique_lock<std::mutex> unique_lk(_mutex);
	_msg_que.push(msg);
	//由0变为1则发送通知信号
	if (_msg_que.size() == 1) {
		unique_lk.unlock();
		_consume.notify_one();
	}
}

void LogicSystem::DealMsg() {
	for (;;) {
		std::unique_lock<std::mutex> unique_lk(_mutex);
		//判断队列为空则用条件变量阻塞等待，并释放锁
		while (_msg_que.empty() && !_b_stop) {
			_consume.wait(unique_lk);
		}

		//判断是否为关闭状态，把所有逻辑执行完后则退出循环
		if (_b_stop ) {
			while (!_msg_que.empty()) {
				auto msg_node = _msg_que.front();
				cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
				auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
				if (call_back_iter == _fun_callbacks.end()) {
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
					std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
				_msg_que.pop();
			}
			break;
		}

		//如果没有停服，且说明队列中有数据，利用回调函数去处理，回调函数中发送相应
		auto msg_node = _msg_que.front();
		cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
		auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
		if (call_back_iter == _fun_callbacks.end()) {
			_msg_que.pop();
			continue;
		}

		/*
			注意这里调用了回调函数（session，id，data），这里的data是recvnode的data，纯消息体
		*/
		call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id, 
			std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
		_msg_que.pop();
	}
}

void LogicSystem::RegisterCallBacks() {

	//注册hello_word类型的消息
	_fun_callbacks.insert(make_pair(MSG_HELLO_WORD, std::bind(&LogicSystem::HelloWordCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//注册类型的消息
	_fun_callbacks.insert(make_pair(MSG_REGISTER, std::bind(&LogicSystem::RegisterCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//注册登录消息
	_fun_callbacks.insert(make_pair(MSG_LOGIN, std::bind(&LogicSystem::LoginCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));
}
/*
	以下是注册回调函数，注意：需要session，id，data
	-session是需要知道客户端
	-id是消息类型，用于send
	-data是纯消息体（json）
*/


//hello_word
void LogicSystem::HelloWordCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << endl;
	std::cout<<"进入hello_word回调函数"<<endl;
	//需要在这里构造回复的消息
	root["data"] = "server has received msg, msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, msg_id);//发送消息
}


//注册
void LogicSystem::RegisterCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << endl;
	std::cout<<"进入注册回调函数"<<endl;
	//需要在这里构造回复的消息
	root["data"] = "server has received msg, msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, msg_id);//发送消息
}

//登录
void LogicSystem::LoginCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << endl;
	std::cout<<"进入登录回调函数"<<endl;
	//需要在这里构造回复的消息
	root["data"] = "server has received msg, msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, msg_id);//发送消息
}