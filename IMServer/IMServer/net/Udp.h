#pragma once
#include "net.h"

class Udp :public net {
public:
	Udp();
	~Udp();

	bool initNet();

	bool sendData(char* data, int len, long to);

	void recvData();

	void unInitNet();
private:
	HANDLE m_handle;
};