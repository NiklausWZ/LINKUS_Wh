#include "TcpServerMediator.h"
#include "../net/TcpServer.h"
#include "../CKernel.h"

TcpServerMediator::TcpServerMediator() {
	m_pNet = new TcpServer(this);
}
TcpServerMediator::~TcpServerMediator() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet; 
		m_pNet = nullptr;
	}
}

// 打开网络
bool TcpServerMediator::openNet() {
	// 调用TcpServer的init()函数
	return m_pNet->initNet();
}
// 发送数据
bool TcpServerMediator::sendData(char* data, int len, long to) {
	return m_pNet->sendData(data, len, to);
}
// 转发数据（把接收到的数据转发给kernel）
void TcpServerMediator::transmitData(char* data, int len, long from) {
	cout << __func__ << endl;
	CKernel::pKernel->dealData(data, len, from);


}
// 关闭网络
void TcpServerMediator::closeNet() {
	m_pNet->unInitNet();
}