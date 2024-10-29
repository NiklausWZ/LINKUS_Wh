#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;
/*
* ��������mediator����ֱ࣬��ʹ�ã�һ��ͱ��뵽��
*/
class mediator;
class net {
public:
	// ���캯������
	net() :m_sock(INVALID_SOCKET) {}
	virtual ~net() {}
	// ��ʼ������(��ʼ���ɹ��ˣ���ʼ��ʧ����)
	virtual bool initNet() = 0;
	// ��������(�������ݳɹ��ˣ���������ʧ����)������ʲô���ݣ�����˭��
	// UDP���IP��ulong���͵ģ��������ݷ���˭��������TCP���棬socket��uint���ͣ���������˭��
	virtual bool sendData(char* data, int len, long to) = 0;
	// ��������
	virtual void recvData() = 0;
	// �ر�����
	virtual void unInitNet() = 0;
protected:
	SOCKET m_sock; // ��������
	mediator* m_pMediator;
};