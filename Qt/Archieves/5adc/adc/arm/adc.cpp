#include "adc.h"
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlcdnumber.h>
/*
#include <qtopia/qpeapplication.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>

MyADCForm::MyADCForm( QWidget* parent, const char* name, WFlags fl)
	    :ADCBaseForm(parent, name, fl)
{
fd=0;
    connect(PushButton1,SIGNAL(clicked()),this,SLOT(startADC()));
connect(PushButton1,SIGNAL(clicked()),this,SLOT(stopADC()));

}

MyADCForm::~MyADCForm()
{
	::close(fd);
}



void MyADCForm::startADC()
{
    //TextLabel1->setText("System Led is Started");


	fd = ::open("/dev/adc", 0);
	if (fd < 0) {
		::exit(1);
	}
	for(int num=0;num<20;num++) {
		char buffer[30];
		int len = ::read(fd, buffer, sizeof buffer -1);
		if (len > 0) {
			buffer[len] = '\0';
			int value = -1;
			sscanf(buffer, "%d", &value);
			//printf("ADC Value: %d\n", value);
			LCDNumber1->display(value);

		} else	::exit(1);
		usleep(500* 1000);
		LCDNumber2->display(num);
	}
	
	::close(fd);
}void MyADCForm::stopADC()
{
::close(fd);
}

