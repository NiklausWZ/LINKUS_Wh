/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QTextBrowser *tb_chat;
    QGroupBox *groupBox;
    QPushButton *pb_tool1;
    QPushButton *pb_tool2;
    QPushButton *pb_tool3;
    QPushButton *pb_send;
    QTextEdit *te_chat;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName(QString::fromUtf8("ChatDialog"));
        ChatDialog->resize(804, 533);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(14);
        ChatDialog->setFont(font);
        tb_chat = new QTextBrowser(ChatDialog);
        tb_chat->setObjectName(QString::fromUtf8("tb_chat"));
        tb_chat->setGeometry(QRect(12, 12, 661, 207));
        groupBox = new QGroupBox(ChatDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 230, 311, 70));
        pb_tool1 = new QPushButton(groupBox);
        pb_tool1->setObjectName(QString::fromUtf8("pb_tool1"));
        pb_tool1->setGeometry(QRect(15, 13, 45, 45));
        pb_tool2 = new QPushButton(groupBox);
        pb_tool2->setObjectName(QString::fromUtf8("pb_tool2"));
        pb_tool2->setGeometry(QRect(80, 13, 45, 45));
        pb_tool3 = new QPushButton(groupBox);
        pb_tool3->setObjectName(QString::fromUtf8("pb_tool3"));
        pb_tool3->setGeometry(QRect(145, 13, 45, 45));
        pb_send = new QPushButton(ChatDialog);
        pb_send->setObjectName(QString::fromUtf8("pb_send"));
        pb_send->setGeometry(QRect(688, 458, 101, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font1.setPointSize(17);
        pb_send->setFont(font1);
        te_chat = new QTextEdit(ChatDialog);
        te_chat->setObjectName(QString::fromUtf8("te_chat"));
        te_chat->setGeometry(QRect(12, 310, 661, 211));

        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QApplication::translate("ChatDialog", "Dialog", nullptr));
        groupBox->setTitle(QString());
        pb_tool1->setText(QString());
        pb_tool2->setText(QString());
        pb_tool3->setText(QString());
        pb_send->setText(QApplication::translate("ChatDialog", "\345\217\221 \351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
