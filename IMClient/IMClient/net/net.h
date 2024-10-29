#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;
/*
* 先声明有mediator这个类，直接使用，一会就编译到了
*/
class mediator;
class net {
public:
	// 构造函数，先
	net() :m_sock(INVALID_SOCKET) {}
	virtual ~net() {}
	// 初始化网络(初始化成功了，初始化失败了)
	virtual bool initNet() = 0;
	// 发送数据(发送数据成功了，发送数据失败了)，发送什么数据，发给谁。
	// UDP里，由IP是ulong类型的，决定数据发给谁。但是在TCP里面，socket是uint类型，决定发给谁。
	virtual bool sendData(char* data, int len, long to) = 0;
	// 接受数据
	virtual void recvData() = 0;
	// 关闭网络
	virtual void unInitNet() = 0;
protected:
	SOCKET m_sock; // 接受连接
	mediator* m_pMediator;
};
