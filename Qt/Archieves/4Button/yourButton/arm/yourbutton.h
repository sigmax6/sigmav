#ifndef YOURBUTTONFORM_H
#define YOURBUTTONFORM_H
#include "your_button_base_form.h"
#include<qtimer.h>

class YourButtonForm : public YourButtonBaseForm
{ 
	Q_OBJECT

	public:
		YourButtonForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
		virtual ~YourButtonForm();
		QTimer *timer;

	private slots:
		void show_button(void);
};

#endif // YOURBUTTONFORM_H
