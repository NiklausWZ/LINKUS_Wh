#include "CKernel.h"
#include "mediator/TcpServerMediator.h"
#include "list"

// 静态指针的初始化
CKernel* CKernel::pKernel = nullptr;
CKernel::CKernel(){
	pKernel = this;
	m_pMediator = new TcpServerMediator();
	setProtocolMap();
}

CKernel::~CKernel() {
	if (m_pMediator) {
		m_pMediator->closeNet();
		delete m_pMediator;
		m_pMediator = nullptr;
	}
}
// 绑定协议头
void CKernel::setProtocolMap()
{
	cout << __func__ << endl;
	// 初始化数组
	memset(m_mapProtocol, 0, sizeof(m_mapProtocol));
	// 给数组赋值
	m_mapProtocol[_DEF_REGISTER_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealRegisterRq;
	m_mapProtocol[_DEF_LOGIN_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealLoginRq;
	m_mapProtocol[_DEF_CHAT_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRq;
	m_mapProtocol[_DEF_OFFLINE_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealClientoffline;
	m_mapProtocol[_DEF_ADD_FRIEND_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRq;
	m_mapProtocol[_DEF_ADD_FRIEND_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRs;
}

bool CKernel::startServer() {
	// 1、打开网络
	if (!m_pMediator->openNet()) {
		cout << "打开网络失败" << endl;
		return false;
	}
	// 2、连接数据库
	if (!m_mysql.ConnectMySql((char*)"127.0.0.1", (char*)"root", (char*)"password", (char*)"improject")) {
		cout << "连接数据库失败" << endl;
		return false;
	}
	return true;
}

// 关闭服务器
void CKernel::closeServer() {
	// 1、关闭网络
	if (m_pMediator) {
		m_pMediator->closeNet();
		delete m_pMediator;
		m_pMediator = nullptr;
	}
	// 2、断开数据库连接

}

// 处理所有接受到的数据
void CKernel::dealData(char* data, int len, long from)
{
	cout << __func__ << endl;
	// 取出协议头
	PackType type = *(PackType*)data;
	// 计算出数组下标
	int index = type - _DEF_PROTOCOL_BASE - 1;
	// 判断数组下标是否在有效范围内
	if (0 <= index && index < _DEF_PROTOCOL_COUNT) {
		// 取出函数指针
		PFUN pf = m_mapProtocol[index];
		if (pf) {
			//调用函数
			(this->*pf)(data, len, from);
		}
		else {
			// 如果打印出日志，两种原因：1、没有绑定协议头和数组；2、结构体协议头赋值错误；
			cout << "type2:" << type << endl;
		}
	}
	else {
		// 如果打印出日志，两种原因：1、结构体协议头赋值错误；2、接受数据有问题，可能offset没清零
		cout << "type1:" << type << endl;
	}
}

// 处理注册请求
void CKernel::dealRegisterRq(char* data, int len, long from) {
	cout << __func__ << endl;
	cout << from << endl;
	// 1.拆包
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;

	// 2.校验数据合法性

	// 3、根据电话号码从数据库查询电话号码
	list<string> lstRes; // 
	char szSql[1024] = "";
	// SQL语句的拼接
	sprintf(szSql, "select tel from t_user where tel = '%s';", rq->tel);
	
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}
	// 4、判断结果查询结果是否为空
	_STRU_REGISTER_RS rs;
	if (0 != lstRes.size()) {
		// 查询到，说明电话号码已经被注册，注册失败
		rs.result = _register_tel_exists;
	}
	else {
		// 5、根据昵称查询昵称
		sprintf(szSql, "select name from t_user where name = '%s';", rq->name);
		if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
			cout << "查询数据库失败：" << szSql << endl;
			return;
		}
		// 6、判断结果查询结果是否为空
		if (0 != lstRes.size()) {
			// 查询到，说明昵称已经被注册，注册失败
			rs.result = _register_name_exists;
		}
		else {
			// 7、没有查询到，把注册信息写入数据库
			sprintf(szSql, "insert into t_user (name, tel, password, iconid, feeling) values ('%s','%s','%s',6,'学习好累呀');",
				rq->name, rq->tel, rq->password);
			if (!m_mysql.UpdateMySql(szSql)) {
				cout << "更新数据库失败：" << szSql << endl;
				return;
			}
			// 8、注册成功
			rs.result = _register_success;
		}
	}
	// 9、不管是否成功，都把结果发回给客户端
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	// cout << "name:" << rq->name << "，tel:" << rq->tel << "，pass:" << rq->password << endl;
}

// 处理登录请求
void CKernel::dealLoginRq(char* data, int len, long from) {
	cout << __func__ << endl;
	// 1、拆包
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;

	// 2、根据电话号码从数据库查询密码
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select id, password from t_user where tel = '%s';", rq->tel);
	
	if (!m_mysql.SelectMySql(szSql, 2, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}
	// 3、判断结果是否为空
	_STRU_LOGIN_RS rs;
	if (0 == lstRes.size()) {
		// 如果结果为空，说明电话号码没有注册，登录失败
		rs.result = _login_tel_not_exists;
	}
	else {
		for (auto it = lstRes.begin(); it != lstRes.end(); ++it) {
			cout << *it << endl;  // 通过迭代器访问每个元素
		}

		// 取出用户id，记得要和sql语句保证顺序
		int userId = stoi(lstRes.front());
		lstRes.pop_front();

		// 取出查询到的密码（取完的数据从列表中删除）
		string pass = lstRes.front();// list怎么取值?
		lstRes.pop_front();

		// 4、结果不为空，比较查询到的密码和用户输入的密码
		if (0 != strcmp(rq->password, pass.c_str())) { // 字符串比较用什么？
			// 密码不相等，登陆失败
			rs.result = _login_password_error;
		}
		else {
			// 5、密码相等，登录成功
			rs.result = _login_success;
			rs.userid = userId;

			// 保存当前登录用户的socket
			m_mapIdToSocket[userId] = from;

			// 6、无论登录是否成功，都要给客户端回复登录结果
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);

			// 获取当前登录用户的好友列表
			getUserInfo(userId);
			return;
		}
	}
	// 6、无论登录是否成功，都要给客户端回复登录结果
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}

/*
* 网络发送数据，发送的是拷贝的内容，不是地址。
*/

// 获取用户的好友信息(包括自己的)
void CKernel::getUserInfo(int userId) {
	cout << __func__ << endl;
	// 从数据库查询自己的信息
	_STRU_FRIEND_INFO userInfo; // 查一个发一个

	// 输出参数：先定义一个变量，然后把这个地址的变量传进去，然后在函数中进行操作赋值
	getInfoById(userId, &userInfo);

	// 把自己的信息发给客户端
	if (m_mapIdToSocket.find(userId) != m_mapIdToSocket.end()) {
		m_pMediator->sendData((char*)&userInfo,sizeof(userInfo),m_mapIdToSocket[userId]);
	}
	// A -> <- 服务器 -> <- B

	// 从好友关系表中查询好友的id列表
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select idB from t_friend where idA = '%d';", userId);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}

	// 遍历好友id列表
	int friendId = 0;
	_STRU_FRIEND_INFO friendInfo;
	while (lstRes.size() > 0) {
		// 取出好友id
		friendId = stoi(lstRes.front());
		lstRes.pop_front();
		// 根据好友id查询好友信息
		getInfoById(friendId, &friendInfo);
		// 把好友的信息发给客户端
		if (m_mapIdToSocket.find(userId) != m_mapIdToSocket.end()) {
			m_pMediator->sendData((char*)&friendInfo, sizeof(friendInfo), m_mapIdToSocket[userId]);
		}

		// 通知所有在线好友，自己上线了
		if (m_mapIdToSocket.count(friendId) > 0)
		{
			m_pMediator->sendData((char*)&userInfo, sizeof(userInfo), m_mapIdToSocket[friendId]);
		}
	}
}

// 根据用户id查询用户信息
void CKernel::getInfoById(int id, _STRU_FRIEND_INFO* info) {
	cout << __func__ << endl;
	info->usedId = id;
	if (m_mapIdToSocket.count(id) > 0) {
		// 用户在线
		info->status = _status_online;
	}
	else {
		// 用户不在线
		info->status = _status_offline;
	}
	// 根据id从数据库查询用户的昵称、签名、头像id
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select name,feeling,iconid from t_user where id = '%d';", id);
	if (!m_mysql.SelectMySql(szSql, 3, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}
	if (3 == lstRes.size()) {
		// 取出昵称
		strcpy(info->name, lstRes.front().c_str());
		lstRes.pop_front();
		// 取出签名
		strcpy(info->feeling, lstRes.front().c_str());
		lstRes.pop_front();
		// 取出头像id
		info->iconId = stoi(lstRes.front());
		lstRes.pop_front();
	}
}

// 处理聊天请求
void CKernel::dealChatRq(char* data, int len, long from) {

	cout << __func__ << endl;

	// 1.拆包
	_STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;
	
	// 2.判断好友是否在线
	if (m_mapIdToSocket.count(rq->friendId) > 0) {
		// 好友在线，把聊天请求转发给好友
		m_pMediator->sendData(data, len, m_mapIdToSocket[rq->friendId]);
	}
	else {
		// 好友不在线，把聊天内容存到数据库里，等好友上线了再发给他
		// 好友不在线，回复 “发送失败”
		_STRU_CHAT_RS rs;
		rs.result = _send_fail;
		rs.friendId = rq->friendId;
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
}

// 处理下线请求
void CKernel::dealClientoffline(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.拆包
	_STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;
	// 2.根据用户id查询所有好友的id列表
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql,"select idB from t_friend where idA = '%d';",rq->userId);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}
	// 3.遍历查询结果
	int friendId = 0;
	while (lstRes.size() > 0) {
		// 4.取出每个好友的id
		friendId = stoi(lstRes.front());
		lstRes.pop_front();
	}
	// 5.判断好友是否在线
	if (m_mapIdToSocket.count(friendId > 0)) {
		// 6.如果好友在线，给好友转发下线请求
		m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
	}
	// 7.找到下线用户的socket
	if (m_mapIdToSocket.count(rq->userId) > 0) {
		// 关闭socket
		closesocket(m_mapIdToSocket[rq->userId]);

		// 从map中移除无效节点
		m_mapIdToSocket.erase(rq->userId);
	}
}

// 处理添加好友请求
void CKernel::dealAddFriendRq(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.拆包
	_STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

	// 2.根据昵称查询用户id
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select id from t_user where name = '%s';", rq->friendName);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "查询数据库失败：" << szSql << endl;
		return;
	}

	// 3.判断结果是否为空
	_STRU_ADD_FRIEND_RS rs;
	if (0 == lstRes.size()) {
		// 4.结果为空，说明用户不存在，添加失败
		rs.result = _add_friend_not_exists;
		strcpy_s(rs.friendName, rq->friendName);
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
	else {
		// 取出用户id
		int friendId = stoi(lstRes.front());
		lstRes.pop_front();
		// 5.结果不为空，判断用户是否在线
		if (m_mapIdToSocket.count(friendId)> 0) {
			// 6.用户在线，给用户转发添加好友请求
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
		else {
			// 7.用户不在线，添加好友失败
			rs.result = _add_friend_offline;
			strcpy_s(rs.friendName, rq->friendName);
			// 把结果发给客户端
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		}
	}
}

// 处理添加好友回复
void CKernel::dealAddFriendRs(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.拆包
	_STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;

	// 2.判断添加结果，是否同意
	if (_add_friend_success == rs->result) {
		// 3.把好友关系写入数据库
		list<string> lstRes;
		char szSql[1024] = "";
		sprintf(szSql, "insert into t_friend values ('%d','%d');",rs->aId,rs->bId);
		if (!m_mysql.UpdateMySql(szSql)) {
			cout << "插入数据库失败：" << szSql << endl;
			return;
		}

		sprintf(szSql, "insert into t_friend values ('%d','%d');", rs->bId, rs->aId);
		if (!m_mysql.UpdateMySql(szSql)) {
			cout << "插入数据库失败：" << szSql << endl;
			return;
		}
		// 4.更新好友列表
		getUserInfo(rs->aId);
	}
	// 5.不管结果如何，都要给A客户端
	if (m_mapIdToSocket.count(rs->aId) > 0) {
		m_pMediator->sendData(data, len, m_mapIdToSocket[rs->aId]);
	}
	
}