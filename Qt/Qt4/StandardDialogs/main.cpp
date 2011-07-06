#include "StandardDialogs.h"

int main (int argc, char * argv[])
{
    QApplication app(argc,argv);
    StandardDialogs sd=new StandardDialogs();
    sd.show();
    return app.exec();
}

