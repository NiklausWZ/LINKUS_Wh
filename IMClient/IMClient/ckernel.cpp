#include "ckernel.h"
#include "mediator/TcpClientMediator.h"
#include <QDebug>
#include "net/def.h"
#include <QMessageBox>
#include "useritem.h"
#include <QTextCodec>
#include <QInputDialog>

CKernel::CKernel(QObject *parent) : QObject(parent)
{
    qDebug() << __func__;
    setProtocolMap();

    // 给好友列表界面new对象
    m_pFriendList = new FriendList;

    // 绑定下线的信号和槽函数
    connect(m_pFriendList,&FriendList::sig_offline,this,&CKernel::slot_offline);

    // 绑定添加好友的信号和槽函数
    connect(m_pFriendList,&FriendList::sig_addFriend,this,&CKernel::slot_addFriend);

    // 给登录&注册窗口new对象，并且显示
    m_pImDialog = new IMDialog;
    m_pImDialog->show();

    // 绑定关闭程序的信号和槽函数
    connect(m_pImDialog,&IMDialog::sig_closeApp,this,&CKernel::slot_closeApp);

    //绑定处理注册信息的信号和槽函数
    connect(m_pImDialog,&IMDialog::sig_commitRegister,this,&CKernel::slots_commitRegister);

    //绑定处理登录信息的信号和槽函数
    connect(m_pImDialog,&IMDialog::sig_commitLogin,this,&CKernel::slots_commitLogin);

    // 通过中介者指针来完成
    m_pMediator = new TcpClientMediator;
    // 打开网络
    if(!m_pMediator->openNet()){ // 打开网络失败
        QMessageBox::about(m_pImDialog,"info","netError");
        exit(0);
    }

    // 绑定处理所有数据的信号的槽函数
    connect(m_pMediator,&TcpClientMediator::sig_transmitData,this,&CKernel::slot_transmitData);

    // 测试代码
    // 客户端发个消息给服务端
    //    _STRU_LOGIN_RQ rq;
    //    m_pMediator->sendData((char*)&rq, sizeof(rq), 123);
}

