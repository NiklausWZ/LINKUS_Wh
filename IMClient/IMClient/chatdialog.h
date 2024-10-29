#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

signals:
    // 把聊天内容发送给kernel
    void sig_sendChatMessage(QString content,int id);

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    // 设置聊天窗口信息
    void setChatInfo(int id, QString name);

    // 设置聊天请求内容到窗口上
    void setChatCotent(QString content);

    // 显示聊天对象不在线
    void setFriendOfflice();

private slots:
    void on_pb_send_clicked();

private:
    Ui::ChatDialog *ui;
    int m_friendId;
    QString m_friendName;
};

#endif // CHATDIALOG_H
