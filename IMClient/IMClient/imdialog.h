#ifndef IMDIALOG_H
#define IMDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class IMDialog; }
QT_END_NAMESPACE

class IMDialog : public QDialog
{
    Q_OBJECT

public:
    IMDialog(QWidget *parent = nullptr);
    ~IMDialog();

    // 重写关闭事件，下线功能
    void closeEvent(QCloseEvent* event);

signals:
    //把注册信息发给kernel
    void sig_commitRegister(QString name,QString tel,QString pass);

    //把登录信息发给kernel
    void sig_commitLogin(QString tel,QString pass);

    // 通知kernel关闭程序
    void sig_closeApp();

private slots:
    void on_pb_clear_register_clicked();

    void on_pb_commit_register_clicked();

    void on_pb_clear_clicked();

    void on_pb_commit_clicked();

private:
    Ui::IMDialog *ui;
};
#endif // IMDIALOG_H
