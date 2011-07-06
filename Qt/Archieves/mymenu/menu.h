#ifndef MENU_H
#define MENU_H

#include <QtGui/QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QTextCodec>//包含中文字符处理类的头文件
#include <QPushButton>
#include <QTextEdit>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "mypage.h"
#include "qextserialport.h"


class menu : public QWidget
{
    Q_OBJECT

public:
    menu(QWidget *parent = 0);
    ~menu();

    QListWidget *list;//定义一个list窗口

    pdjly *wdjly;//定义djly的窗体
    psbj *wsbj;
    pxglc *wxglc;
    psrfssb *wsrfssb;
    pzgdxj *wzgdxj;
    pzlhgr *wzlhgr;
    plkjc *wlkjc;
    pdazj *wdazj;

    QStackedWidget * infowidget;//定义存储类

    Win_QextSerialPort *port;//定义串口

protected slots:
    void pagechanged(int index);
    void sendorder();



};

#endif // MENU_H

