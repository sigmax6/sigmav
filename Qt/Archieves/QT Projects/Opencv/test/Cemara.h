#ifndef CEMARA_H
#define CEMARA_H

#include <QWidget>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <QVector>
#include <QtGui>
#include <cstring>
#include <iostream>
using namespace std;

class Cemara: public QWidget
{
    Q_OBJECT
private:
      QImage image;
public:
       Cemara(QWidget * parent=0);
       QImage IplImageToQImage(const IplImage * iplImage,double mini, double maxi);
       bool ShowImage();
protected:
       void paintEvent(QPaintEvent * event);
};
#endif
