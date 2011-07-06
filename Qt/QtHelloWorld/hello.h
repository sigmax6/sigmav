/****************************************************************************
** Form interface generated from reading ui file 'hello.ui'
**
** Created: Sat Sep 25 20:32:26 2010
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef HELLO_H
#define HELLO_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QPushButton;

class hello : public QWidget
{ 
    Q_OBJECT

public:
    hello( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~hello();

    QPushButton* CloseButton;
    QLabel* user_text;
    QPushButton* UserButton;

public slots:
    virtual void user_button();

};

#endif // HELLO_H
