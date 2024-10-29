#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QTime>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

// 设置聊天窗口信息
void ChatDialog::setChatInfo(int id, QString name)
{
    // 保存信息
    m_friendId = id;
    m_friendName = name;

    // 设置到控件上
    setWindowTitle(QString("与【%1】的聊天窗口").arg(m_friendName));
}

// 设置聊天请求内容到窗口上
void ChatDialog::setChatCotent(QString content)
{
    ui->tb_chat->append(QString("【%1】%2").arg(m_friendName).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);

}

// 显示聊天对象不在线
void ChatDialog::setFriendOfflice()
{
    ui->tb_chat->append(QString("【%1】%2 不在线").arg(m_friendName).arg(QTime::currentTime().toString("hh:mm:ss")));
}

void ChatDialog::on_pb_send_clicked()
{
    // 1.从控件上获取纯文本数据
    QString content = ui->te_chat->toPlainText();

    // 2.校验用户输入的数据是否合法
    if(content.isEmpty()||content.remove(" ").isEmpty()){
        ui->te_chat->setText("");
        return;
    }

    // 3.获得带格式的数据
    content = ui->te_chat->toHtml();

    // 4.清空编辑窗口
    ui->te_chat->setText("");

    // 5.把数据显示到浏览窗口上
    ui->tb_chat->append(QString("【我】%1").arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->tb_chat->append(content);

    // 6.把数据发给kernel
    Q_EMIT sig_sendChatMessage(content,m_friendId);
}

