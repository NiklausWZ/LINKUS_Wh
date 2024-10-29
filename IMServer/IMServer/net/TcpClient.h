#pragma once
#include "net.h"


class TcpClient :public net {
public:
	TcpClient(mediator* pNediator);
	~TcpClient();
	// 初始化网络(要么成功要么失败)
	bool initNet();
	// 发送数据 UDP是由于IP地址来决定发给谁，TCP是由socket来决定发给谁
	bool sendData(char* data, int len, long to);
	// 接受数据
	void recvData();
	// 关闭网络
	void unInitNet();
	// 线程函数的声明
protected:
	static unsigned __stdcall recvThread(void* lpVoid);
private:
	HANDLE m_handle;
	bool m_bRunning;
};