CKernel::~CKernel()
{
    qDebug() << __func__;
    // 回收资源
    if(m_pImDialog){
        m_pImDialog->hide();
        delete m_pImDialog;
        m_pImDialog = nullptr;
    }
    if(m_pMediator){
        m_pMediator->closeNet();
        delete m_pMediator;
        m_pMediator = nullptr;
    }
    if(m_pFriendList){
        m_pFriendList->hide();
        delete m_pFriendList;
        m_pFriendList = nullptr;
    }

    // 好友聊天窗口，map中指针的取消
    for(auto ite = m_mapIdToChatdlg.begin();ite!=m_mapIdToChatdlg.end();){
        ChatDialog* chat = ite.value();
        if(chat){
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        // 把无效节点从map中删除
        ite = m_mapIdToChatdlg.erase(ite);
    }
}

// utf-8转GB2312
void CKernel::utf8Togb2312(QString src, char *dest, int len)
{
    QTextCodec* dc = QTextCodec::codecForName("gb2312");
    QByteArray ba = dc->fromUnicode(src);
    strcpy_s(dest,len,ba.data());

}

// GB2312转到utf-8
QString CKernel::gb2312Toutf8(char* src)
{
    QTextCodec* dc = QTextCodec::codecForName("gb2312");
    return dc->toUnicode(src);
}


// 绑定协议头和数组
void CKernel::setProtocolMap()
{
    qDebug()<<__func__;
    // 初始化数组
    memset(m_mapProtocol, 0, sizeof(m_mapProtocol));
    // 给数组赋值
    m_mapProtocol[_DEF_REGISTER_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealRegisterRs;
    m_mapProtocol[_DEF_LOGIN_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealLoginRs;
    m_mapProtocol[_DEF_FRIEND_INFO - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealFriendInfos;

    m_mapProtocol[_DEF_CHAT_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRq;
    m_mapProtocol[_DEF_CHAT_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealChatRs;

    m_mapProtocol[_DEF_OFFLINE_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealOfflineRq;

    m_mapProtocol[_DEF_ADD_FRIEND_RQ - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRq;
    m_mapProtocol[_DEF_ADD_FRIEND_RS - _DEF_PROTOCOL_BASE - 1] = &CKernel::dealAddFriendRs;
}

// 处理注册回复
void CKernel::dealRegisterRs(char *data, int len, long from)
{
    // 服务端处理的是请求，客户端处理的是回复
    qDebug()<<__func__;
    qDebug() << from;
    // 1、拆包
    _STRU_REGISTER_RS* rs=(_STRU_REGISTER_RS*)data;

    // 2、根据注册结果提示用户
    switch (rs->result) {
    case _register_success:
        QMessageBox::about(m_pImDialog,"提示","注册成功");
        break;
    case _register_tel_exists:
        QMessageBox::about(m_pImDialog,"提示","注册失败，电话号码已经被注册");
        break;
    case _register_name_exists:
        QMessageBox::about(m_pImDialog,"提示","注册失败，昵称已经被注册");
        break;
    default:
        break;
    }
}

// 处理登录回复
void CKernel::dealLoginRs(char *data, int len, long from)
{
    qDebug()<<__func__;
    // 1、拆包
    _STRU_LOGIN_RS* rs=(_STRU_LOGIN_RS*)data;

    // 2、根据登录结果提示用户
    switch (rs->result) {
    case _login_success:
    {
        // 进行页面跳转，隐藏登录信息，显示好友列表界面
        m_pImDialog->hide();
        m_pFriendList->show();
        // 保存登录用户的id
        m_id = rs->userid;
    }
        break;
    case _login_tel_not_exists:
        QMessageBox::about(m_pImDialog,"提示","登录失败，电话号码未注册");
        break;
    case _login_password_error:
        QMessageBox::about(m_pImDialog,"提示","登录失败，密码错误");
        break;
    default:
        break;
    }
}

// 处理好友信息
void CKernel::dealFriendInfos(char *data, int len, long from)
{
    qDebug() << __func__;
    // 1.拆包
    _STRU_FRIEND_INFO* rq=(_STRU_FRIEND_INFO*)data;
    QString name = gb2312Toutf8(rq->name);
    QString feeling = gb2312Toutf8(rq->feeling);
    //    qDebug() << rq->name;
    //    qDebug() << rq->usedId;


    //    // 测试代码，添加10个好友，5个在线，5个不在线
    //    for(int i = 1;i<6;i++){
    //        // new一个好友
    //        useritem* item = new useritem;
    //        // 设置好友信息
    //        item->setFriendInfo(i,_status_online,3,QString("用户%1").arg(i),"sdasdsad");
    //        // 把好友添加到列表上
    //        m_pFriendList->addFriend(item);
    //    }
    //    for(int i = 6;i<11;i++){
    //        // new一个好友
    //        useritem* item = new useritem;
    //        // 设置好友信息
    //        item->setFriendInfo(i,_status_offline,5,QString("用户%1").arg(i),"sdasdsad");
    //        // 把好友添加到列表上
    //        m_pFriendList->addFriend(item);
    //    }


    // 2.判断是不是自己的信息

    //    qDebug() << rq->usedId;
    //    qDebug() << m_id;

    if(rq->usedId == m_id){

        // 保存自己的昵称
        m_name = name;
        // 把自己的信息设置到界面上
        m_pFriendList->setUserInfo(name,feeling,rq->iconId);
        return;
    }

    // 好友的信息，判断列表是不是已经存在这个好友了
    if(m_mapIdToUseritem.count(rq->usedId)>0){
        // 有好友，那就更新好友的信息(好友状态变化的时候-上线和下限)
        useritem* item = m_mapIdToUseritem[rq->usedId];
        item->setFriendInfo(rq->usedId,rq->status,rq->iconId,name,feeling);
    }else{
        // 没有好友的话，new一个好友
        useritem* item = new useritem;

        // 设置好友的信息
        item->setFriendInfo(rq->usedId,rq->status,rq->iconId,name,feeling);

        // 把好友添加到列表上
        m_pFriendList->addFriend(item);

        // 保存useritem
        m_mapIdToUseritem[rq->usedId] = item;

        // 绑定显示聊天窗口的信号和槽函数
        connect(item,&useritem::sig_showChatDialog,this,&CKernel::slots_showChatDialog);

        // new一个和这个好友绑定的聊天窗口
        ChatDialog* chat = new ChatDialog;

        // 设置窗口信息
        chat->setChatInfo(rq->usedId, name);

        // 保存聊天窗口
        m_mapIdToChatdlg[rq->usedId] = chat;

        // 绑定发送聊天内容的信号和槽函数
        connect(chat,&ChatDialog::sig_sendChatMessage,this,&CKernel::slot_sendChatMessage);
    }

}

// 处理聊天请求
void CKernel::dealChatRq(char *data, int len, long from)
{
    qDebug() << __func__;
    // 1.拆包
    _STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;
    // 2.当前是B客户端，聊天内容是A客户端发送过来的
    // 找到A客户端的聊天窗口
    if(m_mapIdToChatdlg.count(rq->usedId)>0){
        // 设置聊天内容到窗口上，并且显示窗口
        ChatDialog* chat = m_mapIdToChatdlg[rq->usedId];
        chat->setChatCotent(rq->content);
        chat->showNormal();
    }
}


// 处理聊天回复
void CKernel::dealChatRs(char *data, int len, long from)
{
    qDebug() << __func__;
    // 1. 拆包
    _STRU_CHAT_RS* rs = (_STRU_CHAT_RS*)data;
    // 2. 我是A客户端，B客户端不在线
    // 找到跟B客户端的聊天窗口
    if(m_mapIdToChatdlg.count(rs->friendId)>0){
        // 显示B客户端不在线，显示聊天窗口
        ChatDialog* chat = m_mapIdToChatdlg[rs->friendId];
        chat->setFriendOfflice();
        chat->showNormal();
    }
}

// 处理下线请求
void CKernel::dealOfflineRq(char *data, int len, long from)
{
    qDebug() << __func__;
    // 1.拆包
    _STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;
    // 2. 找到好友的useritem
    if(m_mapIdToUseritem.count(rq->userId>0)){
        useritem* item = m_mapIdToUseritem[rq->userId];
        // 设置好友的状态为下线状态
        item->setFriendOffLine();
    }
}


// 处理添加好友请求（是b客户端）
void CKernel::dealAddFriendRq(char *data, int len, long from)
{
    qDebug()<< __func__;
    // 1.拆包
    _STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;
    // 2.提示用用有人添加好友，询问是否同意
    _STRU_ADD_FRIEND_RS rs;
    if(QMessageBox::Yes == QMessageBox::question(m_pFriendList,"添加好友",QString("【%1】想要添加你为好友，是否同意？").arg(rq->userName))){
        rs.result = _add_friend_success;
    }else{
        rs.result = _add_friend_refuse;
    }
    rs.aId = rq->usedId;
    rs.bId = m_id;
    strcpy_s(rs.friendName,m_name.toStdString().c_str());
    // 把结果发给服务端
    m_pMediator->sendData((char*)&rs,sizeof(rs),78);
}
// 处理添加好友回复
void CKernel::dealAddFriendRs(char *data, int len, long from)
{
    qDebug()<< __func__;

    // 1.拆包
    _STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;
    QString friendName = gb2312Toutf8(rs->friendName);
    qDebug() << rs->result;
    // 2. 根据结果提示用户
    switch(rs->result){
    case _add_friend_success:
        QMessageBox::about(m_pFriendList,"标题",QString("添加【%1】成为好友成功").arg(rs->friendName));
        break;
    case _add_friend_not_exists:
        QMessageBox::about(m_pFriendList,"标题",QString("添加【%1】成为好友失败，好友不存在").arg(friendName));
        break;
    case _add_friend_offline:
        QMessageBox::about(m_pFriendList,"标题",QString("添加【%1】成为好友失败，好友不在线").arg(friendName));
        break;
    case _add_friend_refuse:
        QMessageBox::about(m_pFriendList,"标题",QString("添加【%1】拒绝成为你的好友").arg(rs->friendName));
        break;
    default:
        break;
    }
}



// 处理好友信息
void CKernel::slot_transmitData(char *data, int len, long from)
{
    qDebug() << __func__;
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
            qDebug() << "type2:" << type ;
        }
    }
    else{
        // 如果打印出日志，两种原因：1、结构体协议头赋值错误；2、接受数据有问题，可能offset没清零
        qDebug()<< "type1:" << type ;
    }
}

// 注册槽函数，提交按钮，处理注册信息
void CKernel::slots_commitRegister(QString name, QString tel, QString pass)
{
    qDebug()<<__func__;
    // 1、打包
    _STRU_REGISTER_RQ rq;
    //    strcpy_s(rq.name,name.toStdString().c_str());
    utf8Togb2312(name,rq.name,sizeof(rq));
    strcpy_s(rq.tel,tel.toStdString().c_str());
    strcpy_s(rq.password,pass.toStdString().c_str());
    // 2、发给服务端
    m_pMediator->sendData((char*)&rq,sizeof(rq),6879);
}

// 提交槽函数，提交按钮，处理登录信息
void CKernel::slots_commitLogin(QString tel, QString pass)
{
    qDebug()<<__func__;
    // 1、打包
    _STRU_LOGIN_RQ rq;
    strcpy_s(rq.tel,tel.toStdString().c_str());
    strcpy_s(rq.password,pass.toStdString().c_str());
    // 2、发给服务端
    m_pMediator->sendData((char*)&rq,sizeof(rq),789);

}

// 处理显示聊天窗口
void CKernel::slots_showChatDialog(int id)
{
    qDebug()<<__func__;
    // 判断map中是否有这个窗口
    if(m_mapIdToChatdlg.count(id)>0){
        // 找到窗口并且显示
        ChatDialog* chat = m_mapIdToChatdlg[id];
        chat->showNormal();
    }
}

// 处理聊天内容发送给服务端
void CKernel::slot_sendChatMessage(QString content, int id)
{
    qDebug()<<__func__;
    // 1.打包
    _STRU_CHAT_RQ rq;
    rq.usedId = m_id;
    rq.friendId = id;
    strcpy_s(rq.content,content.toStdString().c_str());

    // 2.发送
    m_pMediator->sendData((char*)&rq,sizeof(rq),89);

    // 需不需要转码？需要，1.是不是中文，2.服务器不需要，但是真实需要关心
}


// 关闭程序
void CKernel::slot_closeApp()
{
    qDebug()<<__func__;
    // 1.回收资源
    if(m_pImDialog){
        m_pImDialog->hide();
        delete m_pImDialog;
        m_pImDialog = nullptr;
    }
    if(m_pMediator){
        m_pMediator->closeNet();
        delete m_pMediator;
        m_pMediator = nullptr;
    }
    if(m_pFriendList){
        m_pFriendList->hide();
        delete m_pFriendList;
        m_pFriendList = nullptr;
    }

    // 好友聊天窗口，map中指针的取消
    for(auto ite = m_mapIdToChatdlg.begin();ite!=m_mapIdToChatdlg.end();){
        ChatDialog* chat = ite.value();
        if(chat){
            chat->hide();
            delete chat;
            chat = nullptr;
        }
        // 把无效节点从map中删除
        ite = m_mapIdToChatdlg.erase(ite);
    }
    // 退出进程
    exit(0);
}

void CKernel::slot_offline()
{
    qDebug() << __func__;
    // 1.要给服务端发送下线通知
    _STRU_OFFLINE_RQ rq;
    rq.userId = m_id;
    m_pMediator->sendData((char*)&rq,sizeof(rq),890);
    // 2.回收资源，退出进程
    slot_closeApp();

}

void CKernel::slot_addFriend()
{
    qDebug() << __func__;
    // 1.弹出输入窗口，让用户昵称
    QString friendName = QInputDialog::getText(m_pFriendList,"添加好友","请输入好友的昵称:");
    qDebug() << friendName;
    QString friendNameTmp = friendName;

    // 2.判断用户输入的内容是否合法
    if(friendName.isEmpty()||friendNameTmp.remove(" ").isEmpty()){
        return;
    }

    // 3.判断输入的昵称是否是自己的昵称
    if(friendName == m_name){
        QMessageBox::about(m_pFriendList,"提示","不能添加自己为好友");
        return;
    }

    // 4.判断昵称是否为好友
    for(useritem* item:m_mapIdToUseritem){
        if(item->name() == friendName){
            QMessageBox::about(m_pFriendList,"提示","已经是好友了");
            return;
        }

    }

    // 5.给服务端发送添加好友请求？流程
    _STRU_ADD_FRIEND_RQ rq;
    rq.usedId = m_id;
    //    strcpy_s(rq.friendName,friendName.toStdString().c_str());
    utf8Togb2312(friendName,rq.friendName,sizeof(rq.friendName));
    strcpy_s(rq.userName,m_name.toStdString().c_str());
    m_pMediator->sendData((char*)&rq,sizeof(rq),89);

}

// 字符串转换:char*/char[]，std::string,QString，不是内容转换，是相同的内容存储到不同类型的变量里
// char*/char[]是基础类型，std::string和QString是类，基础类型是可以直接给对象赋值的
// std::string调用c_str()转换成char数组，char*
// QString调用toStdString()转换成std::string

