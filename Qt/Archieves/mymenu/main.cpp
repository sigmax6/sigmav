#include <QtGui/QApplication>
#include "menu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());//加此行以正确显示中文

    menu w;
    w.show();

    return a.exec();
}
