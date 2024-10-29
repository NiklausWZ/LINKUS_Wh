#pragma once
#include "net.h"
#include <list>
#include <map>
class TcpServer :public net {
public:
	TcpServer(mediator* pNediator);
	~TcpServer();
	// 初始化网络(要么成功要么失败)
	bool initNet();
	// 发送数据 UDP是由于IP地址来决定发给谁，TCP是由socket来决定发给谁
	bool sendData(char* data, int len, long to);
	// 接受数据
	void recvData();
	// 关闭网络
	void unInitNet();
protected:
	static unsigned __stdcall acceptThread(void* lpVoid); // 接受连接的线程
	static unsigned __stdcall recvThread(void* lpVoid);// 接受数据的线程
private:
	bool m_bRunning;
	// 通过list，去保存所有的线程句柄。
	// 为什么要选择list这个stl容器？
	list<HANDLE> m_listHandle;
	// 通过线程ID去寻找Socket
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
};

// 数据结构
// vector
// list：数据不连续，查找速度慢，增加和删除数据方便
// queue：
// deque：
// heap：
// map:key-value，查找速度快
// set：