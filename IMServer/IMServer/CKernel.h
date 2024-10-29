#pragma once
#include "mediator/mediator.h"
#include <iostream>
#include "MySQL/CMySql.h"
#include "net/def.h"
#include <map>
using namespace std;

// ��������ָ��
class CKernel;
// ����һ��ָ����ĳ�Ա�����ĺ���ָ��PFUN�����������������涨��ġ�
typedef void (CKernel::*PFUN)(char* data, int len, long from);

class CKernel{
public:
	//��ָ̬��
	static CKernel* pKernel;
	CKernel();
	~CKernel();
	// �򿪷�����
	bool startServer();
	// �رշ�����
	void closeServer();
	// �������н��յ�������
	void dealData(char* data, int len, long from);
	// ��Э��ͷ
	void setProtocolMap();
	// ����û��ĺ�����Ϣ�������Լ�)
	void getUserInfo(int userId);
	// �����û�id��ѯ�û���Ϣ
	void getInfoById(int id, _STRU_FRIEND_INFO* info);
	// ����ע������
	void dealRegisterRq(char* data, int len, long from);
	// �����¼����
	void dealLoginRq(char* data, int len, long from);
	
	// ������������
	void dealChatRq(char* data, int len, long from);

	// ����ͻ�����������
	void dealClientoffline(char* data, int len, long from);

	// ������Ӻ�������
	void dealAddFriendRq(char* data, int len, long from);

	// ������Ӻ��ѻظ�
	void dealAddFriendRs(char* data, int len, long from);
private:
	mediator* m_pMediator;// �н���ָ��
	
	// ����ָ������
	PFUN m_mapProtocol[_DEF_PROTOCOL_COUNT];
	// ���ݿ����
	CMySql m_mysql;
	// ����socket��map
	map<int, SOCKET> m_mapIdToSocket;
};

