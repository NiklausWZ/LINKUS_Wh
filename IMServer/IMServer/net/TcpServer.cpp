#include "TcpServer.h"
#include "def.h"
#include "../mediator/TcpServerMediator.h"
#include <process.h>

/*
* ��ջ�����ԭ��
* A new B �Ķ���B new A�Ķ����������캯��֮�䲻ͣ��new���󣬵ȶԷ������м��Ⱥ󵰡� 
* ��ν����Ҫ����main()�������Ѿ���һ��TcpServerMediator�Ķ����ˡ�
* ��main()�����new��һ��TcpServerMediator���ڹ��캯����Լ���һ��thisָ��ָ�������
*/

TcpServer::TcpServer(mediator* pNediator) :m_bRunning(true) {
	m_pMediator = pNediator;
}
TcpServer::~TcpServer() {

}
// ��ʼ������(Ҫô�ɹ�Ҫôʧ��)
// ���ؿ⡢�����׽��֡��󶨡������������������ӵ��߳�
bool TcpServer::initNet() {
	// 1.���ؿ�
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err = WSAStartup(version, &data);// ����ɹ�����WSAStartup����������0
	if (0 != err) {
		cout << "���ؿ�ʧ��" << endl;
		return false;
	}
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion)) {
		cout << "�汾�Ŵ���" << endl;
		return false;
	}
	else {
		cout << "���ؿ�ɹ�" << endl;
	}
	// 2.�����׽���
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "�����׽���ʧ��" << endl;
		return false;
	}else {
		cout << "�����׽��ֳɹ�" << endl;
	}
	// 3.����
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(_DEF_TCP_PORT);
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
	err = bind(m_sock, (sockaddr*)&addrServer, sizeof(addrServer));
	if (SOCKET_ERROR == err) {
		cout << "����ʧ��" << endl;
		return false;
	}
	else {
		cout << "���ӳɹ�" << endl;
	}
	// 4.����
	err = listen(m_sock, 10);
	if (SOCKET_ERROR == err) {
		cout << "����ʧ��:" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "�����ɹ�" << endl;
	}

	// 5.�����������ӵ��߳�
	HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &acceptThread, this, 0, nullptr);
	if (handle) {
		m_listHandle.push_back(handle);
	}
	return true;
}

// �����������ӵ��̺߳���
unsigned _stdcall TcpServer::acceptThread(void* lpVoid) {
	// �����һֱҪ����
	TcpServer* pThis = (TcpServer*)lpVoid;
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	unsigned int threadId = 0;
	while (pThis->m_bRunning) {
		SOCKET sockTalk = accept(pThis->m_sock, (sockaddr*)&addrClient, &addrClientSize);
		if (INVALID_SOCKET == sockTalk) {
			// ����ʧ��
			cout << "accept error" << WSAGetLastError() << endl;
		}
		else {
			// ���ӳɹ�����ӡ�ͻ��˵�ip
			cout << "accept success,client ip:" << inet_ntoa(addrClient.sin_addr) << endl;
			// �����������ݵ��̣߳�serverһ��n+1���߳̾��
			HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, pThis, 0, &threadId);
			if (handle) {
				pThis->m_listHandle.push_back(handle);
			}
			// ����socket���̺߳�socket��һһ��Ӧ�Ĺ�ϵ
			pThis->m_mapThreadIdToSocket[threadId] = sockTalk;
		}
	}
	return 1;
}
// �������ݵ��̺߳���
unsigned __stdcall TcpServer::recvThread(void* lpVoid) {
	TcpServer* pThis = (TcpServer*)lpVoid;
	pThis->recvData();
	return 1;
}
// �������ݣ�����˱�����to��������
// �ͻ��˷���ע�⣬ֻ������˷������ݣ�����send������ʱ���sock������
bool TcpServer::sendData(char* data, int len, long to) {
	// 1.У�����
	if (!data || len <= 0) {
		return false;
	}
	// 2.�ȷ�����С
	if (send(to, (char*)&len, sizeof(int), 0) <= 0) {
		cout << "sendData error1:" << WSAGetLastError() << endl;
		return false;
	}
	// 3.�ٷ�������
	if (send(to, data, len, 0) <= 0) {
		cout << "sendData error2:" << WSAGetLastError() << endl;
		return false;
	}
	return true;
}
// ��������
void TcpServer::recvData() {
	// ����һ��(��ֹmap�л�û�б���socket)
	Sleep(10);
	// ��map��ȡ������ǰ�̶߳�Ӧ��socket
	SOCKET sock = m_mapThreadIdToSocket[GetCurrentThreadId()];
	if (!sock || INVALID_SOCKET == sock) {
		cout << "recvData socket error" << endl;
		return;
	}
	int nRecvNum = 0;
	int packSize = 0;
	int nOffset = 0;// ƫ��������¼һ�������յ���������
	while (m_bRunning) {
		// ���ܰ���С
		// ���ض��������ֽڴ�С����������������رգ��򷵻�ֵΪ��
		nRecvNum = recv(sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {
			// �������ݳɹ���֪������С��
			char* recvBuf = new char[packSize];
			// ���ܰ�����
			while (packSize > 0) {
				nRecvNum = recv(sock, recvBuf + nOffset, packSize, 0);
				if (nRecvNum > 0) {
					nOffset += nRecvNum;
					packSize -= nRecvNum;
				}
				else {
					cout << "recvData error2:" << WSAGetLastError() << endl;
					break;
				}
			}
			// ���ܰ����ݳɹ��������ݴ����н���
			m_pMediator->transmitData(recvBuf, nOffset, sock);
	
			// ƫ��������
			nOffset = 0;
		}
		else {
			cout << "recvData error1:" << WSAGetLastError() << endl;
			break;
		}
	}
}
// �ر�����
void TcpServer::unInitNet() {
	// 1.�����߳�
	m_bRunning = false;
	for (auto ite = m_listHandle.begin(); ite != m_listHandle.end();) {
		HANDLE handle = *ite;
		TerminateThread(handle, -1);// �ر�handle
		if (WAIT_TIMEOUT == WaitForSingleObject(handle, 1000)) {
			TerminateThread(handle, -1);
		}
		CloseHandle(handle);
		handle = nullptr;
		// ��list���Ƴ��Ѿ��رյ�handle�����µ�����
		ite = m_listHandle.erase(ite);
	}

	// 2.�ر��׽���
	if (!m_sock && m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
	for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
		SOCKET sock = ite->second;
		if (!sock && sock != INVALID_SOCKET) {
			closesocket(sock);
		}
		// ��map���Ƴ���Ч�ڵ�
		ite = m_mapThreadIdToSocket.erase(ite);
	}
	// 3.ж�ؿ�
	WSACleanup();
}
