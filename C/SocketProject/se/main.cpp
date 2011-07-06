#include <qapplication.h>
#include "myserver.h"
QString DishId;
int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    qWarning("program begining!");
    myserver w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
