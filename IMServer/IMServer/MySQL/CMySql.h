#pragma once

#include <string>
#include <iostream>
// 本地的mysql.h头文件和libmysql.lib的库文件
#include <mysql.h>

// dll要和exe放在一个目录下
#pragma comment(lib,"libmysql.lib")

#include <list>
using namespace std;

class CMySql
{
public:
    CMySql(void);
    ~CMySql(void);
public:     //ip, 用户名, 密码，数据库，端口号
    bool  ConnectMySql(char *host,char *user,char *pass,char *db,short nport = 3306);
    void  DisConnect();
    bool  SelectMySql(char* szSql, int nColumn, list<string>& lstStr);
	//获得数据库中的表
    bool GetTables(char* szSql,list<string>& lstStr);
    //更新：删除、插入、修改
    bool  UpdateMySql(char* szSql);
 
private:
    MYSQL *m_sock;   
	MYSQL_RES *m_results;   
	MYSQL_ROW m_record; 
};

