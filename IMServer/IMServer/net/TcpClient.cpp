#include "TcpClient.h"
#include "def.h"
#include "../mediator/TcpClientMediator.h"
#include <process.h>
TcpClient::TcpClient(mediator* pNediator):m_handle(nullptr),m_bRunning(true){
	m_pMediator = pNediator;
}
TcpClient::~TcpClient() {

}
// ��ʼ������(Ҫô�ɹ�Ҫôʧ��)
bool TcpClient::initNet() {
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
	}else {
		cout << "���ؿ�ɹ�" << endl;
	}
	// 2.�����׽���
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		cout << "�����׽���ʧ��" << endl;
		return false;
	}else {
		cout << "�����׽��ֳɹ�" << endl;
	}
	// 3.����
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	// ħ������
	addrServer.sin_port = htons(_DEF_TCP_PORT);
	addrServer.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
	err = connect(m_sock, (sockaddr*)&addrServer, sizeof(addrServer));// ���ӳɹ��������㡣
	if (0 != err) {
		cout << "����ʧ��" << endl;
		return false;
	}else {
		cout << "���ӳɹ�" << endl;
	}
	// 4.�����������ݵ��߳�(������Ҫ����)
	// CreateThread��ExitThread��һ�ԣ�����߳�ʹ����C++����ʱ��ĺ���������(strcpy����ռ䲻�ͷ�)
	// ExitThreadҲ�����ͷſռ䣬�ͻ�����ڴ�й¶
	// _beginthreadex��_endthreadex��һ�ԣ�_endthreadex���Ȼ��տռ䣬�ٵ���ExitThread
	m_handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, this, 0, nullptr);
	return true;
}

// �̺߳����������Ϊ��ĳ�Ա����
// ��̬��Ա�������ܵ��÷Ǿ�̬��Ա������������Ҫ���ö��� this ����
unsigned __stdcall TcpClient::recvThread(void* lpVoid) {
	TcpClient* pThis = (TcpClient*)lpVoid;
	pThis->recvData();
	return 1;
}

// �������� UDP������IP��ַ����������˭��TCP����socket����������˭
// tcpЭ������4�ֽ��ճ�����⣬�ӱ�־λ���̶�����С�������ӣ��ȷ�����С�ٷ�������
// �ӱ�־λΪʲô�����ã���Ϊ�������û���������ݣ�����������ݺͱ�־λ�ظ�֮��Ҳ�������ˡ�
bool TcpClient::sendData(char* data, int len, long to) {
	// 1.У�����
	if (!data || len <= 0) {
		return false;
	}
	// 2.�ȷ�����С
	if (send(m_sock, (char*)&len, sizeof(int), 0) <= 0) {
		cout << "sendData error1:" << WSAGetLastError() << endl;
		return false;
	}
	// 3.�ٷ�������
	if (send(m_sock, data, len, 0) <= 0) {
		cout << "sendData error2:" << WSAGetLastError() << endl;
		return false;
	}
	return true;
}
// �������ݣ��Ƚ��ܰ���С
void TcpClient::recvData() {
	int nRecvNum = 0;
	int packSize = 0;
	int nOffset = 0;// ƫ��������¼һ�������յ���������
	while (m_bRunning) {
		// ���ܰ���С
		// ���ض��������ֽڴ�С����������������رգ��򷵻�ֵΪ��
		nRecvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {
			// �������ݳɹ���֪������С��
			char* recvBuf = new char[packSize];
			// ���ܰ�����
			while (packSize > 0) {
				nRecvNum = recv(m_sock, recvBuf + nOffset, packSize, 0);
				if (nRecvNum > 0) {
					nOffset += nRecvNum;
					packSize -= nRecvNum;
				}else {
					cout << "recvData error2:" << WSAGetLastError() << endl;
					break;
				}
			}
			// ���ܰ����ݳɹ��������ݴ����н���
			m_pMediator->transmitData(recvBuf, nOffset, m_sock);
			// ƫ��������
			nOffset = 0;
		}else {
			cout << "recvData error1:" << WSAGetLastError() << endl;
			break;
		}
	}
}
// �ر�����
void TcpClient::unInitNet() {
	// 1.�����߳�
	m_bRunning = false;
	if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 1000)) {
		TerminateThread(m_handle, -1);
	}
	CloseHandle(m_handle);
	m_handle = nullptr;
	// 2.�ر��׽���
	if (!m_sock && m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
	// 3.ж�ؿ�
	WSACleanup();
}
