#include "UdpMediator.h"

UdpMediator::UdpMediator() {

}
UdpMediator::~UdpMediator() {

}

// ������
bool UdpMediator::openNet() {
	return true;
}
// ��������
bool UdpMediator::sendData(char* data, int len, long to) {
	return true;
}
// ת�����ݣ��ѽ��յ�������ת����kernel��
void UdpMediator::transmitData(char* data, int len, long from) {

}
// �ر�����
void UdpMediator::closeNet() {

}