#ifndef STANDARDDIALOGS_H
#define STANDARDDIALOGS_H
#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>

class StandardDialogs : public QDialog
{
        Q_OBJECT
public:
        StandardDialogs(QWidget *parent=0,Qt::WindowFlags f=0);
        ~StandardDialogs();
public:
        QGridLayout * layout;
        QPushButton * fileButton;
        QPushButton * colorButton;
        QPushButton * fontButton;
        QLineEdit * fileLineEdit;
        QLineEdit * fontLineEdit;
        QFrame * colorFrame;

public slots:
        void slotOpenFileDlg();
        void slotOpenColorDlg();
        void slotOpenFontDlg();
};
#endif
