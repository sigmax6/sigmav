#ifndef MYHELLOFORM_H
#define MYHELLOFORM_H
#include "adc_base.h"
#include <qpixmap.h>

class MyADCForm : public ADCBaseForm
{ 
    Q_OBJECT
	
    public:
        MyADCForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	    virtual ~MyADCForm();
	    //const QPixmap *eddy;
		int fd;
    private slots:
        void startADC();
	void stopADC();


};

#endif // MYHELLOFORM_H
