#include <QApplication>
#include "Cemara.h"

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);
    Cemara cam;
    if(cam.ShowImage())
      return app.exec();
    else
      return 0;
}
