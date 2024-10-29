/********************************************************************************
** Form generated from reading UI file 'useritem.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERITEM_H
#define UI_USERITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_useritem
{
public:
    QPushButton *pb_icon;
    QLabel *lb_feeling;
    QLabel *lb_name;

    void setupUi(QWidget *useritem)
    {
        if (useritem->objectName().isEmpty())
            useritem->setObjectName(QString::fromUtf8("useritem"));
        useritem->resize(346, 61);
        useritem->setMinimumSize(QSize(346, 61));
        useritem->setMaximumSize(QSize(346, 61));
        pb_icon = new QPushButton(useritem);
        pb_icon->setObjectName(QString::fromUtf8("pb_icon"));
        pb_icon->setGeometry(QRect(5, 5, 51, 51));
        lb_feeling = new QLabel(useritem);
        lb_feeling->setObjectName(QString::fromUtf8("lb_feeling"));
        lb_feeling->setGeometry(QRect(70, 35, 151, 20));
        lb_name = new QLabel(useritem);
        lb_name->setObjectName(QString::fromUtf8("lb_name"));
        lb_name->setGeometry(QRect(70, 10, 151, 20));

        retranslateUi(useritem);

        QMetaObject::connectSlotsByName(useritem);
    } // setupUi

    void retranslateUi(QWidget *useritem)
    {
        useritem->setWindowTitle(QApplication::translate("useritem", "Form", nullptr));
        pb_icon->setText(QString());
        lb_feeling->setText(QApplication::translate("useritem", "\345\255\246\344\271\240\344\270\215\347\264\257\344\272\206", nullptr));
        lb_name->setText(QApplication::translate("useritem", "\347\216\213\345\255\246\344\270\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class useritem: public Ui_useritem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERITEM_H
