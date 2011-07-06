#include "StandardDialogs.h"

StandardDialogs::StandardDialogs(QWidget * parent,Qt::WindowFlags f)
:QDialog(parent,f)
{
    setWindowTitle("StandardDialogs");
    layout=new QGridLayout(this);
    fileButton = new QPushButton;
    fileButton->setText(tr("File Dialog"));
    colorButton = new QPushButton;
    colorButton->setText(tr("Color Dialog"));
    fontButton = new QPushButton;
    fontButton->setText(tr("Font Dialog"));

    fileLineEdit=new QLineEdit;
    colorFrame = new QFrame;
    colorFrame->setFrameShape(QFrame::Box);
    colorFrame->setAutoFillBackground(true);
    fontLineEdit= new LineEdit;
    fontLineEdit->setText(tr("Hello World"));

    layout->addWidget(fileButton,0,0);
    layout->addWidget(fileLineEdit,0,1);
    layout->addWidget(colorButton,1,0);
    layout->addWidget(colorFrame,1,1);
    layout->addWidget(fontButton,2,0);
    layout->addWidget(fontLineEdit,2,1);
    layout->setMargin(15);
    layout->setSpacing(10);

    connect(fileButton,SIGNAL(clicked()),this,SLOT(slotOpenFileDlg()));
    connect(colorButton,SIGNAL(clicked()),this,SLOT(slotOpenColorDlg()));
    connect(fontButton,SIGNAL(clicked),this,SLOT(slotOpenFontDlg()));

}

void StandardDialogs::slotOpenFileDlg()
{
    QString s=QFileDialog::getOpenFileName
        (this,"open file dialog","/","C++ files(*.cpp);;C files(*.c);;Head files(*.h)");
    lineEditFile->setText(s.toAscii());
}

void StandardDialogs::slotOpenColorDlg()
{
    QColor color=QColorDialog::getColor(Q::blue);

    if(color.isValid())
    {
        colorFrame->setPalette(QPalette(color));
    }
}

void StandardDialogs::slotOpenFontDlg()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok);

    if(ok)
    {
        fontLineEdit->setFont(font);
    }
}

