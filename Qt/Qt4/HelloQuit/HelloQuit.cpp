#include <QApplication>
#include <QPushButton>

int main (int argc, char * argv[])
{
    QApplication app(argc,argv);
    QPushButton quitBtn("HelloQuit");
    quitBtn.show();
    QObject::connect(&quitBtn,SIGNAL(clicked()),&app,SLOT(quit()));
    return app.exec();
}

