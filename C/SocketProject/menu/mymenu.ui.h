/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
******************************************************************************/
#include <iostream>
#include <qsocket.h>
 
void mymenu::querypressed()
{
     /*设置dataTable2*/
    dataTable2->setSorting(TRUE);
    QStringList dataTable2_string;
    dataTable2_string<<"seq ASC";
    dataTable2->setSort(dataTable2_string);
    dataTable2->show();
    
    /*连接数据库*/
    QSqlDatabase *DB=QSqlDatabase::addDatabase("QMYSQL3");
    if(!DB){
	qWarning("Failed to connect to driver");
	return;
    }
    DB->setDatabaseName("menu");
    DB->setUserName("root");
    DB->setPassword("19881210");
    DB->setHostName("localhost");
    if(!DB->open()){
	qWarning("Failed to open menu database:"+DB->lastError().driverText());
	qWarning(DB->lastError().databaseText());
	return;
    }
    
    /*创建数据库游标*/
    QSqlCursor *menucursor=new QSqlCursor("mymenu",TRUE,DB);
    QString select="id=";
    select+=dishid->text();
    menucursor->select(select);
    dataTable2->setSqlCursor(menucursor);
    dataTable2->refresh(QDataTable::RefreshColumns);
    /*图片显示*/
    QSqlQuery query;
    QString select1="select photo from mymenu where "+select;
    query.exec(select1);
    if(query.first())
    {
    QString photo1=query.value(0).toString();
    QPixmap pixmap(photo1); //告诉pixmap 我们要显示的图片
    textLabel1->setPixmap(pixmap); //让textLabel1调用/显示pixmap(photo)
    textLabel1->setScaledContents( TRUE ) ; //打开自动平衡收放图片大小的功能
    }
}


void mymenu::orderpressed()
{
    QSocket socket;
    socket.connectToHost("localhost",2000);
    if(socket.state()==QSocket::Connected)
    {
	qWarning("connected");
	QTextStream os(&socket);
	os<<dishid->text()<<"\n";
	dishid->setText("");
	socket.close();
    }
   if(socket.state()==QSocket::Connecting)
       qWarning("connecting");
       //socket.close();
}



