/****************************************************************************
** Form interface generated from reading ui file 'mymenu.ui'
**
** Created: 六  9月 4 10:15:11 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYMENU_H
#define MYMENU_H

#include <qvariant.h>
#include <qdialog.h>
//extern QSocket socket;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSqlDatabase;
class QSqlCursor;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QDataTable;
class QSqlRecord;
class QLabel;

class mymenu : public QDialog
{
    Q_OBJECT

public:
    mymenu( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~mymenu();

    QLineEdit* dishid;
    QPushButton* querybutton;
    QGroupBox* groupBox1;
    QDataTable* dataTable2;
    QLabel* textLabel1;
    QPushButton* orderbutton;
    QDataTable* dataTable1;

    QSqlDatabase* myconnConnection;

public slots:
    virtual void polish();

    virtual void querypressed();
    virtual void orderpressed();
    void emitdishid();
    void socketError(int);
    virtual void languageChange();
   
};

#endif // MYMENU_H
