/****************************************************************************
** Form interface generated from reading ui file 'template_base.ui'
**
** Created: Sat Sep 25 20:34:13 2010
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef HELLOBASEFORM_H
#define HELLOBASEFORM_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QPushButton;

class HelloBaseForm : public QWidget
{ 
    Q_OBJECT

public:
    HelloBaseForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~HelloBaseForm();

    QPushButton* dinosauPushButton;
    QLabel* girlPixmapLabel;
    QPushButton* helloPushButton;

};

#endif // HELLOBASEFORM_H
