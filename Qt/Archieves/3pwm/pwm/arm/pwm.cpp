#include "pwm.h"
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qpushbutton.h>
#include <qbutton.h>

#include <qtopia/qpeapplication.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>


#include <sys/ioctl.h>

static int fd;
#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			2

MyPwmForm::MyPwmForm( QWidget* parent, const char* name, WFlags fl)
	    :PwmBaseForm(parent, name, fl)
{
	fd = -1;
	freq = 1000;
	LCDNumber1->setDecMode();
	LCDNumber1->display(freq);

	connect(PushButton1,SIGNAL(clicked()),this,SLOT(buzzInc()));
	connect(PushButton2,SIGNAL(clicked()),this,SLOT(buzzDec()));
	connect(PushButton3,SIGNAL(clicked()),this,SLOT(buzzON()));
	connect(PushButton4,SIGNAL(clicked()),this,SLOT(buzzOFF()));
}

MyPwmForm::~MyPwmForm()
{
	buzzOFF();
}

static void close_buzzer(void)
{
	if (fd >= 0) {
		::ioctl(fd, PWM_IOCTL_STOP);
		::close(fd);
		fd = -1;
	}
}
static void set_buzzer_freq(int freq)
{
    // this IOCTL command is the key to set frequency
    int ret = ::ioctl(fd, PWM_IOCTL_SET_FREQ, freq);
    if(ret < 0) {
	::perror("set the frequency of the buzzer");
	::exit(1);
    }
}

void MyPwmForm::buzzON()
{

    fd = ::open("/dev/pwm", 0);
    if (fd < 0) {
	::perror("open pwm_buzzer device");
	::exit(1);
    }
    set_buzzer_freq(freq);
    LCDNumber2->display(fd);
    // any function exit call will stop the buzzer
    ::atexit(close_buzzer);
}

void MyPwmForm::buzzOFF()
{
    int ret = ::ioctl(fd, PWM_IOCTL_STOP);	//PWM_IOCTL_STOP= 2
    //int ret = ::ioctl(fd, 2);
    LCDNumber2->display(fd);
    if(ret < 0) {
	::perror("stop the buzzer");
	::exit(1);
    }
    ::close(fd);

    //::exit(0);	// exit the program directly
}
void MyPwmForm::buzzInc()
{
    if( freq < 20000 )
        freq += 10;
    LCDNumber1->display(freq);
    
    set_buzzer_freq(freq);
}

void MyPwmForm::buzzDec()
{
    if( freq > 11 )
        freq -= 10 ;
    LCDNumber1->display(freq);
    
    set_buzzer_freq(freq);
}


//-------------------------------------------------------------------------

