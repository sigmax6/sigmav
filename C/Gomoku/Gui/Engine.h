/*
 * MainWindow.h
 *
 *  Created on: 2009-10-27
 *      Author: kos
 */

#ifndef ENGINE_H_
#define ENGINE_H_
#include <QtGui>
#include "../core/gomokuCore.h"
#include "../core/statement.h"

class Engine: public QWidget
{
Q_OBJECT
public:
	Engine(QWidget * parent = 0);
	~Engine();
	void mousePressEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);

private:
	void init();
	void boardArray();
	void drawBackground();
	void drawPath();
	void mouse_pixpos_to_pos();

	void MAN(int x,int y);
	void outputArray();
	void PC();
	void PC(int x,int y);


	QPoint boardPoint[GO_SIZE - 1][GO_SIZE - 1];
	/**
	 * 背景图像相对于pix的位置
	 */
	int startX;
	int startY;
	/*
	 * pix相对于Engine的位置
	 */
	int shiftX;
	int shiftY;
	int mouse_pixx;
	int mouse_pixy;
	int mouse_x;
	int mouse_y;
	pair<int,int> pc;

	QPainterPath pathMan;

	QPainterPath pathPC;

	QBrush brush;
	QPen pen;

 	QImage * imgBackground;

 	/*
 	 * 备注：本想将背景和前景分开管理的，
 	 * 但后来发现用不上，BG留作以后用。
 	 */
 	/*
 	 * 画背景
 	 */
	QPainter * painterBG;

	/*
	 * 画前景
	 */
	QPainter * painterFG;

	/*
	 * 画背景和前景，
	 * 将背景和前景Engine上显示
	 */
	QPainter * painterALL;

	/*
	 * 背景图层
	 */
	QPixmap *pixbg;

	/*
	 * 前景图层
	 */
	QPixmap * pixfg;

	/*
	 * 智能核心
	 */

	GomokuCore core;
};

#endif /* Engine_H_ */
