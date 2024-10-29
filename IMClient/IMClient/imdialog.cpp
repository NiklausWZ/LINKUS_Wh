#include "imdialog.h"
#include "ui_imdialog.h"
#include <QDebug>
#include <QMessageBox>
IMDialog::IMDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IMDialog)
{
    ui->setupUi(this);
}

IMDialog::~IMDialog()
{
    delete ui;
}

// 重写关闭事件
void IMDialog::closeEvent(QCloseEvent *event)
{
    Q_EMIT sig_closeApp();
}

// 注册界面点击提交
void IMDialog::on_pb_commit_register_clicked()
{
    qDebug() << __func__;
    // 1.从控件上面获取用户输入的数据
    QString name=ui->le_name_register->text();
    QString tel=ui->le_tel_register->text();
    QString pass=ui->le_password_register->text();
    QString nameTmp=name;
    QString telTmp=tel;
    QString passTmp=pass;

    // 2、校验用户数据合法性
    // 2.1、校验是否为空或者全空格
    if(name.isEmpty()||tel.isEmpty()||pass.isEmpty()||
            name.remove(" ").isEmpty()||tel.remove(" ").isEmpty()||pass.remove(" ").isEmpty()){
        QMessageBox::about(this,"提示","输入内容不能为空或者全空格");
        return;
    }

    // 2.2、校验长度(昵称不能超过15，密码不能超过20，电话号码必须是11位）
    if(name.length()>15||pass.length()>20||tel.length()!=11){
        QMessageBox::about(this,"提示","昵称不能超过15，密码不能超过20，电话号码必须是11位");
        return;
    }

    // 2.3、校验内容（电话号码必须是数字，满足电话号码的规律，密码只能是字母数字下划线）-暂时不写，正则表达式

    // 3、把数据传给kernel
    Q_EMIT sig_commitRegister(name,tel,pass);
}

// 注册界面的输入内容清空
void IMDialog::on_pb_clear_register_clicked()
{
    qDebug() << __func__;
    ui->le_name_register->setText("");
    ui->le_name_register->setText("");
    ui->le_tel_register->setText("");
}

void IMDialog::on_pb_commit_clicked()
{
    qDebug() << __func__;
    // 1、从控件上获取用户输入的数据
    QString tel=ui->le_tel->text();
    QString pass=ui->le_password->text();
    QString telTmp=tel;
    QString passTmp=pass;
    // 2、校验用户数据合法性
    // 2.1、校验是否为空或者全空格
    if(tel.isEmpty()||pass.isEmpty()||
            tel.remove(" ").isEmpty()||pass.remove(" ").isEmpty()){
        QMessageBox::about(this,"提示","输入内容不能为空或者全空格");
        return;
    }
    // 2.2、校验长度(密码不能超过20，电话号码必须是11位）
    if(pass.length()>20||tel.length()!=11){
        QMessageBox::about(this,"提示","密码不能超过20，电话号码必须是11位");
        return;
    }
    // 2.3、校验内容（电话号码必须是数字，满足电话号码的规律，密码只能是字母数字下划线）-暂时不写，正则表达式

    // 3、把登录信息发给kernel
    Q_EMIT sig_commitLogin(tel,pass);
}

// 登录界面的输入内容清空
void IMDialog::on_pb_clear_clicked()
{
    qDebug() << __func__;
    ui->le_password->setText("");
    ui->le_tel->setText("");
}




