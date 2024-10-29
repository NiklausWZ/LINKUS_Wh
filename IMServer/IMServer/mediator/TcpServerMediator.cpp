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

// ������
bool TcpServerMediator::openNet() {
	// ����TcpServer��init()����
	return m_pNet->initNet();
}
// ��������
bool TcpServerMediator::sendData(char* data, int len, long to) {
	return m_pNet->sendData(data, len, to);
}
// ת�����ݣ��ѽ��յ�������ת����kernel��
void TcpServerMediator::transmitData(char* data, int len, long from) {
	cout << __func__ << endl;
	CKernel::pKernel->dealData(data, len, from);


}
// �ر�����
void TcpServerMediator::closeNet() {
	m_pNet->unInitNet();
}