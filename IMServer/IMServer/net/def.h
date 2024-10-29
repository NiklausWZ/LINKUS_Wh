#pragma once
#include<string.h>

#define _DEF_TCP_PORT        (67890)
#define _DEF_TCP_SERVER_IP   ("10.61.71.85")
#define _DEF_LENTH           (100)
#define _DEF_COTENT_LENGTH   (4096)
#define _DEF_PROTOCOL_COUNT  (10)

// ����Э��ͷ
#define _DEF_PROTOCOL_BASE   (1000)

// ע������
#define _DEF_REGISTER_RQ      (_DEF_PROTOCOL_BASE + 1)
// ע��ظ�
#define _DEF_REGISTER_RS      (_DEF_PROTOCOL_BASE + 2)
//��¼����
#define _DEF_LOGIN_RQ         (_DEF_PROTOCOL_BASE + 3)
//��¼�ظ�
#define _DEF_LOGIN_RS         (_DEF_PROTOCOL_BASE + 4)
//��ȡ�����б�
#define _DEF_FRIEND_INFO      (_DEF_PROTOCOL_BASE + 5)
//��������
#define _DEF_CHAT_RQ          (_DEF_PROTOCOL_BASE + 6)
//����ظ�
#define _DEF_CHAT_RS          (_DEF_PROTOCOL_BASE + 7)
// ��Ӻ�������
#define _DEF_ADD_FRIEND_RQ    (_DEF_PROTOCOL_BASE + 8)
// ��Ӻ��ѻظ�
#define _DEF_ADD_FRIEND_RS    (_DEF_PROTOCOL_BASE + 9)
// ��������
#define _DEF_OFFLINE_RQ       (_DEF_PROTOCOL_BASE + 10)

// ����Э��ͷ����
typedef int PackType;

// �������
// ע����
#define _register_success      (0)
#define _register_tel_exists   (1)
#define _register_name_exists  (2)
// ��¼���
#define _login_success         (0)
#define _login_tel_not_exists  (1)
#define _login_password_error  (2)

// �û�״̬
#define _status_online     (0)
#define _status_offline    (1)
//���ͽ��
#define _send_success	   (0)
#define _send_fail         (1)
//��Ӻ��ѽ��
#define _add_friend_success     (0)
#define _add_friend_not_exists  (1)
#define _add_friend_offline     (2)
#define _add_friend_refuse      (3)

// ����ṹ��
// ע������Э��ͷ���绰�š��ǳơ�����
typedef struct _STRU_REGISTER_RQ {
	_STRU_REGISTER_RQ() :type(_DEF_REGISTER_RQ)
	{
		memset(tel, 0, _DEF_LENTH);
		memset(name, 0, _DEF_LENTH);
		memset(password, 0, _DEF_LENTH);
	}
	PackType type;
	char tel[_DEF_LENTH];
	char name[_DEF_LENTH];
	char password[_DEF_LENTH];
}STRU_REGISTER_RQ;

//ע��ظ�:Э��ͷ��ע����(ע��ɹ����绰�ѱ�ע�ᡢ�ǳ��ѱ�ע��)
typedef struct _STRU_REGISTER_RS {
	_STRU_REGISTER_RS() :type(_DEF_REGISTER_RS), result(_register_name_exists)
	{
	}
	PackType type;
	int result;
}_STRU_REGISTER_RS;
// ��¼����:Э��ͷ���绰���롢���� 
typedef struct _STRU_LOGIN_RQ {
	_STRU_LOGIN_RQ() : type(_DEF_LOGIN_RQ)
	{
		memset(tel, 0, _DEF_LENTH);
		memset(password, 0, _DEF_LENTH);
	}
	PackType type;
	char tel[_DEF_LENTH];
	char password[_DEF_LENTH];
}STRU_LOGIN_RQ;
// ��¼�ظ�:Э��ͷ����¼���(¼�ɹ���������󡢵绰������) 
typedef struct _STRU_LOGIN_RS {
	_STRU_LOGIN_RS() :type(_DEF_LOGIN_RS), result(_login_password_error),userid(0)
	{
	}
	PackType type;
	int userid;
	int result;
}_STRU_LOGIN_RS;
//��ȡ�����б�:Э��ͷ���û�id���ǳơ�ǩ����ͷ��id��״̬
typedef struct _STRU_FRIEND_INFO {
	_STRU_FRIEND_INFO() :type(_DEF_FRIEND_INFO), status(_status_offline), iconId(0), usedId(0)
	{
		memset(name, 0, _DEF_LENTH);
		memset(feeling, 0, _DEF_LENTH);
	}
	PackType type;
	int usedId;
	char name[_DEF_LENTH];
	char feeling[_DEF_LENTH];
	int iconId; int status;
}_STRU_FRIEND_INFO;
//��������: Э��ͷ���Լ���id���������ݡ����ѵ�id 
typedef struct _STRU_CHAT_RQ {
	_STRU_CHAT_RQ() :type(_DEF_CHAT_RQ), usedId(0), friendId(0)
	{
		memset(content, 0, _DEF_COTENT_LENGTH);
	}
	PackType type;
	int usedId;
	int friendId;
	char content[_DEF_COTENT_LENGTH];
}_STRU_CHAT_RQ;
// ����ظ�: Э��ͷ�����ͽ��(�ɹ����û�������) 
typedef struct _STRU_CHAT_RS {
	_STRU_CHAT_RS() :type(_DEF_CHAT_RS), result(_send_fail), friendId(0)
	{
	}
	PackType type;
	int friendId;
	int result;
}_STRU_CHAT_RS;
//��Ӻ�������(�����س���Ӻ���):Э��ͷ�����ѵ��ǳơ��Լ���id���Լ����ǳ� 
typedef struct _STRU_ADD_FRIEND_RQ {
	_STRU_ADD_FRIEND_RQ() :type(_DEF_ADD_FRIEND_RQ), usedId(0)
	{
		memset(userName, 0, _DEF_LENTH);
		memset(friendName, 0, _DEF_LENTH);
	}
	PackType type;
	int usedId;
	char userName[_DEF_LENTH];
	char friendName[_DEF_LENTH];
}STRU_ADD_FRIEND_RQ;
// ��Ӻ��ѻظ�:Э��ͷ����ӽ��(�ɹ������Ѳ����ڡ����Ѳ����ߡ����Ѿܾ�)
typedef struct _STRU_ADD_FRIEND_RS {
	_STRU_ADD_FRIEND_RS() :type(_DEF_ADD_FRIEND_RS), result(_add_friend_refuse),aId(0),bId(0)
	{
		memset(friendName, 0, _DEF_LENTH);
	}
	PackType type;
	int result;
	int aId;
	int bId;
	char friendName[_DEF_LENTH];
}_STRU_ADD_FRIEND_RS;
// ��������:Э��ͷ���Լ���id
typedef struct _STRU_OFFLINE_RQ {
	_STRU_OFFLINE_RQ() :type(_DEF_OFFLINE_RQ), userId(0)
	{
	}
	PackType type;
	int userId;
}STRU_OFFLINE_RQ;