#include <QtGui>
#include <QApplication>
#include "./core/gomokuCore.h"
#include "Gui/MainWindow.h"
#include "Gui/Engine.h"

int main(int argc ,char *argv[])
{
	QApplication a(argc, argv);

	//	Engine  eg;

	MainWindow mw;

	//	GomokuCore go;
	////	go.board[4][5] = '#';
	//	go.outputArray();

	return a.exec();
}
