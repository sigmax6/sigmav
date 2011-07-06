/**********************************************************
Author		: Lan Xin , xinlan_al@yahoo.com
Date		: 2010.01.12
Version		: V0.1
Discription	: template.cpp

This program is to demostrate how to -----

***********************************************************/

//-----------------------headers of Qtopia----------------------
#include "template.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qgroupbox.h>
#include <qtimer.h>
#include <qstring.h>
#include <qlcdnumber.h>
#include <qlineedit.h>
#include <qtimer.h>
//-------------------headers of hardware driver-----------------

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>


//==============================================================
//----------------Constructor & Destructor Function-------------
MyHelloForm::MyHelloForm( QWidget* parent, const char* name, WFlags fl)
	    :HelloBaseForm(parent, name, fl)
{
    sendmsg[0] = 'a';
    sendmsg[1] = 'b';
    sendmsg[2] = 'c';

    strDestIP="192.168.1.230";
    strSrcIP ="192.168.1.108";

 
    eddy = new QPixmap("/opt/Qtopia/pics/yana3.JPG");
    connect(UdpSendPushButton,SIGNAL(clicked()),this,SLOT(UdpSend()));
    connect(UdpRecvPushButton,SIGNAL(clicked()),this,SLOT(UdpRecv()));
    //timer1 = new QTimer(this);  
    //connect (timer1, SIGNAL(timeout()), this, SLOT(UdpRecv()));
    //timer1 -> start(100);	
    //===================================================================
    //===================================================================
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0){        
	printf("socket creating err in udptalk\n");
	exit(1);
    }
    LCDNumber1->display(sockfd);
 
    //-------------------------------------------------------------------
    socklen = sizeof(struct sockaddr_in);
    memset(&peeraddr, 0, socklen);
    peeraddr.sin_family=AF_INET;
    peeraddr.sin_port=htons(2000);
    if(inet_pton(AF_INET, strDestIP, &peeraddr.sin_addr)<=0){
        printf("Wrong dest IP address!\n");
        exit(0);
	PixmapLabel1->setText( "aae" );
    }
    //-------------------------------------------------------------------
    memset(&localaddr, 0, socklen);
    localaddr.sin_family=AF_INET;
    if(inet_pton(AF_INET, strSrcIP, &localaddr.sin_addr)<=0){
        printf("Wrong source IP address!\n");
        exit(0);
	PixmapLabel1->setText( "aae" );
    }
    localaddr.sin_port=htons(2000);
    //-------------------------------------------------------------------

    //===================================================================
    if(bind(sockfd, (struct sockaddr*)&localaddr, socklen)<0){	
        printf("bind local address err in al udptalk!\n");
        PixmapLabel1->setText( "bind local address err in al udptalk!\n" );
        exit(2);
    }

    //===================================================================
/*    if(fgets(recmsg, BUFLEN, stdin) == NULL) 
   {
        PixmapLabel1->setText( "fgets(recmsg, BUFLEN, stdin) == NULL\n" );
        //exit(0);
    }
    if(sendto(sockfd, recmsg, strlen(recmsg), 0, (struct sockaddr*)&peeraddr, socklen)<0){
            printf("sendto err in udptalk!\n");
            exit(3);
    }
*/
    //===================================================================
}

MyHelloForm::~MyHelloForm()
{
    close(sockfd);    
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//---------------------User defined function--------------------
void MyHelloForm::UdpSend() 
{
    PixmapLabel1->clear();
    PixmapLabel1->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    PixmapLabel1->setText( "UDP is sending Message" );
    PixmapLabel1->setAlignment( AlignCenter ); 



    strBuffer=LineEdit1->text();

    //if(fgets(recmsg, BUFLEN, stdin) == NULL) exit(0);
    if(sendto(sockfd, strBuffer, strlen(strBuffer), 0, (struct sockaddr*)&peeraddr, socklen)<0){
        printf("sendto err in udptalk!\n");
        PixmapLabel1->setText( "sendto err in udptalk!\n" );
        exit(3);
    }

}

void MyHelloForm::UdpRecv()
{
    PixmapLabel1->clear();	
    //PixmapLabel1->setPixmap(*eddy);
    
    /*recv&send message loop*/
/*    n = recvfrom(sockfd, recmsg, BUFLEN, 0, (struct sockaddr*)&peeraddr, (socklen_t*)&socklen);
    if(n<0){
        printf("recvfrom err in udptalk!\n");
        PixmapLabel1->setText( "recvfrom err in udptalk!\n" );
        //exit(4);
    }else{
        recmsg[n]=0;
        printf("peer:%s", recmsg);
        PixmapLabel1->setText( recmsg );	
    }
*/
    recvfrom(sockfd, recmsg, BUFLEN, 0, (struct sockaddr*)&peeraddr, (socklen_t*)&socklen);
    PixmapLabel1->setText( recmsg );

    /*if(n<0){
        printf("recvfrom err in udptalk!\n");
        exit(4);
    }else{
        recmsg[n]=0;
        printf("peer:%s", recmsg);	
    }
*/

}
