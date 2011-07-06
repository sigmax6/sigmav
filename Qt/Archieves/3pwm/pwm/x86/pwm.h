#ifndef MYHELLOFORM_H
#define MYHELLOFORM_H
#include "pwm_base.h"
#include <qpixmap.h>

class MyPwmForm : public PwmBaseForm
{ 
	Q_OBJECT
	
	public:
	    	MyPwmForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	    	virtual ~MyPwmForm();
	    	//const QPixmap *eddy;
		int freq;
		//int fd;

	private slots:
	    	void buzzInc();
	    	void buzzDec();
		void buzzON();
	    	void buzzOFF();
};

#endif // MYHELLOFORM_H
