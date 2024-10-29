#include "Udp.h"

Udp::Udp() {

}
Udp::~Udp() {

}
// 初始化网络(要么成功要么失败)
bool Udp::initNet() {
	return true;

}
// 发送数据 UDP是由于IP地址来决定发给谁，TCP是由socket来决定发给谁
bool Udp::sendData(char* data, int len, long to) {
	return true;

}
// 接受数据
void Udp::recvData() {

}
// 关闭网络
void Udp::unInitNet() {

}
