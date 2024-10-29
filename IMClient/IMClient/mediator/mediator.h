#pragma once
#include <QObject>
/*
* 两个头文件互相包含了，net.h中有mediator.h，同时mediator.h也include了net.h
* 先声明有mediator这个类，直接使用，一会就编译到了
*/
class net;
class mediator:public QObject{
    Q_OBJECT
public:
    // Qt里面不允许在头文件中定义函数实现，也就是没有{}
    mediator();
    virtual ~mediator();

	// 打开网络
	virtual bool openNet() = 0;
	// 发送数据
	virtual bool sendData(char* data, int len, long to) = 0;
	// 转发数据（把接收到的数据转发给kernel）
	virtual void transmitData(char* data, int len, long from) = 0;
	// 关闭网络
	virtual void closeNet() = 0;

signals:
    void sig_transmitData(char* data, int len, long from);
protected:
	net* m_pNet;
};
