#include "friendlist.h"
#include "ui_friendlist.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
FriendList::FriendList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendList)
{
    ui->setupUi(this);
    // new一个垂直布局的层
    m_layout = new QVBoxLayout;
    // 设置小控件之间的间距
    m_layout ->setSpacing(3);
    // 设置小控件和大控件间的距离
    m_layout->setContentsMargins(0,0,0,0);
    // 把层添加到大控件上
    ui->wdg_list->setLayout(m_layout);
    // new一个添加好友菜单的对象
    m_pMenu = new QMenu(this);
    // 设置菜单选项
    m_pMenu->addAction("添加好友");
    m_pMenu->addAction("系统设置");
    // 绑定菜单项点击的信号和槽函数
    connect(m_pMenu,&QMenu::triggered,this,&FriendList::slot_menu_clicked);


}

FriendList::~FriendList()
{
    delete ui;
}

// 添加好友到列表上
void FriendList::addFriend(useritem *item)
{
    m_layout->addWidget(item);
}

// 设置自己的信息
void FriendList::setUserInfo(QString name, QString feeling, int iconId)
{
    ui->lb_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconId)));
}

void FriendList::closeEvent(QCloseEvent *event)
{
    // 忽略当前时间(否则会继续走父类的关闭函数，还是能够关闭窗口)
    event->ignore();
    // 询问用户是否能确认关闭程序
    if(QMessageBox::Yes == QMessageBox::question(this,"标题","是否确认退出？")){
        Q_EMIT sig_offline();
         // Q_EMIT的话，信号记得加括号
    }

}

void FriendList::on_pb_menu_clicked()
{
    // 再鼠标点击位置，向上弹出菜单栏
    // 获取鼠标点击时候的坐标
    QPoint point = QCursor::pos();

    // 获取菜单栏的绝对大小
    QSize size = m_pMenu->sizeHint();

    // 显示菜单
    m_pMenu->exec(QPoint(point.x(),point.y()-size.height()));
}

void FriendList::slot_menu_clicked(QAction *action)
{
    qDebug() << action->text();
    if("添加好友" == action->text()){
        Q_EMIT sig_addFriend();
    }else if("系统设置" == action->text()){

    }
}

