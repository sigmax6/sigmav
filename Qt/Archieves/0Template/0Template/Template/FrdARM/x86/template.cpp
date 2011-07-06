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
//-------------------headers of hardware driver-----------------




//==============================================================
//----------------Constructor & Destructor Function-------------
MyHelloForm::MyHelloForm( QWidget* parent, const char* name, WFlags fl)
	    :HelloBaseForm(parent, name, fl)
{
    eddy = new QPixmap("/opt/Qtopia/pics/yana3.JPG");
    connect(helloPushButton,SIGNAL(clicked()),this,SLOT(sayHello()));
    connect(dinosauPushButton,SIGNAL(clicked()),this,SLOT(showMe()));
}

MyHelloForm::~MyHelloForm()
{
    
}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//---------------------User defined function--------------------
void MyHelloForm::sayHello()
{
    girlPixmapLabel->clear();
    girlPixmapLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    girlPixmapLabel->setText( "Hello\n\nI am Pretty Girl" );
    girlPixmapLabel->setAlignment( AlignCenter ); 
}

void MyHelloForm::showMe()
{
    girlPixmapLabel->clear();	
    girlPixmapLabel->setPixmap(*eddy);
}
