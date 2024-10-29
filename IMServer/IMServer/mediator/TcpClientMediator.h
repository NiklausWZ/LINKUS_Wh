#pragma once
#include "mediator.h"

class TcpClientMediator :public mediator {
public:
	TcpClientMediator();
	~TcpClientMediator();

	// ������
	bool openNet();
	// ��������
	bool sendData(char* data, int len, long to);
	// ת�����ݣ��ѽ��յ�������ת����kernel��
	void transmitData(char* data, int len, long from);
	// �ر�����
	void closeNet();
};