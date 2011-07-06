#include <QApplication>
#include <QPushButton>
int main (int argc, char * argv[])
{
    QApplication app(argc,argv);
    QPushButton * helloButton = new QPushButton("Hello Button");
    QObject::connect(helloButton,SIGNAL(clicked()),&app,SLOT(quit()));
    helloButton->show();
    return app.exec();
}

