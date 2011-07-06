/*
 * MainWindow.cpp
 *
 *  Created on: 2009-10-27
 *      Author: kos
 */

#include "Engine.h"
#include <iostream>
using namespace std;

Engine::Engine(QWidget * parent) :
	QWidget(parent)
{
	init();
//	this->show();
}

Engine::~Engine()
{
	delete pixbg;
	delete pixfg;
	delete imgBackground;
	delete painterFG;
	delete painterBG;
	delete painterALL;
	delete imgBackground;

}
void Engine::init()
{
	startX = 80;
	startY = 40;
	shiftX = 20;
	shiftY = 16;
	mouse_x = -1;
	mouse_y = -1;
	boardArray();

	setWindowTitle("Gomoku");
	setAutoFillBackground(true);
	setMinimumSize(1000, 700);

	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::gray);
	setPalette(palette);

	pixbg = new QPixmap(900, 700);
	pixfg = new QPixmap(900, 700);

	painterBG = new QPainter(pixbg);
	painterFG = new QPainter(pixfg);

	pixbg->fill(Qt::white);
	pixfg->fill(Qt::white);

	imgBackground = new QImage("./image/goBoard.png");
	drawBackground();
	//	PC();
	drawPath();
	update();
}
void Engine::drawBackground()
{
	painterBG->begin(pixbg);
	painterBG->drawImage(QPoint(startX, startY), *imgBackground);
	painterBG->end();
}
void Engine::drawPath()
{
	painterBG->begin(pixbg);

	//画人
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::red);
	QBrush MAN_brush(Qt::red);
	/*
	 * 为什么下面这样不行
	 */
	//	brush.setColor(Qt::red);
	update();
	painterBG->setPen(pen);
	brush = MAN_brush;
	painterBG->setBrush(brush);
	painterBG->drawPath(pathMan);
	painterBG->fillPath(pathMan, brush);
	painterBG->end();
	update();

	//PC
	painterBG->begin(pixbg);
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::blue);
	QBrush PC_brush(Qt::blue);
	brush = PC_brush;

	update();

	painterBG->setPen(pen);
	painterBG->drawPath(pathPC);
	painterBG->fillPath(pathPC, brush);
	update();

	painterBG->end();

}

void Engine::paintEvent(QPaintEvent *)
{

	//	pixfg = pixbg;
	//	painterALL->drawPixmap(QPoint(shiftX, shiftY), *pixfg);
	painterALL = new QPainter(this);
	painterALL->drawPixmap(QPoint(shiftX, shiftY), *pixbg);

}

void Engine::boardArray()
{
	QPoint firstPoint(startX + 42, startY + 42);
	boardPoint[0][0] = firstPoint;
	for (int i = 0; i < GO_SIZE - 1; i++)
	{
		for (int j = 0; j < GO_SIZE - 1; j++)
		{
			boardPoint[i][j] = boardPoint[0][0] + QPoint(37 * i, 37 * j);
		}
	}
}

/*
 * 这部分还有点小bug，但不管它了
 */

void Engine::mouse_pixpos_to_pos()
{
	int tempMouse_pixx = mouse_pixx - (startX + shiftX + 42);
	int tempMouse_pixy = mouse_pixy - (startY + shiftY + 42);
	int tempMouse_x1 = tempMouse_pixx / 37;
	int tempMouse_x2 = tempMouse_pixx % 37;
	int tempMouse_y1 = tempMouse_pixy / 37;
	int tempMouse_y2 = tempMouse_pixy % 37;

	if (tempMouse_x2 >= -10 && tempMouse_x2 <= 15)
	{
		mouse_x = tempMouse_x1;
	}
	else if (tempMouse_x2 >= 22 && tempMouse_x2 <= 36 && tempMouse_x1 < GO_SIZE)
	{
		mouse_x = tempMouse_x1 + 1;
	}
	else
	{
		mouse_x = -1;
	}

	if (tempMouse_y2 >= -10 && tempMouse_y2 <= 15)
	{
		mouse_y = tempMouse_y1;
	}
	else if (tempMouse_y2 >= 22 && tempMouse_y2 <= 36 && tempMouse_y1 < GO_SIZE)
	{
		mouse_y = tempMouse_y1 + 1;
	}
	else
	{
		mouse_y = -1;
	}
	//	cout << "相对于core棋盘的位置" << endl;

}

void Engine::mousePressEvent(QMouseEvent * event)
{
	mouse_x = -1;
	mouse_y = -1;

	mouse_pixx = event->x();
	mouse_pixy = event->y();
	mouse_pixpos_to_pos();
	cout << "mouse_x = " << mouse_x << "," << "mouse_y = " << mouse_y << endl;
	cout << core.board[mouse_x][mouse_y] << endl;
	cout << "mouse_x + 1 = " << mouse_x + 1 << "," << "mouse_y +1 = "
			<< mouse_y + 1 << endl;
	cout << core.board[mouse_x + 1][mouse_y + 1] << endl;

	if (mouse_x >= 0 && mouse_x < 15 && mouse_y >= 0 && mouse_y < 15
			&& (core.board[mouse_y + 1][mouse_x + 1] == ' '))
	{
		MAN(mouse_x, mouse_y);
		PC();
		drawPath();
		outputArray();
	}
}

void Engine::MAN(int x ,int y)
{
	pathMan.addEllipse(boardPoint[x][y], GO_RADIUS, GO_RADIUS);
	core.board[y + 1][x + 1] = '#';
	drawPath();
}

void Engine::outputArray()
{
	for (int i = 0; i < GO_SIZE; ++i)
	{
		for (int j = 0; j < GO_SIZE; j++)
		{
			cout << core.board[i][j] << " ";
		}
		cout << endl;
	}
}

void Engine::PC()
{
	pc = core.PC();
	pathPC.addEllipse(boardPoint[pc.second - 1][pc.first - 1], GO_RADIUS,
			GO_RADIUS);
	update();
}
