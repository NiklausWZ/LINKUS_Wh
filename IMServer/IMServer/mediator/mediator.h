#pragma once
/*
* ����ͷ�ļ���������ˣ�net.h����mediator.h��ͬʱmediator.hҲinclude��net.h
* ��������mediator����ֱ࣬��ʹ�ã�һ��ͱ��뵽��
*/
class net;
class mediator {
public:
	mediator() {}
	virtual ~mediator(){}

	// ������
	virtual bool openNet() = 0;
	// ��������
	virtual bool sendData(char* data, int len, long to) = 0;
	// ת�����ݣ��ѽ��յ�������ת����kernel��
	virtual void transmitData(char* data, int len, long from) = 0;
	// �ر�����
	virtual void closeNet() = 0;
protected:
	net* m_pNet;
};