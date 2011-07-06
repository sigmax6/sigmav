#ifndef MYPAGE_H
#define MYPAGE_H

#include <QtGui/QWidget>
#include <QLabel>//包含显示名字的label类
#include <QLineEdit>//包含显示信息的控件
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class pdjly : public QWidget
{
    Q_OBJECT

public:
    pdjly(QWidget *parent = 0);
    ~pdjly(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();

public slots:

    void ok_clicked();

};






class psbj : public QWidget
{
    Q_OBJECT

public:
    psbj(QWidget *parent = 0);
    ~psbj(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
        void reflash();
public slots:

    void ok_clicked();
};






class pxglc : public QWidget
{
    Q_OBJECT

public:
    pxglc(QWidget *parent = 0);
    ~pxglc(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};






class psrfssb : public QWidget
{
    Q_OBJECT

public:
    psrfssb(QWidget *parent = 0);
    ~psrfssb(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};






class pzgdxj : public QWidget
{
    Q_OBJECT

public:
    pzgdxj(QWidget *parent = 0);
    ~pzgdxj(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};






class pzlhgr : public QWidget
{
    Q_OBJECT

public:
    pzlhgr(QWidget *parent = 0);
    ~pzlhgr(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};








class plkjc : public QWidget
{
    Q_OBJECT

public:
    plkjc(QWidget *parent = 0);
    ~plkjc(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};






class pdazj : public QWidget
{
    Q_OBJECT

public:
    pdazj(QWidget *parent = 0);
    ~pdazj(){};

    QLabel *namelabel,*conslabel,*introlabel,*stylelabel,*speclabel;
    QLineEdit *nameline,*styleline,*specline;
    QListWidget *introline,*consline;

    QListWidget *ordered;
    void reflash();
public slots:

    void ok_clicked();
};





#endif // MYPAGE_H
