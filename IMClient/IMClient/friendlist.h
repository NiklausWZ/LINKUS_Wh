#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QDialog>
#include <QVBoxLayout>
#include "useritem.h"
#include <QMenu>
namespace Ui {
class FriendList;
}

class FriendList : public QDialog
{
    Q_OBJECT


signals:
    // 通知kernel下线
    void sig_offline();

    // 通知kernel添加好友
    void sig_addFriend();



public:
    explicit FriendList(QWidget *parent = nullptr);
    ~FriendList();

    // 添加好友到列表上
    void addFriend(useritem* item);

    // 设置自己的信息
    void setUserInfo(QString name,QString feeling,int iconId);

    // 重写关闭事件
    void closeEvent(QCloseEvent * event);

private slots:
    void on_pb_menu_clicked();
    // 点击菜单项上面的信号
    void slot_menu_clicked(QAction* action);

private:
    Ui::FriendList* ui;
    // 把小控件添加上大控件上，需要有一个层layout
    QVBoxLayout* m_layout;
    // 定义一个菜单的对象
    QMenu* m_pMenu;
};

#endif // FRIENDLIST_H
