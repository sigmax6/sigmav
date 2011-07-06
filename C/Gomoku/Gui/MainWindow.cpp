/*
 * MainWindow.cpp
 *
 *  Created on: 2009-10-31
 *      Author: kos
 */

#include "MainWindow.h"

MainWindow::MainWindow()
{
	setWindowTitle(tr("GomokuMainWindow"));
	engine = new Engine(this);
	titleLabel = new QLabel(tr("Gomoku"));
	setCentralWidget(engine);
	this->show();
}

MainWindow::~MainWindow()
{
	delete engine;
	delete text;
}

void MainWindow::createActions()
{
	fileOpenAction = new QAction(tr("Open"), this);
	fileOpenAction->setShortcut(tr("Ctrl+O"));
	fileOpenAction->setStatusTip(tr("open a file"));

	fileNewAction = new QAction(tr("New"), this);
	fileNewAction->setShortcut(tr("Ctrl+N"));
	fileNewAction->setStatusTip(tr("new file"));
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("File"));
	fileMenu->addAction(fileNewAction);
	fileMenu->addAction(fileOpenAction);

	//	fileMenu->addAction(fileSaveAction);
	//	fileMenu->addAction(exitAction);

	//	editMenu = menuBar()->addMenu(codec->toUnicode("编辑"));
	//	editMenu->addAction(copyAction);
	//	editMenu->addAction(cutAction);
	//	editMenu->addAction(pasteAction);
	//
	//
	//	aboutMenu = menuBar()->addMenu(codec->toUnicode("帮助"));
	//	aboutMenu->addAction(aboutAction);

}
