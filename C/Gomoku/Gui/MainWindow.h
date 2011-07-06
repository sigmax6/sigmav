/*
 * MainWindow.h
 *
 *  Created on: 2009-10-31
 *      Author: kos
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "Engine.h"

class MainWindow: public QMainWindow
{
Q_OBJECT
public:
	MainWindow();
	~MainWindow();
	void createMenus();
	void createActions();
	void createToolBars();

	//	public slots:
	//		void slotNewFile();

private:
	QTextCodec *codec;

	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * aboutMenu;
	QToolBar * fileTool;
	QToolBar * editTool;
	QAction * fileOpenAction;
	QAction * fileNewAction;
	QAction * fileSaveAction;
	QAction * exitAction;
	QAction * copyAction;
	QAction * cutAction;
	QAction * pasteAction;
	QAction * aboutAction;


	QLabel *titleLabel;
	QTextEdit * text;

private:
	Engine *engine;

};

#endif /* MAINWINDOW_H_ */
