/**********************************************************
Author		: Lan Xin , xinlan_al@yahoo.com
Date		: 2010.01.12
Version		: V0.1
Discription	: template.h

This program is to demostrate how to -----

***********************************************************/
#ifndef MYHELLOFORM_H
#define MYHELLOFORM_H
#include "template_base.h"
//-------------------headers of hardware driver-----------------
#define BUFLEN 255
//-------------------headers of hardware driver-----------------
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

class MyHelloForm : public HelloBaseForm
{ 
    Q_OBJECT
	
    public:		// Constructor and Destructor
    MyHelloForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    virtual ~MyHelloForm();
    
    /********************************************************************/   
    public:		// User defined public global variable
        const QPixmap *eddy;
        char* charTmp;
	struct sockaddr_in peeraddr, localaddr;
	int sockfd;
	char recmsg[BUFLEN+1];
	int socklen, n;
        QString strDestIP,strSrcIP;
        QString strBuffer;
        char sendmsg[5];
        QTimer *timer1;

    private slots:      // User defined private global variable
        void UdpSend();
        void UdpRecv();
};

#endif // MYHELLOFORM_H
