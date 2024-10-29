#pragma once
#include<string.h>

#define _DEF_TCP_PORT        (67890)
#define _DEF_TCP_SERVER_IP   ("10.61.71.85")
#define _DEF_LENTH           (100)
#define _DEF_COTENT_LENGTH   (4096)
#define _DEF_PROTOCOL_COUNT  (10)

// 定义协议头
#define _DEF_PROTOCOL_BASE   (1000)

// 注册请求
#define _DEF_REGISTER_RQ      (_DEF_PROTOCOL_BASE + 1)
// 注册回复
#define _DEF_REGISTER_RS      (_DEF_PROTOCOL_BASE + 2)
//登录请求
#define _DEF_LOGIN_RQ         (_DEF_PROTOCOL_BASE + 3)
//登录回复
#define _DEF_LOGIN_RS         (_DEF_PROTOCOL_BASE + 4)
//获取好友列表
#define _DEF_FRIEND_INFO      (_DEF_PROTOCOL_BASE + 5)
//聊天请求
#define _DEF_CHAT_RQ          (_DEF_PROTOCOL_BASE + 6)
//聊天回复
#define _DEF_CHAT_RS          (_DEF_PROTOCOL_BASE + 7)
// 添加好友请求
#define _DEF_ADD_FRIEND_RQ    (_DEF_PROTOCOL_BASE + 8)
// 添加好友回复
#define _DEF_ADD_FRIEND_RS    (_DEF_PROTOCOL_BASE + 9)
// 下线请求
#define _DEF_OFFLINE_RQ       (_DEF_PROTOCOL_BASE + 10)

// 声明协议头类型
typedef int PackType;

// 声明结果
// 注册结果
#define _register_success      (0)
#define _register_tel_exists   (1)
#define _register_name_exists  (2)
// 登录结果
#define _login_success         (0)
#define _login_tel_not_exists  (1)
#define _login_password_error  (2)

// 用户状态
#define _status_online     (0)
#define _status_offline    (1)
//发送结果
#define _send_success	   (0)
#define _send_fail         (1)
//添加好友结果
#define _add_friend_success     (0)
#define _add_friend_not_exists  (1)
#define _add_friend_offline     (2)
#define _add_friend_refuse      (3)

// 请求结构体
// 注册请求：协议头、电话号、昵称、密码
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

//注册回复:协议头、注册结果(注册成功、电话已被注册、昵称已被注册)
typedef struct _STRU_REGISTER_RS {
	_STRU_REGISTER_RS() :type(_DEF_REGISTER_RS), result(_register_name_exists)
	{
	}
	PackType type;
	int result;
}_STRU_REGISTER_RS;
// 登录请求:协议头、电话号码、密码 
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
// 登录回复:协议头、登录结果(录成功、密码错误、电话不存在) 
typedef struct _STRU_LOGIN_RS {
	_STRU_LOGIN_RS() :type(_DEF_LOGIN_RS), result(_login_password_error),userid(0)
	{
	}
	PackType type;
	int userid;
	int result;
}_STRU_LOGIN_RS;
//获取好友列表:协议头、用户id、昵称、签名、头像id、状态
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
//聊天请求: 协议头、自己的id、聊天内容、好友的id 
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
// 聊天回复: 协议头、发送结果(成功、用户不在线) 
typedef struct _STRU_CHAT_RS {
	_STRU_CHAT_RS() :type(_DEF_CHAT_RS), result(_send_fail), friendId(0)
	{
	}
	PackType type;
	int friendId;
	int result;
}_STRU_CHAT_RS;
//添加好友请求(根据呢称添加好友):协议头、好友的昵称、自己的id、自己的昵称 
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
// 添加好友回复:协议头、添加结果(成功、好友不存在、好友不在线、好友拒绝)
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
// 下线请求:协议头、自己的id
typedef struct _STRU_OFFLINE_RQ {
	_STRU_OFFLINE_RQ() :type(_DEF_OFFLINE_RQ), userId(0)
	{
	}
	PackType type;
	int userId;
}STRU_OFFLINE_RQ;