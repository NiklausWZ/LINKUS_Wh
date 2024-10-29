#pragma once
#include "mediator/mediator.h"
#include <iostream>
#include "MySQL/CMySql.h"
#include "net/def.h"
#include <map>
using namespace std;

// 声明函数指针
class CKernel;
// 这是一个指向类的成员函数的函数指针PFUN，但是呢类是在下面定义的。
typedef void (CKernel::*PFUN)(char* data, int len, long from);

class CKernel{
public:
	//静态指针
	static CKernel* pKernel;
	CKernel();
	~CKernel();
	// 打开服务器
	bool startServer();
	// 关闭服务器
	void closeServer();
	// 处理所有接收到的数据
	void dealData(char* data, int len, long from);
	// 绑定协议头
	void setProtocolMap();
	// 获得用户的好友信息（包括自己)
	void getUserInfo(int userId);
	// 根据用户id查询用户信息
	void getInfoById(int id, _STRU_FRIEND_INFO* info);
	// 处理注册请求
	void dealRegisterRq(char* data, int len, long from);
	// 处理登录请求
	void dealLoginRq(char* data, int len, long from);
	
	// 处理聊天请求
	void dealChatRq(char* data, int len, long from);

	// 处理客户端下线请求
	void dealClientoffline(char* data, int len, long from);

	// 处理添加好友请求
	void dealAddFriendRq(char* data, int len, long from);

	// 处理添加好友回复
	void dealAddFriendRs(char* data, int len, long from);
private:
	mediator* m_pMediator;// 中介者指针
	
	// 函数指针数组
	PFUN m_mapProtocol[_DEF_PROTOCOL_COUNT];
	// 数据库对象
	CMySql m_mysql;
	// 好友socket的map
	map<int, SOCKET> m_mapIdToSocket;
};

