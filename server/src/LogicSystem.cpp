#include "LogicSystem.h"
#include "string"
#include "code.h"
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

	//注册消息
	_fun_callbacks.insert(make_pair(MSG_REGISTER, std::bind(&LogicSystem::RegisterCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//登录消息
	_fun_callbacks.insert(make_pair(MSG_LOGIN, std::bind(&LogicSystem::LoginCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//登出消息
	_fun_callbacks.insert(make_pair(MSG_LOGOUT, std::bind(&LogicSystem::logoutCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//添加好友消息
	_fun_callbacks.insert(make_pair(MSG_ADD_FRIEND, std::bind(&LogicSystem::addFriendCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

	//一对一聊天消息
	_fun_callbacks.insert(make_pair(MSG_ONE_CHAT, std::bind(&LogicSystem::oneChatCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));
	
	//加入群组消息
	_fun_callbacks.insert(make_pair(MSG_JOIN_GROUP, std::bind(&LogicSystem::JoinGroupCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));
	
	//创建群组消息
	_fun_callbacks.insert(make_pair(MSG_CREATE_GROUP, std::bind(&LogicSystem::CreateGroupCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));
	
	//群组聊天消息
	_fun_callbacks.insert(make_pair(MSG_GROUP_CHAT, std::bind(&LogicSystem::groupChatCallBack, this,
		placeholders::_1, placeholders::_2, placeholders::_3)));

}
/*
	以下是注册回调函数，注意：需要session，id，data
	-session是需要知道客户端
	-id是消息类型，用于send
	-data是纯消息体（json）
*/

/*
	注意，一旦进入回调函数，server中一定会有这个session，直到读写发生错误（对端关闭连接），或者实现退出业务，主动删除session
*/
//hello_word
void LogicSystem::HelloWordCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << msg_id << " msg data is "
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
	Json::Value response;//返回消息
	reader.parse(msg_data, root);
	
	std::cout<<"进入注册回调函数"<<endl;
	
	/*
        获取用户名和密码然后插入数据库
        返回是否插入成功
    */
    std::string username = root["username"].asString();
	std::string password = root["password"].asString();
	std::cout<<"获取到的用户名是"<<username<<endl;
	std::cout<<"获取到的密码是"<<password<<endl;
	
	User user;
	user.setName(username);
	user.setPassword(password);

	bool is_insert_success = true;
	is_insert_success = _user_model.insert(user);//插入数据库

	if(is_insert_success){
		response["code"] = CODE_REGISTER_SUCCESS;
		response["id"]=user.getId();
		response["msg"] = "register success";
	}
	else {
		response["code"] = CODE_REGISTER_FAILED;
		response["msg"] = "register failed";
	}
	std::string return_str = response.toStyledString();
	session->Send(return_str, msg_id);//发送消息
}

//登录
void LogicSystem::LoginCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	Json::Value response;//返回消息
	reader.parse(msg_data, root);
	std::cout<<"进入登录回调函数"<<std::endl;
	std::cout<<root<<endl;
	/*
		获取id,用户名,密码
	*/
	string id_str=root["id"].asString();
	int id=stoi(id_str);
	std::string username = root["username"].asString();
	std::string password = root["password"].asString();
	/*
		查询数据库
		返回是否查询成功
	*/
	bool is_login_success = true;
	User user;
	user = _user_model.query(id);
	if(user.getId()==-1){//查询失败
		is_login_success=false;
	}
	else if(user.getName()!=username||user.getPassword()!=password){
		is_login_success=false;
	}else if(user.getState()=="online"){
		is_login_success=false;
	}
	/*
		构造返回消息
	*/
	
	if(is_login_success){//登录成功
		
		//设置用户状态为在线,并且设置uuid
		user.setState("online");
		std::cout<<"设置用户的uuid:"<<session->GetUuid()<<endl;
		user.setUuid(session->GetUuid());

		//更新数据库
		_user_model.updateState(user);

		response["code"] = CODE_LOGIN_SUCCESS;
		response["msg"] = "login success";
	}
	else {//登录失败
		response["code"] = CODE_LOGIN_FAILED;
		response["msg"] = "login failed";
	}
	std::string return_str = response.toStyledString();
	session->Send(return_str, msg_id);//发送消息
}


//退出回调函数
void LogicSystem::logoutCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	Json::Value response;//返回消息
	reader.parse(msg_data, root);
	std::cout<<"进入退出回调函数"<<endl;
	
	//需要在这里构造回复的消息
	response["code"] = CODE_LOGOUT_SUCCESS;
	response["msg"] = "logout success";
	std::string return_str = response.toStyledString();
	session->Send(return_str, msg_id);//发送消息
	//设置用户状态为离线
	User user(root["id"].asInt(), "", "", "offline");
	_user_model.updateState(user);
	//此时对端肯定会关闭连接，服务器不需要处理
}


// 添加好友业务回调
void LogicSystem::addFriendCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
    Json::Reader reader;
    Json::Value root;
    Json::Value response;

    reader.parse(msg_data, root);
    std::cout << "添加好友回调" << endl;
	std::cout<<root<<endl;
    /*
        id:用户id
	    friendid:好友id
    */

    string id_str=root["id"].asString();
	string friendid_str=root["friendid"].asString();
	int id=stoi(id_str);
	int friendid=stoi(friendid_str);
    
    /*
		操作数据库
	*/
	bool is_add_success = true;
	is_add_success = _friend_model.insert(id, friendid);

	/*
		返回结果
	*/
    if (is_add_success) {
        response["code"] = CODE_ADD_FRIEND_SUCCESS;
        response["msg"] = "add friend success";
    } else {
        response["code"] = CODE_ADD_FRIEND_FAILED;
        response["msg"] = "add friend failed";
    }

    std::string return_str = response.toStyledString();
    session->Send(return_str, msg_id); // 发送消息
}


void LogicSystem::oneChatCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	Json::Value response;//返回消息
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << msg_id << " msg data is "
		<< root["data"].asString() << endl;
	std::cout<<"进入单聊回调函数"<<endl;
}

// 创建群组业务回调
void LogicSystem::CreateGroupCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
    Json::Reader reader;
    Json::Value root;
    Json::Value response;

    reader.parse(msg_data, root);
    std::cout << "Received message ID: " << msg_id << " Message data: " << root["data"].asString() << endl;
    std::cout << "Entering create group callback function" << endl;

    /*
        解析参数并执行创建群组的业务逻辑（待实现）
        设置is_create_success的值
    */
    bool is_create_success = true;
    /*
        构造返回消息
    */
    if (is_create_success) {
        response["code"] = CODE_CREATE_GROUP_SUCCESS;
        response["data"] = "create group success";
    } else {
        response["code"] = CODE_CREATE_GROUP_FAILED;
        response["data"] = "create group failed";
    }

    std::string return_str = response.toStyledString();
    session->Send(return_str, msg_id); // 发送消息
}

// 加入群组业务回调
void LogicSystem::JoinGroupCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
    Json::Reader reader;
    Json::Value root;
    Json::Value response;

    reader.parse(msg_data, root);
    std::cout << "Received message ID: " << msg_id << " Message data: " << root["data"].asString() << endl;
    std::cout << "Entering join group callback function" << endl;


	bool is_join_success = true;
    /*
        解析参数并执行加入群组的业务逻辑（待实现）
        设置is_join_success的值
    */
    /*
        构造返回消息
    */
    if (is_join_success) {
        response["code"] = CODE_JOIN_GROUP_SUCCESS;
        response["data"] = "join group success";
    } else {
        response["code"] = CODE_JOIN_GROUP_FAILED;
        response["data"] = "join group failed";
    }

    std::string return_str = response.toStyledString();
    session->Send(return_str, msg_id); // 发送消息
}


//群组聊天功能
void LogicSystem::groupChatCallBack(shared_ptr<CSession> session, short msg_id, string msg_data) {
	Json::Reader reader;
	Json::Value root;
	Json::Value response;//返回消息
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << msg_id << " msg data is "
		<< root["data"].asString() << endl;
	std::cout<<"进入群组聊天回调函数"<<endl;
	

}

