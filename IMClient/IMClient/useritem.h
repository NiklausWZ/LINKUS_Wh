#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>
#include "net/def.h"
namespace Ui {
class useritem;
}

class useritem : public QWidget
{
    Q_OBJECT

signals:
    // 通知kernel显示聊天窗口
    void sig_showChatDialog(int id);

public:
    explicit useritem(QWidget *parent = nullptr);
    ~useritem();

    // 保存并且式子好友信息
    void setFriendInfo(int friendId,int status,int iconId,QString name,QString feeling);

    // 设置好友的状态为下线状态
    void setFriendOffLine();


    const QString &name() const;

private slots:
    void on_pb_icon_clicked();

private:
    Ui::useritem *ui;
    int m_friendId;
    int m_status;
    int m_iconId;
    QString m_name;
    QString m_feeling;
};

#endif // USERITEM_H
