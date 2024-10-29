#pragma once
#include "net.h"
#include <list>
#include <map>
class TcpServer :public net {
public:
	TcpServer(mediator* pNediator);
	~TcpServer();
	// ��ʼ������(Ҫô�ɹ�Ҫôʧ��)
	bool initNet();
	// �������� UDP������IP��ַ����������˭��TCP����socket����������˭
	bool sendData(char* data, int len, long to);
	// ��������
	void recvData();
	// �ر�����
	void unInitNet();
protected:
	static unsigned __stdcall acceptThread(void* lpVoid); // �������ӵ��߳�
	static unsigned __stdcall recvThread(void* lpVoid);// �������ݵ��߳�
private:
	bool m_bRunning;
	// ͨ��list��ȥ�������е��߳̾����
	// ΪʲôҪѡ��list���stl������
	list<HANDLE> m_listHandle;
	// ͨ���߳�IDȥѰ��Socket
	map<unsigned int, SOCKET> m_mapThreadIdToSocket;
};

// ���ݽṹ
// vector
// list�����ݲ������������ٶ��������Ӻ�ɾ�����ݷ���
// queue��
// deque��
// heap��
// map:key-value�������ٶȿ�
// set��