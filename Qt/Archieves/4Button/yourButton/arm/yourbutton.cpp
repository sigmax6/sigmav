#include "yourbutton.h"
// Coded by sigmax6 201007

#include <qradiobutton.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

YourButtonForm::YourButtonForm( QWidget* parent, const char* name, WFlags fl):YourButtonBaseForm(parent, name, fl)
{
	timer = new QTimer(this);
	connect (timer, SIGNAL(timeout()), this, SLOT(show_button()));
	timer -> start(10);
}

YourButtonForm::~YourButtonForm()
{

}

void YourButtonForm::show_button()
{
  int fd, i;
  char current_buttons[6];

  fd = ::open("/dev/utu2440-buttons", 0);

  ::read(fd, current_buttons, 6);

  key1RadioButton -> setChecked (current_buttons[0] & 1) ;
  key2RadioButton -> setChecked (current_buttons[1] & 1) ;
  key3RadioButton -> setChecked (current_buttons[2] & 1) ;
  key4RadioButton -> setChecked (current_buttons[3] & 1) ;
  key5RadioButton -> setChecked (current_buttons[4] & 1) ;
  key6RadioButton -> setChecked (current_buttons[5] & 1) ;

  ::close(fd);

}
