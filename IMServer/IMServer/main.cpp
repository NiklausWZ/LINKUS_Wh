#include <iostream>
#include <windows.h>
#include "CKernel.h"
using namespace std;

int main() {
	CKernel kernel;
	if (!kernel.startServer()) {
	cout << "�򿪷����ʧ��" << endl;
	return 1;
	}
	// ���Դ��룬�ȿ�������ˣ�Ȼ�����ͻ���
	//mediator* pServer = new TcpServerMediator;
	//if (!pServer->openNet()) {
	//	cout << "�򿪷����ʧ��" << endl;
	//	return 1;
	//}
	//mediator* pClient = new TcpClientMediator;
	//if (!pClient->openNet()) {
	//	cout << "�򿪿ͻ���ʧ��" << endl;
	//	return 1;
	//}
	//// �ͻ��˷�����Ϣ�������
	//char buf[] = "hello Server,I'am Client";
	//pClient->sendData(buf, sizeof(buf), 100);
	// �ó���һֱ����
	while (true)
	{
		Sleep(5000);
		cout << "Server is running" << endl;
	}
	return 0;
}

// ��Ŀ����
// IM����ϵͳ����Ϸ�У����������С���飬�������ԣ��ܹ㷺��
// ��ͨ��Э�飺TCPЭ�飬Ϊʲôѡ�����Э�飿��
// ��ģ�ͣ�C/Sģ�ͣ�Ϊʲôѡ�����ģ�ͣ���


// ����ô�жϴ�����������ݣ��Ǹ����ע���¼�������Ӧ���أ���
// ���Լ�������һ��Э��ͷ��һ��ƥ��Ļ��ƣ��ڿͻ��˺ͷ���˶�Լ�������֡�
// ��ÿһ�������ǰ���һ�� *int*(data)
// ������
// ע��:
// ��½:
// ��ú����б�:
// ����:
// ��Ӻ���:
// ����:

// ��Զ����̣���

// �ͻ��ˣ�Qt
// net�ࣨ�շ����ݣ���ʼ�����磬�ر����磩
// �н����ࣨΪ���Ժ���չʹ�ã�
// kernel�ࣨ������յ������ݣ���֯Ҫ���͵����ݣ�
// ui�ࣨע��&��¼����&�Ѻ��б����&�������죩

// ����ˣ�VScode
// net�ࣨ�շ����ݣ���ʼ�����磬�ر����磩
// �н����ࣨΪ���Ժ���չʹ�ã�
// kernel�ࣨ������յ������ݣ���֯Ҫ���͵����ݣ�
// mysql��

// net��(�̳кͶ�̬)
// ����(�շ����ݣ���ʼ�����磬�ر�����)
// TCP����(TCP���࣬TCP�ͻ���)���շ����ݣ���ʼ�����磬�ر����磩
// UDP���ࣨ�շ����ݣ���ʼ�����磬�ر����磩
