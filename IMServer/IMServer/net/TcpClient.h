#pragma once
#include "net.h"


class TcpClient :public net {
public:
	TcpClient(mediator* pNediator);
	~TcpClient();
	// ��ʼ������(Ҫô�ɹ�Ҫôʧ��)
	bool initNet();
	// �������� UDP������IP��ַ����������˭��TCP����socket����������˭
	bool sendData(char* data, int len, long to);
	// ��������
	void recvData();
	// �ر�����
	void unInitNet();
	// �̺߳���������
protected:
	static unsigned __stdcall recvThread(void* lpVoid);
private:
	HANDLE m_handle;
	bool m_bRunning;
};