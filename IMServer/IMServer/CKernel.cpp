#include "CKernel.h"
#include "mediator/TcpServerMediator.h"
#include "list"

// ��ָ̬��ĳ�ʼ��
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
// ��Э��ͷ
void CKernel::setProtocolMap()
{
	cout << __func__ << endl;
	// ��ʼ������
	memset(m_mapProtocol, 0, sizeof(m_mapProtocol));
	// �����鸳ֵ
	m_mapProtocol[_DEF_REGISTER_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealRegisterRq;
	m_mapProtocol[_DEF_LOGIN_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealLoginRq;
	m_mapProtocol[_DEF_CHAT_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRq;
	m_mapProtocol[_DEF_OFFLINE_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealClientoffline;
	m_mapProtocol[_DEF_ADD_FRIEND_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRq;
	m_mapProtocol[_DEF_ADD_FRIEND_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRs;
}

bool CKernel::startServer() {
	// 1��������
	if (!m_pMediator->openNet()) {
		cout << "������ʧ��" << endl;
		return false;
	}
	// 2���������ݿ�
	if (!m_mysql.ConnectMySql((char*)"127.0.0.1", (char*)"root", (char*)"password", (char*)"improject")) {
		cout << "�������ݿ�ʧ��" << endl;
		return false;
	}
	return true;
}

// �رշ�����
void CKernel::closeServer() {
	// 1���ر�����
	if (m_pMediator) {
		m_pMediator->closeNet();
		delete m_pMediator;
		m_pMediator = nullptr;
	}
	// 2���Ͽ����ݿ�����

}

// �������н��ܵ�������
void CKernel::dealData(char* data, int len, long from)
{
	cout << __func__ << endl;
	// ȡ��Э��ͷ
	PackType type = *(PackType*)data;
	// ����������±�
	int index = type - _DEF_PROTOCOL_BASE - 1;
	// �ж������±��Ƿ�����Ч��Χ��
	if (0 <= index && index < _DEF_PROTOCOL_COUNT) {
		// ȡ������ָ��
		PFUN pf = m_mapProtocol[index];
		if (pf) {
			//���ú���
			(this->*pf)(data, len, from);
		}
		else {
			// �����ӡ����־������ԭ��1��û�а�Э��ͷ�����飻2���ṹ��Э��ͷ��ֵ����
			cout << "type2:" << type << endl;
		}
	}
	else {
		// �����ӡ����־������ԭ��1���ṹ��Э��ͷ��ֵ����2���������������⣬����offsetû����
		cout << "type1:" << type << endl;
	}
}

// ����ע������
void CKernel::dealRegisterRq(char* data, int len, long from) {
	cout << __func__ << endl;
	cout << from << endl;
	// 1.���
	_STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;

	// 2.У�����ݺϷ���

	// 3�����ݵ绰��������ݿ��ѯ�绰����
	list<string> lstRes; // 
	char szSql[1024] = "";
	// SQL����ƴ��
	sprintf(szSql, "select tel from t_user where tel = '%s';", rq->tel);
	
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}
	// 4���жϽ����ѯ����Ƿ�Ϊ��
	_STRU_REGISTER_RS rs;
	if (0 != lstRes.size()) {
		// ��ѯ����˵���绰�����Ѿ���ע�ᣬע��ʧ��
		rs.result = _register_tel_exists;
	}
	else {
		// 5�������ǳƲ�ѯ�ǳ�
		sprintf(szSql, "select name from t_user where name = '%s';", rq->name);
		if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
			cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
			return;
		}
		// 6���жϽ����ѯ����Ƿ�Ϊ��
		if (0 != lstRes.size()) {
			// ��ѯ����˵���ǳ��Ѿ���ע�ᣬע��ʧ��
			rs.result = _register_name_exists;
		}
		else {
			// 7��û�в�ѯ������ע����Ϣд�����ݿ�
			sprintf(szSql, "insert into t_user (name, tel, password, iconid, feeling) values ('%s','%s','%s',6,'ѧϰ����ѽ');",
				rq->name, rq->tel, rq->password);
			if (!m_mysql.UpdateMySql(szSql)) {
				cout << "�������ݿ�ʧ�ܣ�" << szSql << endl;
				return;
			}
			// 8��ע��ɹ�
			rs.result = _register_success;
		}
	}
	// 9�������Ƿ�ɹ������ѽ�����ظ��ͻ���
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	// cout << "name:" << rq->name << "��tel:" << rq->tel << "��pass:" << rq->password << endl;
}

// �����¼����
void CKernel::dealLoginRq(char* data, int len, long from) {
	cout << __func__ << endl;
	// 1�����
	_STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;

	// 2�����ݵ绰��������ݿ��ѯ����
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select id, password from t_user where tel = '%s';", rq->tel);
	
	if (!m_mysql.SelectMySql(szSql, 2, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}
	// 3���жϽ���Ƿ�Ϊ��
	_STRU_LOGIN_RS rs;
	if (0 == lstRes.size()) {
		// ������Ϊ�գ�˵���绰����û��ע�ᣬ��¼ʧ��
		rs.result = _login_tel_not_exists;
	}
	else {
		for (auto it = lstRes.begin(); it != lstRes.end(); ++it) {
			cout << *it << endl;  // ͨ������������ÿ��Ԫ��
		}

		// ȡ���û�id���ǵ�Ҫ��sql��䱣֤˳��
		int userId = stoi(lstRes.front());
		lstRes.pop_front();

		// ȡ����ѯ�������루ȡ������ݴ��б���ɾ����
		string pass = lstRes.front();// list��ôȡֵ?
		lstRes.pop_front();

		// 4�������Ϊ�գ��Ƚϲ�ѯ����������û����������
		if (0 != strcmp(rq->password, pass.c_str())) { // �ַ����Ƚ���ʲô��
			// ���벻��ȣ���½ʧ��
			rs.result = _login_password_error;
		}
		else {
			// 5��������ȣ���¼�ɹ�
			rs.result = _login_success;
			rs.userid = userId;

			// ���浱ǰ��¼�û���socket
			m_mapIdToSocket[userId] = from;

			// 6�����۵�¼�Ƿ�ɹ�����Ҫ���ͻ��˻ظ���¼���
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);

			// ��ȡ��ǰ��¼�û��ĺ����б�
			getUserInfo(userId);
			return;
		}
	}
	// 6�����۵�¼�Ƿ�ɹ�����Ҫ���ͻ��˻ظ���¼���
	m_pMediator->sendData((char*)&rs, sizeof(rs), from);
}

/*
* ���緢�����ݣ����͵��ǿ��������ݣ����ǵ�ַ��
*/

// ��ȡ�û��ĺ�����Ϣ(�����Լ���)
void CKernel::getUserInfo(int userId) {
	cout << __func__ << endl;
	// �����ݿ��ѯ�Լ�����Ϣ
	_STRU_FRIEND_INFO userInfo; // ��һ����һ��

	// ����������ȶ���һ��������Ȼ��������ַ�ı�������ȥ��Ȼ���ں����н��в�����ֵ
	getInfoById(userId, &userInfo);

	// ���Լ�����Ϣ�����ͻ���
	if (m_mapIdToSocket.find(userId) != m_mapIdToSocket.end()) {
		m_pMediator->sendData((char*)&userInfo,sizeof(userInfo),m_mapIdToSocket[userId]);
	}
	// A -> <- ������ -> <- B

	// �Ӻ��ѹ�ϵ���в�ѯ���ѵ�id�б�
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select idB from t_friend where idA = '%d';", userId);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}

	// ��������id�б�
	int friendId = 0;
	_STRU_FRIEND_INFO friendInfo;
	while (lstRes.size() > 0) {
		// ȡ������id
		friendId = stoi(lstRes.front());
		lstRes.pop_front();
		// ���ݺ���id��ѯ������Ϣ
		getInfoById(friendId, &friendInfo);
		// �Ѻ��ѵ���Ϣ�����ͻ���
		if (m_mapIdToSocket.find(userId) != m_mapIdToSocket.end()) {
			m_pMediator->sendData((char*)&friendInfo, sizeof(friendInfo), m_mapIdToSocket[userId]);
		}

		// ֪ͨ�������ߺ��ѣ��Լ�������
		if (m_mapIdToSocket.count(friendId) > 0)
		{
			m_pMediator->sendData((char*)&userInfo, sizeof(userInfo), m_mapIdToSocket[friendId]);
		}
	}
}

// �����û�id��ѯ�û���Ϣ
void CKernel::getInfoById(int id, _STRU_FRIEND_INFO* info) {
	cout << __func__ << endl;
	info->usedId = id;
	if (m_mapIdToSocket.count(id) > 0) {
		// �û�����
		info->status = _status_online;
	}
	else {
		// �û�������
		info->status = _status_offline;
	}
	// ����id�����ݿ��ѯ�û����ǳơ�ǩ����ͷ��id
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select name,feeling,iconid from t_user where id = '%d';", id);
	if (!m_mysql.SelectMySql(szSql, 3, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}
	if (3 == lstRes.size()) {
		// ȡ���ǳ�
		strcpy(info->name, lstRes.front().c_str());
		lstRes.pop_front();
		// ȡ��ǩ��
		strcpy(info->feeling, lstRes.front().c_str());
		lstRes.pop_front();
		// ȡ��ͷ��id
		info->iconId = stoi(lstRes.front());
		lstRes.pop_front();
	}
}

// ������������
void CKernel::dealChatRq(char* data, int len, long from) {

	cout << __func__ << endl;

	// 1.���
	_STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;
	
	// 2.�жϺ����Ƿ�����
	if (m_mapIdToSocket.count(rq->friendId) > 0) {
		// �������ߣ�����������ת��������
		m_pMediator->sendData(data, len, m_mapIdToSocket[rq->friendId]);
	}
	else {
		// ���Ѳ����ߣ����������ݴ浽���ݿ���Ⱥ����������ٷ�����
		// ���Ѳ����ߣ��ظ� ������ʧ�ܡ�
		_STRU_CHAT_RS rs;
		rs.result = _send_fail;
		rs.friendId = rq->friendId;
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
}

// ������������
void CKernel::dealClientoffline(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.���
	_STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;
	// 2.�����û�id��ѯ���к��ѵ�id�б�
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql,"select idB from t_friend where idA = '%d';",rq->userId);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}
	// 3.������ѯ���
	int friendId = 0;
	while (lstRes.size() > 0) {
		// 4.ȡ��ÿ�����ѵ�id
		friendId = stoi(lstRes.front());
		lstRes.pop_front();
	}
	// 5.�жϺ����Ƿ�����
	if (m_mapIdToSocket.count(friendId > 0)) {
		// 6.����������ߣ�������ת����������
		m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
	}
	// 7.�ҵ������û���socket
	if (m_mapIdToSocket.count(rq->userId) > 0) {
		// �ر�socket
		closesocket(m_mapIdToSocket[rq->userId]);

		// ��map���Ƴ���Ч�ڵ�
		m_mapIdToSocket.erase(rq->userId);
	}
}

// ������Ӻ�������
void CKernel::dealAddFriendRq(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.���
	_STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

	// 2.�����ǳƲ�ѯ�û�id
	list<string> lstRes;
	char szSql[1024] = "";
	sprintf(szSql, "select id from t_user where name = '%s';", rq->friendName);
	if (!m_mysql.SelectMySql(szSql, 1, lstRes)) {
		cout << "��ѯ���ݿ�ʧ�ܣ�" << szSql << endl;
		return;
	}

	// 3.�жϽ���Ƿ�Ϊ��
	_STRU_ADD_FRIEND_RS rs;
	if (0 == lstRes.size()) {
		// 4.���Ϊ�գ�˵���û������ڣ����ʧ��
		rs.result = _add_friend_not_exists;
		strcpy_s(rs.friendName, rq->friendName);
		m_pMediator->sendData((char*)&rs, sizeof(rs), from);
	}
	else {
		// ȡ���û�id
		int friendId = stoi(lstRes.front());
		lstRes.pop_front();
		// 5.�����Ϊ�գ��ж��û��Ƿ�����
		if (m_mapIdToSocket.count(friendId)> 0) {
			// 6.�û����ߣ����û�ת����Ӻ�������
			m_pMediator->sendData(data, len, m_mapIdToSocket[friendId]);
		}
		else {
			// 7.�û������ߣ���Ӻ���ʧ��
			rs.result = _add_friend_offline;
			strcpy_s(rs.friendName, rq->friendName);
			// �ѽ�������ͻ���
			m_pMediator->sendData((char*)&rs, sizeof(rs), from);
		}
	}
}

// ������Ӻ��ѻظ�
void CKernel::dealAddFriendRs(char* data, int len, long from) {
	cout << __func__ << endl;

	// 1.���
	_STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;

	// 2.�ж���ӽ�����Ƿ�ͬ��
	if (_add_friend_success == rs->result) {
		// 3.�Ѻ��ѹ�ϵд�����ݿ�
		list<string> lstRes;
		char szSql[1024] = "";
		sprintf(szSql, "insert into t_friend values ('%d','%d');",rs->aId,rs->bId);
		if (!m_mysql.UpdateMySql(szSql)) {
			cout << "�������ݿ�ʧ�ܣ�" << szSql << endl;
			return;
		}

		sprintf(szSql, "insert into t_friend values ('%d','%d');", rs->bId, rs->aId);
		if (!m_mysql.UpdateMySql(szSql)) {
			cout << "�������ݿ�ʧ�ܣ�" << szSql << endl;
			return;
		}
		// 4.���º����б�
		getUserInfo(rs->aId);
	}
	// 5.���ܽ����Σ���Ҫ��A�ͻ���
	if (m_mapIdToSocket.count(rs->aId) > 0) {
		m_pMediator->sendData(data, len, m_mapIdToSocket[rs->aId]);
	}
	
}