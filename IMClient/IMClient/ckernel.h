#ifndef CKERNEL_H
#define CKERNEL_H
#include "imdialog.h"
#include "mediator/mediator.h"
#include <QObject>
#include "friendlist.h"
#include "net/def.h"
#include <QMap>
#include "chatdialog.h"

// 声明函数指针
class CKernel;
// 这是一个指向类的成员函数的函数指针PFUN，但是呢类是在下面定义的。
typedef void (CKernel::*PFUN)(char* data, int len, long from);

class CKernel : public QObject
{
    Q_OBJECT
public:
    explicit CKernel(QObject *parent = nullptr);
    ~CKernel();

    // utf-8转GB2312
    void utf8Togb2312(QString src,char* dest,int len);

    // GB2312转utf-8
    QString gb2312Toutf8(char* src);

    // 绑定协议头和数组
    void setProtocolMap();
    // 处理注册请求
    void dealRegisterRs(char* data, int len, long from);
    // 处理登录请求
    void dealLoginRs(char* data, int len, long from);
    // 处理好友信息
    void dealFriendInfos(char* data, int len, long from);
    // 处理聊天请求
    void dealChatRq(char* data,int len,long from);
    // 处理聊天回复
    void dealChatRs(char* data,int len,long from);
    // 处理下线请求
    void dealOfflineRq(char* data,int len,long from);
    // 处理添加好友请求
    void dealAddFriendRq(char* data,int len,long from);
    // 处理添加好友回复
    void dealAddFriendRs(char* data,int len,long from);
signals:

public slots:
    // 处理所有接受到的数据
    void slot_transmitData(char* data, int len, long from);
    // 处理注册信息
    void slots_commitRegister(QString name,QString tel,QString pass);
    // 处理登录信息
    void slots_commitLogin(QString tel,QString pass);
    // 处理显示聊天窗口
    void slots_showChatDialog(int id);
    // 处理聊天内容发给服务端
    void slot_sendChatMessage(QString content,int id);
    // 处理关闭程序的信号
    void slot_closeApp();
    // 处理friendList的下线信号
    void slot_offline();
    // 处理添加好友的信号
    void slot_addFriend();
private:

    int m_id;
    QString m_name;
    IMDialog* m_pImDialog;// 界面指针
    mediator* m_pMediator;// 中介者指针
    FriendList* m_pFriendList; // 好友指针
    // 函数指针数组
    PFUN m_mapProtocol[_DEF_PROTOCOL_COUNT];

    // 保存好友，提问需要回收useritem*吗，不用
    QMap<int, useritem*> m_mapIdToUseritem;

    // 保存和好友的聊天窗口
    QMap<int, ChatDialog*> m_mapIdToChatdlg;
};

#endif // CKERNEL_H


// 客户端做一个字符转换
// Qt使用的是utf-8（QString），vs使用的是gb2312（char[]）
// 客户端做字符转换
// Qt发给Vs,就是utf-8转GB2312
// 反之，就是GB2312转utf-8

