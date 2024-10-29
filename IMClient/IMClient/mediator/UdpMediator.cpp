#include "UdpMediator.h"

UdpMediator::UdpMediator() {

}
UdpMediator::~UdpMediator() {

}

// 打开网络
bool UdpMediator::openNet() {
	return true;
}
// 发送数据
bool UdpMediator::sendData(char* data, int len, long to) {
	return true;
}
// 转发数据（把接收到的数据转发给kernel）
void UdpMediator::transmitData(char* data, int len, long from) {

}
// 关闭网络
void UdpMediator::closeNet() {

}