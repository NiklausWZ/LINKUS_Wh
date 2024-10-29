#include "TcpClientMediator.h"
#include "../net/TcpClient.h"

TcpClientMediator::TcpClientMediator() {
	m_pNet = new TcpClient(this);
}
TcpClientMediator::~TcpClientMediator() {
	if (m_pNet) {
		m_pNet->unInitNet();
		delete m_pNet;
		m_pNet = nullptr;
	}
}

// 打开网络
bool TcpClientMediator::openNet() {
	// 调用TcpClient的init()函数
	return m_pNet->initNet();
}
// 发送数据
bool TcpClientMediator::sendData(char* data, int len, long to) {
	return m_pNet->sendData(data, len, to);
}
// 转发数据（把接收到的数据转发给kernel）
void TcpClientMediator::transmitData(char* data, int len, long from) {
	cout << "TcpClientMediator::transmitData：" << data << endl;
}
// 关闭网络
void TcpClientMediator::closeNet() {
	m_pNet->unInitNet();
}