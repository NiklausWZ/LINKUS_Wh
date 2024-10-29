#pragma once
#include "mediator.h"

class TcpServerMediator :public mediator {
public:
	TcpServerMediator();
	~TcpServerMediator();

	// ������
	bool openNet();
	// ��������
	bool sendData(char* data, int len, long to);
	// ת�����ݣ��ѽ��յ�������ת����kernel��
	void transmitData(char* data, int len, long from);
	// �ر�����
	void closeNet();
};