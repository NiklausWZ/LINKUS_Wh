/********************************************************************************
** Form generated from reading UI file 'friendlist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDLIST_H
#define UI_FRIENDLIST_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendList
{
public:
    QPushButton *pb_icon;
    QLabel *lb_name;
    QLineEdit *le_feeling;
    QTabWidget *tw_page;
    QWidget *page_message;
    QWidget *page_friend;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QWidget *wdg_list;
    QSpacerItem *verticalSpacer;
    QWidget *page_space;
    QPushButton *pb_menu;
    QPushButton *pb_tool1;
    QPushButton *pb_tool2;
    QPushButton *pb_tool3;

    void setupUi(QDialog *FriendList)
    {
        if (FriendList->objectName().isEmpty())
            FriendList->setObjectName(QString::fromUtf8("FriendList"));
        FriendList->resize(356, 744);
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(16);
        FriendList->setFont(font);
        pb_icon = new QPushButton(FriendList);
        pb_icon->setObjectName(QString::fromUtf8("pb_icon"));
        pb_icon->setGeometry(QRect(10, 10, 71, 71));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/tx/21.png"), QSize(), QIcon::Normal, QIcon::Off);
        pb_icon->setIcon(icon);
        pb_icon->setIconSize(QSize(80, 80));
        pb_icon->setFlat(true);
        lb_name = new QLabel(FriendList);
        lb_name->setObjectName(QString::fromUtf8("lb_name"));
        lb_name->setGeometry(QRect(90, 12, 211, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font1.setPointSize(16);
        lb_name->setFont(font1);
        le_feeling = new QLineEdit(FriendList);
        le_feeling->setObjectName(QString::fromUtf8("le_feeling"));
        le_feeling->setGeometry(QRect(90, 46, 211, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font2.setPointSize(12);
        le_feeling->setFont(font2);
        tw_page = new QTabWidget(FriendList);
        tw_page->setObjectName(QString::fromUtf8("tw_page"));
        tw_page->setGeometry(QRect(2, 86, 351, 618));
        page_message = new QWidget();
        page_message->setObjectName(QString::fromUtf8("page_message"));
        tw_page->addTab(page_message, QString());
        page_friend = new QWidget();
        page_friend->setObjectName(QString::fromUtf8("page_friend"));
        scrollArea = new QScrollArea(page_friend);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(4, 0, 341, 581));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 339, 579));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        wdg_list = new QWidget(scrollAreaWidgetContents);
        wdg_list->setObjectName(QString::fromUtf8("wdg_list"));

        verticalLayout->addWidget(wdg_list);

        verticalSpacer = new QSpacerItem(20, 537, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);
        tw_page->addTab(page_friend, QString());
        page_space = new QWidget();
        page_space->setObjectName(QString::fromUtf8("page_space"));
        tw_page->addTab(page_space, QString());
        pb_menu = new QPushButton(FriendList);
        pb_menu->setObjectName(QString::fromUtf8("pb_menu"));
        pb_menu->setGeometry(QRect(6, 712, 26, 26));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font3.setPointSize(18);
        pb_menu->setFont(font3);
        pb_tool1 = new QPushButton(FriendList);
        pb_tool1->setObjectName(QString::fromUtf8("pb_tool1"));
        pb_tool1->setGeometry(QRect(280, 712, 26, 26));
        pb_tool1->setFont(font3);
        pb_tool2 = new QPushButton(FriendList);
        pb_tool2->setObjectName(QString::fromUtf8("pb_tool2"));
        pb_tool2->setGeometry(QRect(310, 712, 26, 26));
        pb_tool2->setFont(font3);
        pb_tool3 = new QPushButton(FriendList);
        pb_tool3->setObjectName(QString::fromUtf8("pb_tool3"));
        pb_tool3->setGeometry(QRect(250, 712, 26, 26));
        pb_tool3->setFont(font3);

        retranslateUi(FriendList);

        tw_page->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FriendList);
    } // setupUi

    void retranslateUi(QDialog *FriendList)
    {
        FriendList->setWindowTitle(QApplication::translate("FriendList", "Dialog", nullptr));
        pb_icon->setText(QString());
        lb_name->setText(QApplication::translate("FriendList", "\347\216\213\346\230\212", nullptr));
        le_feeling->setText(QApplication::translate("FriendList", "\345\255\246\344\271\240\345\245\275\347\264\257\345\221\200", nullptr));
        tw_page->setTabText(tw_page->indexOf(page_message), QApplication::translate("FriendList", "\346\266\210\346\201\257", nullptr));
        tw_page->setTabText(tw_page->indexOf(page_friend), QApplication::translate("FriendList", "\350\201\224\347\263\273\344\272\272", nullptr));
        tw_page->setTabText(tw_page->indexOf(page_space), QApplication::translate("FriendList", "\347\251\272\351\227\264", nullptr));
        pb_menu->setText(QString());
        pb_tool1->setText(QString());
        pb_tool2->setText(QString());
        pb_tool3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FriendList: public Ui_FriendList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDLIST_H
