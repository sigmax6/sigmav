/****************************************************************************
** Form implementation generated from reading ui file 'myserver.ui'
**
** Created: 四  9月 2 17:34:09 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "myserver.h"
#include <qvariant.h>
#include <qgroupbox.h>
#include <qtextbrowser.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
extern QString DishId;
/*
 *  Constructs a myserver as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
myserver::myserver( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "myserver" );
    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setGeometry( QRect( 50, 70, 710, 510 ) );

    textBrowser1 = new QTextBrowser( groupBox1, "textBrowser1" );
    textBrowser1->setGeometry( QRect( 40, 50, 640, 400 ) );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    //qWaring("constructing server");
    SimpleServer *server=new SimpleServer(this);
    connect(server,SIGNAL(newConnect(ClientSocket*)),SLOT(newConnect()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
myserver::~myserver()
{
    // no need to delete child widgets, Qt does it all for us
}
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void myserver::languageChange()
{
    setCaption( tr( "my server" ) );
    groupBox1->setTitle( tr( "dish ordered detail" ) );
}
void myserver::newConnect()
{
    qWarning(DishId);
    textBrowser1->setSource(DishId);
    DishId="";
}

