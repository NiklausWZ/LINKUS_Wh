#include "TcpServer.h"
#include "def.h"
#include "../mediator/TcpServerMediator.h"
#include <process.h>

/*
* 堆栈溢出的原因
* A new B 的对象，B new A的对象，两个构造函数之间不停地new对象，等对方，先有鸡先后蛋。 
* 如何解决？要发现main()函数中已经有一个TcpServerMediator的对象了。
* 在main()函数里，new了一个TcpServerMediator，在构造函数里，以及有一个this指针指向对象了
*/

TcpServer::TcpServer(mediator* pNediator) :m_bRunning(true) {
	m_pMediator = pNediator;
}
TcpServer::~TcpServer() {

}
// 初始化网络(要么成功要么失败)
// 加载库、创建套接字、绑定、监听、创建接受连接的线程
bool TcpServer::initNet() {
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
	}
	else {
		cout << "加载库成功" << endl;
	}
	// 2.创建套接字
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_sock) {
		cout << "创建套接字失败" << endl;
		return false;
	}else {
		cout << "创建套接字成功" << endl;
	}
	// 3.链接
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(_DEF_TCP_PORT);
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
	err = bind(m_sock, (sockaddr*)&addrServer, sizeof(addrServer));
	if (SOCKET_ERROR == err) {
		cout << "连接失败" << endl;
		return false;
	}
	else {
		cout << "连接成功" << endl;
	}
	// 4.监听
	err = listen(m_sock, 10);
	if (SOCKET_ERROR == err) {
		cout << "监听失败:" << WSAGetLastError() << endl;
		return false;
	}
	else {
		cout << "监听成功" << endl;
	}

	// 5.创建接受连接的线程
	HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &acceptThread, this, 0, nullptr);
	if (handle) {
		m_listHandle.push_back(handle);
	}
	return true;
}

// 创建接受连接的线程函数
unsigned _stdcall TcpServer::acceptThread(void* lpVoid) {
	// 服务端一直要连接
	TcpServer* pThis = (TcpServer*)lpVoid;
	sockaddr_in addrClient;
	int addrClientSize = sizeof(addrClient);
	unsigned int threadId = 0;
	while (pThis->m_bRunning) {
		SOCKET sockTalk = accept(pThis->m_sock, (sockaddr*)&addrClient, &addrClientSize);
		if (INVALID_SOCKET == sockTalk) {
			// 连接失败
			cout << "accept error" << WSAGetLastError() << endl;
		}
		else {
			// 连接成功，打印客户端的ip
			cout << "accept success,client ip:" << inet_ntoa(addrClient.sin_addr) << endl;
			// 创建接受数据的线程，server一共n+1个线程句柄
			HANDLE handle = (HANDLE)_beginthreadex(nullptr, 0, &recvThread, pThis, 0, &threadId);
			if (handle) {
				pThis->m_listHandle.push_back(handle);
			}
			// 保存socket，线程和socket是一一对应的关系
			pThis->m_mapThreadIdToSocket[threadId] = sockTalk;
		}
	}
	return 1;
}
// 接受数据的线程函数
unsigned __stdcall TcpServer::recvThread(void* lpVoid) {
	TcpServer* pThis = (TcpServer*)lpVoid;
	pThis->recvData();
	return 1;
}
// 发送数据，服务端必须用to参数决定
// 客户端发送注意，只跟服务端发送数据，所以send用连接时候的sock就行了
bool TcpServer::sendData(char* data, int len, long to) {
	// 1.校验参数
	if (!data || len <= 0) {
		return false;
	}
	// 2.先发包大小
	if (send(to, (char*)&len, sizeof(int), 0) <= 0) {
		cout << "sendData error1:" << WSAGetLastError() << endl;
		return false;
	}
	// 3.再发包内容
	if (send(to, data, len, 0) <= 0) {
		cout << "sendData error2:" << WSAGetLastError() << endl;
		return false;
	}

	return true;
}
// 接受数据
void TcpServer::recvData() {
	// 休眠一会(防止map中还没有保存socket)
	Sleep(10);
	// 从map中取出来当前线程对应的socket
	SOCKET sock = m_mapThreadIdToSocket[GetCurrentThreadId()];
	if (!sock || INVALID_SOCKET == sock) {
		cout << "recvData socket error" << endl;
		return;
	}
	int nRecvNum = 0;
	int packSize = 0;
	int nOffset = 0;// 偏移量，记录一个包接收到多少数据
	while (m_bRunning) {
		// 接受包大小
		// 返回读出来的字节大小。如果连接已正常关闭，则返回值为零
		nRecvNum = recv(sock, (char*)&packSize, sizeof(int), 0);
		if (nRecvNum > 0) {
			// 接受数据成功，知道包大小了
			char* recvBuf = new char[packSize];
			// 接受包内容
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
			// 接受包内容成功，把数据传给中介者
			m_pMediator->transmitData(recvBuf, nOffset, sock);
	
			// 偏移量清零
			nOffset = 0;
		}
		else {
			cout << "recvData error1:" << WSAGetLastError() << endl;
			break;
		}
	}
}
// 关闭网络
void TcpServer::unInitNet() {
	// 1.结束线程
	m_bRunning = false;
	for (auto ite = m_listHandle.begin(); ite != m_listHandle.end();) {
		HANDLE handle = *ite;
		TerminateThread(handle, -1);// 关闭handle
		if (WAIT_TIMEOUT == WaitForSingleObject(handle, 1000)) {
			TerminateThread(handle, -1);
		}
		CloseHandle(handle);
		handle = nullptr;
		// 从list中移除已经关闭的handle并更新迭代器
		ite = m_listHandle.erase(ite);
	}

	// 2.关闭套接字
	if (!m_sock && m_sock != INVALID_SOCKET) {
		closesocket(m_sock);
	}
	for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
		SOCKET sock = ite->second;
		if (!sock && sock != INVALID_SOCKET) {
			closesocket(sock);
		}
		// 从map中移除无效节点
		ite = m_mapThreadIdToSocket.erase(ite);
	}
	// 3.卸载库
	WSACleanup();
}
