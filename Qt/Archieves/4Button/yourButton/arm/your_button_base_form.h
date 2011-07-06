/****************************************************************************
** Form interface generated from reading ui file 'your_button_base_form.ui'
**
** Created: Tue Jul 20 09:09:56 2010
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef YOURBUTTONBASEFORM_H
#define YOURBUTTONBASEFORM_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QRadioButton;

class YourButtonBaseForm : public QWidget
{ 
    Q_OBJECT

public:
    YourButtonBaseForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~YourButtonBaseForm();

    QRadioButton* key1RadioButton;
    QRadioButton* key2RadioButton;
    QRadioButton* key3RadioButton;
    QRadioButton* key4RadioButton;
    QRadioButton* key5RadioButton;
    QRadioButton* key6RadioButton;

protected:
    QVBoxLayout* Layout1;
};

#endif // YOURBUTTONBASEFORM_H
