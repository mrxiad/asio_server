#include "CSession.h"
#include"CServer.h"
#include"json/json.h"
#include"LogicSystem.h"
#include<iostream>
#include<sstream>
CSession::CSession(boost::asio::io_service& io_service, CServer* server):
	_socket(io_service), _server(server), _b_close(false),_b_head_parse(false){
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_uuid = boost::uuids::to_string(a_uuid);
	_recv_head_node = std::make_shared<MsgNode>(HEAD_TOTAL_LEN);
}
CSession::~CSession() {
	std::cout << "~CSession destruct" << endl;
}

tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string& CSession::GetUuid() {
	return _uuid;
}

void CSession::Start(){
	::memset(_data, 0, MAX_LENGTH);
	_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH), std::bind(&CSession::HandleRead, this, 
		std::placeholders::_1, std::placeholders::_2, SharedSelf()));
}

void CSession::Send(std::string msg, short msgid) {
	std::lock_guard<std::mutex> lock(_send_lock);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE) {
		std::cout << "session: " << _uuid << " send que fulled, size is " << MAX_SENDQUE << endl;
		return;
	}

	_send_que.push(make_shared<SendNode>(msg.c_str(), msg.length(), msgid));
	if (send_que_size > 0) {
		return;
	}
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}

void CSession::Send(char* msg, short max_length, short msgid) {
	std::lock_guard<std::mutex> lock(_send_lock);
	int send_que_size = _send_que.size();
	if (send_que_size > MAX_SENDQUE) {
		std::cout << "session: " << _uuid << " send que fulled, size is " << MAX_SENDQUE << endl;
		return;
	}

	_send_que.push(make_shared<SendNode>(msg, max_length, msgid));
	if (send_que_size>0) {
		return;
	}
	auto& msgnode = _send_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len), 
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}

void CSession::Close() {
	_socket.close();
	_b_close = true;
}

std::shared_ptr<CSession>CSession::SharedSelf() {
	return shared_from_this();
}

void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self) {
	//增加异常处理
	try {
		if (!error) {
			std::lock_guard<std::mutex> lock(_send_lock);
			//cout << "send data " << _send_que.front()->_data+HEAD_LENGTH << endl;
			_send_que.pop();//发送成功后删除队列头部
			if (!_send_que.empty()) {
				auto& msgnode = _send_que.front();
				boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_self));
			}
		}
		else {
			std::cout << "handle write failed, error is " << error.message() << endl;
			Close();
			_server->ClearSession(_uuid);
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception code : " << e.what() << endl;
	}
	
}

