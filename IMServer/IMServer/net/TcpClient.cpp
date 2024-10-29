#include "TcpClient.h"
#include "def.h"
#include "../mediator/TcpClientMediator.h"
#include <process.h>
TcpClient::TcpClient(mediator* pNediator):m_handle(nullptr),m_bRunning(true){
	m_pMediator = pNediator;
}
TcpClient::~TcpClient() {

}
// 初始化网络(要么成功要么失败)
bool TcpClient::initNet() {
	// 1.加载库
	WORD version = MAKEWORD(2, 2);
	WSADATA data;
	int err = WSAStartup(version, &data);// 如果成功，则WSAStartup函数将返回0
	if (0 != err) {
		cout << "加载库失败" << endl;
		return false;
	}
	if (2 != HIBYTE(data.wVersion) || 2 != LOBYTE(data.wVersion)) {
		cout << "版本号错误" << endl;
		return false;
	}else {
		cout << "加载库成功" << endl;
	}
	// 2.创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET) {
		cout << "创建套接字失败" << endl;
		return false;
	}else {
		cout << "创建套接字成功" << endl;
	}
	// 3.链接
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	// 魔鬼数字
	addrServer.sin_port = htons(_DEF_TCP_PORT);
	addrServer.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
	err = connect(m_sock, (sockaddr*)&addrServer, sizeof(addrServer));// 连接成功，返回零。
	if (0 != err) {
		cout << "连接失败" << endl;
		return false;
	}else {
		cout << "连接成功" << endl;
	}
	// 4.创建接受数据的线程(创建既要运行)
	// CreateThread和ExitThread是一对，如果线程使用了C++运行时库的函数，例如(strcpy申请空间不释放)
	// ExitThread也不会释放空间，就会造成内存泄露
	// _beginthreadex和_endthreadex是一对，_endthreadex会先回收空间，再调用ExitThread
	m_handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, this, 0, nullptr);
	return true;
}

// 线程函数，定义成为类的成员函数
// 静态成员函数不能调用非静态成员函数，还是需要运用对象 this 对象
unsigned __stdcall TcpClient::recvThread(void* lpVoid) {
	TcpClient* pThis = (TcpClient*)lpVoid;
	pThis->recvData();
	return 1;
}

// 发送数据 UDP是由于IP地址来决定发给谁，TCP是由socket来决定发给谁
// tcp协议有着4种解决粘包问题，加标志位，固定包大小，短链接，先发包大小再发包内容
// 加标志位为什么不好用？因为这里由用户输入的内容，当输入的内容和标志位重复之后，也不好用了。
bool TcpClient::sendData(char* data, int len, long to) {
	// 1.校验参数
	if (!data || len <= 0) {
		return false;
	}
	// 2.先发包大小
	if (send(m_sock, (char*)&len, sizeof(int), 0) <= 0) {
		cout << "sendData error1:" << WSAGetLastError() << endl;
		return false;
	}
	// 3.再发包内容
	if (send(m_sock, data, len, 0) <= 0) {
		cout << "sendData error2:" << WSAGetLastError() << endl;
		return false;
	}
	return true;
}
// 接受数据，先接受包大小
void TcpClient::recvData() {
	int nRecvNum = 0;
	int packSize = 0;
	int nOffset = 0;// 偏移量，记录一个包接收到多少数据
	while (m_bRunning) {
		// 接受包大小
		// 返回读出来的字节大小。如果连接已正常关闭，则返回值为零
		nRecvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {
			// 接受数据成功，知道包大小了
			char* recvBuf = new char[packSize];
			// 接受包内容
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
			// 接受包内容成功，把数据传给中介者
			m_pMediator->transmitData(recvBuf, nOffset, m_sock);
			// 偏移量清零
			nOffset = 0;
		}else {
			cout << "recvData error1:" << WSAGetLastError() << endl;
			break;
		}
	}
}
// 关闭网络
void TcpClient::unInitNet() {
	// 1.结束线程
	m_bRunning = false;
	if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 1000)) {
		TerminateThread(m_handle, -1);
	}
	CloseHandle(m_handle);
	m_handle = nullptr;
	// 2.关闭套接字
	if (!m_sock && m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
	// 3.卸载库
	WSACleanup();
}
