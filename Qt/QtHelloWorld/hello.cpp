/****************************************************************************
** Form implementation generated from reading ui file 'hello.ui'
**
** Created: Sat Sep 25 20:32:27 2010
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "hello.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a hello which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
hello::hello( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "hello" );
    resize( 592, 480 ); 
    setCaption( tr( "HelloWorldForm" ) );
    setFocusPolicy( QWidget::TabFocus );

    CloseButton = new QPushButton( this, "CloseButton" );
    CloseButton->setGeometry( QRect( 260, 330, 110, 30 ) ); 
    CloseButton->setText( tr( "CloseButton" ) );

    user_text = new QLabel( this, "user_text" );
    user_text->setGeometry( QRect( 190, 190, 261, 81 ) ); 
    user_text->setText( tr( "" ) );

    UserButton = new QPushButton( this, "UserButton" );
    UserButton->setGeometry( QRect( 240, 90, 171, 51 ) ); 
    UserButton->setText( tr( "User Button" ) );

    // signals and slots connections
    connect( UserButton, SIGNAL( clicked() ), this, SLOT( user_button() ) );
    connect( CloseButton, SIGNAL( clicked() ), this, SLOT( close() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
hello::~hello()
{
    // no need to delete child widgets, Qt does it all for us
}

void hello::user_button()
{
    qWarning( "hello::user_button(): Not implemented yet!" );
}