void CSession::HandleRead(const boost::system::error_code& error, size_t  bytes_transferred, std::shared_ptr<CSession> shared_self){
	try {
		if (!error) {
			//已经移动的字符数
			int copy_len = 0;

			/*
				大致流程如下：

				1.最开始一定会先解析头部
				2.解析头部的时候会有两种情况:	a.此时的数据+收到的数据<头部长度，说明数据未收全，异步读取（递归），return
										  	  b.此时的数据+收到的数据>=头部长度，先把头部的读取出来，然后解析头部，然后解析部分消息体
											  c.部分消息体如果特别长，比头部传递过来的len还要长，此时_b_head_parse为true，继续异步读取（递归），return
											  d.无论如何，当头部解析完成，_b_head_parse置为true，后面是否变为false另说
				3.当_b_head_parse为true时候，解析消息体
				4.具体流程看代码注释

				5.注意，如果消息过于长，转化到处理头部即可，运用continue即可，不可以递归
			*/


			while (bytes_transferred > 0) {
				/*
					1.最开始的时候一定是在存储头部，但是之后就不一定了
					注意：若想进入头部解析，只需要_b_head_parse置为false，congtinue即可，因为bytes_transferred>0
						 或者_b_head_parse置为false，调用异步读
				*/
				if (!_b_head_parse) {

					/*
						2.如果已经存储的数据+收到的数据<头部，存储进去，继续递归接受
						注意，这里面是直接接受的字节流，还没有解析呢
					*/
					if (bytes_transferred + _recv_head_node->_cur_len < HEAD_TOTAL_LEN) {
						memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, bytes_transferred);
						_recv_head_node->_cur_len += bytes_transferred;
						::memset(_data, 0, MAX_LENGTH);
						_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
							std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
						return;
					}
					/*
						3.如果已经存储的数据+收到的数据>=头部，则需要特殊存储				 
					*/
					int head_remain = HEAD_TOTAL_LEN - _recv_head_node->_cur_len;
					memcpy(_recv_head_node->_data + _recv_head_node->_cur_len, _data + copy_len, head_remain);
					/*
						4.此时头部已经存储完成，该解析了(转化为本地字节序)，这样才可以直到id和len，方便消息体的接收
						此时有一个问题，如果收到的数据特别多，比如>>len，此时跨越了好几个消息，如何运用继续运用这个函数呢？
						仔细看8是如何解释的
					*/

					/*
						5.注意id和len非法的情况，此时在server里删除session即可，这样肯定这个session被关掉了
						原因：server内部对session的引用计数-1，而这个函数结束后，session引用计数-1，并且没有其他地方引用，所以session被析构
					*/
					copy_len += head_remain;
					bytes_transferred -= head_remain;
					//获取头部MSGID数据
					short msg_id = 0;
					memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
					//网络字节序转化为本地字节序
					msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
					std::cout << "msg_id is " << msg_id << endl;
					//id非法
					if (msg_id > MAX_LENGTH) {
						std::cout << "invalid msg_id is " << msg_id << endl;
						_server->ClearSession(_uuid);//删除session
						return;
					}
					short msg_len = 0;
					memcpy(&msg_len, _recv_head_node->_data+HEAD_ID_LEN, HEAD_DATA_LEN);
					//网络字节序转化为本地字节序
					msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
					std::cout << "msg_len is " << msg_len << endl;
					//len非法
					if (msg_len > MAX_LENGTH) {
						std::cout << "invalid data length is " << msg_len << endl;
						_server->ClearSession(_uuid);//删除session
						return;
					}
					/*
						6.根据len和id初始化一个消息体，注意，接受消息不需要转化为字节序，因为是等待处理的
						而转化为字节序是为了发送
					*/
					_recv_msg_node = make_shared<RecvNode>(msg_len, msg_id);

					/*
						7.已经存储的消息长度+接收到的消息<长度,存储后继续监听，下一次递归必然不走"_b_head_parse==false的分支"(本分支)
						注意_b_head_parse要变为true，因为头部已经解析完成了

						有可能会有疑问，这个已经存储的长度是否一定为0？不一定为0，看8的注释
					*/
					if (bytes_transferred < msg_len) {
						memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
						_recv_msg_node->_cur_len += bytes_transferred;
						::memset(_data, 0, MAX_LENGTH);
						//头部处理完成
						_b_head_parse = true;
						_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
							std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
						return;
					}

					/*
						8.已经存储的长度+接收到的消息长度>=len
						则此消息接受完成，需要投递到逻辑队列中。
						注意，此时可能会剩余长度，要存储下一个消息体，所以不可以直接调用异步读，必须将剩余的数据全部处理完才可以异步读
						看注释9
					*/
					memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, msg_len);
					_recv_msg_node->_cur_len += msg_len;
					copy_len += msg_len;
					bytes_transferred -= msg_len;
					_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
					//cout << "receive data is " << _recv_msg_node->_data << endl;
					//此处将消息投递到逻辑队列中
					LogicSystem::GetInstance()->PostMsgToQue(make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
				
					/*
						9.头部解析改为false，注意，剩余消息的长度可能很长，所以返回函数的初始点，直接调用continue即可，不可以递归。
						如果所有数据都被处理完，才允许调用递归。
						无论如何，此时_b_head_parse一定为false
					*/
					_b_head_parse = false;
					_recv_head_node->Clear();
					if (bytes_transferred <= 0) {
						::memset(_data, 0, MAX_LENGTH);
						_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
							std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
						return;
					}
					continue;//不要异步读（递归），因为本轮数据没处理完毕
				}

				/*
					10.此时已经处理完头部，正在处理消息体，能进入此分支，必须是头部处理完并且消息体没有处理
					_b_head_parse一定为true,注意解析完消息体后重置
				*/
				int remain_msg = _recv_msg_node->_total_len - _recv_msg_node->_cur_len;

				/*
					11.此时已经存储的数据+收到的数据<len，存储后异步读
				*/
				if (bytes_transferred < remain_msg) {
					memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, bytes_transferred);
					_recv_msg_node->_cur_len += bytes_transferred;
					::memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
						std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
					return;
				}
				/*
					12.此时存储的数据+收到的数据>=len，存储后消息体后投递到逻辑队列中
					_b_head_parse置为false，因为可能还有数据所以用continue
					如果没有数据的时候，才可以调用异步读。
				*/
				memcpy(_recv_msg_node->_data + _recv_msg_node->_cur_len, _data + copy_len, remain_msg);
				_recv_msg_node->_cur_len += remain_msg;
				bytes_transferred -= remain_msg;
				copy_len += remain_msg;
				_recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
				//cout << "receive data is " << _recv_msg_node->_data << endl;

				//此处将消息投递到逻辑队列中
				LogicSystem::GetInstance()->PostMsgToQue(make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
				
				//继续轮询剩余未处理数据
				_b_head_parse = false;
				_recv_head_node->Clear();
				if (bytes_transferred <= 0) {
					::memset(_data, 0, MAX_LENGTH);
					_socket.async_read_some(boost::asio::buffer(_data, MAX_LENGTH),
						std::bind(&CSession::HandleRead, this, std::placeholders::_1, std::placeholders::_2, shared_self));
					return;
				}
				continue;
			}
		}
		else {
			/*
				13.对端关闭连接后，需要手动调用close，关闭socket
				server删除session
			*/
			std::cout << "handle read failed, error is " << error.message() << endl;
			Close();
			_server->ClearSession(_uuid);
		}
	}
	catch (std::exception& e) {
		std::cout << "Exception code is " << e.what() << endl;
	}
}
