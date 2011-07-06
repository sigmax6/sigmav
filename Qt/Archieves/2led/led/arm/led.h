#ifndef MYHELLOFORM_H
#define MYHELLOFORM_H
#include "led_base.h"
#include <qpixmap.h>

class MyLEDForm : public LEDBaseForm
{
	Q_OBJECT

	public:
	    MyLEDForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	    virtual ~MyLEDForm();
	    //const QPixmap *eddy;

	private slots:
	    void startSys();
	    void stopSys();
	void OnLED();
	void OffLED();
void blink1();
void blink2();
};

#endif // MYHELLOFORM_H
