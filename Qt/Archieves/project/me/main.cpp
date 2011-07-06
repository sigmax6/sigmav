#include <qapplication.h>
#include "mymenu.h"
#include <qmessagebox.h>
#include <qsqldatabase.h>

 bool createConnection()
 {
     QSqlDatabase *db =QSqlDatabase::addDatabase("QMYSQL3");
     db->setHostName("localhost");
     db->setDatabaseName("menu");
     db->setUserName("root");
     db->setPassword("19881210");
     if (!db->open()){
         //db->lastError().showMessage();
         return false;
     }
     return true;
 }



int main( int argc, char ** argv )
{ 
    QApplication a( argc, argv );
     if (!createConnection())
	 return 1;
    mymenu w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
