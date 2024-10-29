/********************************************************************************
** Form generated from reading UI file 'imdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMDIALOG_H
#define UI_IMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IMDialog
{
public:
    QTabWidget *tb_page;
    QWidget *page_login;
    QLabel *lb_tel;
    QLabel *lb_password;
    QLineEdit *le_tel;
    QLineEdit *le_password;
    QPushButton *pb_clear;
    QPushButton *pb_commit;
    QWidget *page_register;
    QLineEdit *le_tel_register;
    QLineEdit *le_password_register;
    QLineEdit *le_name_register;
    QLabel *lb_password_register;
    QLabel *lb_tel_register;
    QLabel *lb_name_register;
    QPushButton *pb_clear_register;
    QPushButton *pb_commit_register;
    QLabel *lb_icon;

    void setupUi(QDialog *IMDialog)
    {
        if (IMDialog->objectName().isEmpty())
            IMDialog->setObjectName(QString::fromUtf8("IMDialog"));
        IMDialog->resize(946, 627);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(19);
        IMDialog->setFont(font);
        tb_page = new QTabWidget(IMDialog);
        tb_page->setObjectName(QString::fromUtf8("tb_page"));
        tb_page->setGeometry(QRect(400, 0, 551, 631));
        page_login = new QWidget();
        page_login->setObjectName(QString::fromUtf8("page_login"));
        lb_tel = new QLabel(page_login);
        lb_tel->setObjectName(QString::fromUtf8("lb_tel"));
        lb_tel->setGeometry(QRect(50, 130, 131, 71));
        lb_password = new QLabel(page_login);
        lb_password->setObjectName(QString::fromUtf8("lb_password"));
        lb_password->setGeometry(QRect(50, 210, 131, 71));
        le_tel = new QLineEdit(page_login);
        le_tel->setObjectName(QString::fromUtf8("le_tel"));
        le_tel->setGeometry(QRect(180, 150, 241, 41));
        le_password = new QLineEdit(page_login);
        le_password->setObjectName(QString::fromUtf8("le_password"));
        le_password->setGeometry(QRect(180, 230, 241, 41));
        le_password->setEchoMode(QLineEdit::Password);
        pb_clear = new QPushButton(page_login);
        pb_clear->setObjectName(QString::fromUtf8("pb_clear"));
        pb_clear->setGeometry(QRect(60, 400, 131, 51));
        pb_commit = new QPushButton(page_login);
        pb_commit->setObjectName(QString::fromUtf8("pb_commit"));
        pb_commit->setGeometry(QRect(300, 400, 121, 51));
        tb_page->addTab(page_login, QString());
        page_register = new QWidget();
        page_register->setObjectName(QString::fromUtf8("page_register"));
        le_tel_register = new QLineEdit(page_register);
        le_tel_register->setObjectName(QString::fromUtf8("le_tel_register"));
        le_tel_register->setGeometry(QRect(210, 230, 241, 41));
        le_password_register = new QLineEdit(page_register);
        le_password_register->setObjectName(QString::fromUtf8("le_password_register"));
        le_password_register->setGeometry(QRect(210, 310, 241, 41));
        le_password_register->setEchoMode(QLineEdit::Password);
        le_name_register = new QLineEdit(page_register);
        le_name_register->setObjectName(QString::fromUtf8("le_name_register"));
        le_name_register->setGeometry(QRect(210, 140, 241, 41));
        lb_password_register = new QLabel(page_register);
        lb_password_register->setObjectName(QString::fromUtf8("lb_password_register"));
        lb_password_register->setGeometry(QRect(80, 290, 131, 71));
        lb_tel_register = new QLabel(page_register);
        lb_tel_register->setObjectName(QString::fromUtf8("lb_tel_register"));
        lb_tel_register->setGeometry(QRect(80, 210, 131, 71));
        lb_name_register = new QLabel(page_register);
        lb_name_register->setObjectName(QString::fromUtf8("lb_name_register"));
        lb_name_register->setGeometry(QRect(80, 130, 131, 71));
        pb_clear_register = new QPushButton(page_register);
        pb_clear_register->setObjectName(QString::fromUtf8("pb_clear_register"));
        pb_clear_register->setGeometry(QRect(140, 440, 131, 51));
        pb_commit_register = new QPushButton(page_register);
        pb_commit_register->setObjectName(QString::fromUtf8("pb_commit_register"));
        pb_commit_register->setGeometry(QRect(380, 440, 121, 51));
        tb_page->addTab(page_register, QString());
        lb_icon = new QLabel(IMDialog);
        lb_icon->setObjectName(QString::fromUtf8("lb_icon"));
        lb_icon->setGeometry(QRect(0, 0, 401, 631));
        lb_icon->setPixmap(QPixmap(QString::fromUtf8(":/images/register.png")));
        lb_icon->setScaledContents(true);

        retranslateUi(IMDialog);

        tb_page->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(IMDialog);
    } // setupUi

    void retranslateUi(QDialog *IMDialog)
    {
        IMDialog->setWindowTitle(QApplication::translate("IMDialog", "IMDialog", nullptr));
        lb_tel->setText(QApplication::translate("IMDialog", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        lb_password->setText(QApplication::translate("IMDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        le_tel->setText(QApplication::translate("IMDialog", "13653405369", nullptr));
        le_password->setText(QApplication::translate("IMDialog", "123", nullptr));
        pb_clear->setText(QApplication::translate("IMDialog", "\346\270\205 \347\251\272", nullptr));
        pb_commit->setText(QApplication::translate("IMDialog", "\346\217\220 \344\272\244", nullptr));
        tb_page->setTabText(tb_page->indexOf(page_login), QApplication::translate("IMDialog", "\347\231\273\345\275\225", nullptr));
        lb_password_register->setText(QApplication::translate("IMDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        lb_tel_register->setText(QApplication::translate("IMDialog", "\346\211\213\346\234\272\345\217\267\357\274\232", nullptr));
        lb_name_register->setText(QApplication::translate("IMDialog", "\346\230\265\347\247\260\357\274\232", nullptr));
        pb_clear_register->setText(QApplication::translate("IMDialog", "\346\270\205 \347\251\272", nullptr));
        pb_commit_register->setText(QApplication::translate("IMDialog", "\346\217\220 \344\272\244", nullptr));
        tb_page->setTabText(tb_page->indexOf(page_register), QApplication::translate("IMDialog", "\346\263\250\345\206\214", nullptr));
        lb_icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class IMDialog: public Ui_IMDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMDIALOG_H
