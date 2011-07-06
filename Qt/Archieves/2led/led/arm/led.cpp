#include "led.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>

#include <qtopia/qpeapplication.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

MyLEDForm::MyLEDForm( QWidget* parent, const char* name, WFlags fl)
	    :LEDBaseForm(parent, name, fl)
{
    connect(PushButton1,SIGNAL(clicked()),this,SLOT(stopSys()));
    connect(PushButton2,SIGNAL(clicked()),this,SLOT(startSys()));
    connect(PushButton3,SIGNAL(clicked()),this,SLOT(OnLED()));
    connect(PushButton4,SIGNAL(clicked()),this,SLOT(OffLED()));
    connect(PushButton5,SIGNAL(clicked()),this,SLOT(blink1()));
    connect(PushButton6,SIGNAL(clicked()),this,SLOT(blink2()));
}

MyLEDForm::~MyLEDForm()
{
    system("/etc/rc.d/init.d/leds start");
}

void MyLEDForm::stopSys()
{
    system("/etc/rc.d/init.d/leds stop");
    TextLabel1->setText("System Led is Stoped");
}

void MyLEDForm::startSys()
{
    system("/etc/rc.d/init.d/leds start");
    TextLabel1->setText("System Led is Started");
}void MyLEDForm::OnLED()
{
    int iData=SpinBox1->text().toFloat();

    int fd = ::open("/dev/leds", 0);
    ::ioctl(fd, 1, iData);
    ::close(fd); // 关闭设备
}
void MyLEDForm::OffLED()
{
    int iData=SpinBox1->text().toFloat();

    int fd = ::open("/dev/leds", 0);
    ::ioctl(fd, 0, iData);
    ::close(fd); // 关闭设备
}
void MyLEDForm::blink1()
{
    volatile int k;

    int fd=::open("/dev/leds", 0);  // 打开设备
    for (k=0; k<4; k++)
    {
      ::ioctl(fd, 1, k);      
	usleep(500000);
      ::ioctl(fd, 0, k);
	usleep(500000);
    }
    ::close(fd); // 关闭设备
}
void MyLEDForm::blink2()
{
    // transfer codes from the demostration.
    // " :: "is added before each function.
    int fd = ::open("/dev/leds0", 0);
    if (fd < 0) {
	fd = ::open("/dev/leds", 0);
    }
    if (fd < 0) {
	::perror("open device leds");
	::exit(1);
    }
    for (int k=0; k<4; k++)
    {
      	::ioctl(fd, 0, 3-k);      
	usleep(500000);
      	::ioctl(fd, 1, 3-k);
	usleep(500000);
    }
    ::close(fd);
}


