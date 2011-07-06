/****************************************************************************
** Form interface generated from reading ui file 'myserver.ui'
**
** Created: 四  9月 2 17:33:33 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYSERVER_H
#define MYSERVER_H

#include <qvariant.h>
#include <qdialog.h>
#include <qsocket.h>
#include <qserversocket.h>
#include <qapplication.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextstream.h>
#include <stdlib.h>
#include <iostream>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QTextBrowser;
class myserver;
extern QString DishId;/*全局变量，用于存储从client端发送过来的数据*/
/*the ClientSocket class provides a socket that is connected with a client.For every chient that connects to the server,the server creates a new instance of this class.*/
class ClientSocket:public QSocket
{
    Q_OBJECT
public:
	ClientSocket(int sock,QObject *parent=0,const char *name=0):
		QSocket(parent,name)
	{
		connect(this,SIGNAL(readyRead()),SLOT(readClient()));
		setSocket(sock);
	}
	~ClientSocket()
	{
	}
public slots:
	void readClient()
	{
		QTextStream ts(this);
		while(canReadLine())
		{
			DishId=ts.readLine()+"\n";
		}
		//cout>>DishId>>endl;
	}
};

/*the SimpleServer class handles new connections to the server .For every client that connects,it creates a new ClientSocket--that instance is now responsible for the communication with that client.*/
class SimpleServer:public QServerSocket
{
    Q_OBJECT
public:
	SimpleServer(QObject* parent=0):QServerSocket(2000,1,parent)
	{
		if(!ok())
		{
			qWarning("Failed to bind to port 2000");
			exit(1);
		}
		else
			qWarning("success to bind to port 2000");
	}
	~SimpleServer()
	{
	}
	void newConnection(int socket)
	{
		ClientSocket *s=new ClientSocket(socket,this);
		emit newConnect(s);
	}
signals:
	void newConnect(ClientSocket*);
};

class myserver : public QDialog
{
    Q_OBJECT

public:
    myserver( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~myserver();

    QGroupBox* groupBox1;
    QTextBrowser* textBrowser1;

private:
	

protected slots:
    virtual void languageChange();
    void newConnect();
};


#endif // MYSERVER_H
