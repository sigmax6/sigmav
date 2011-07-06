/****************************************************************************
** Form implementation generated from reading ui file 'mymenu.ui'
**
** Created: 六  9月 4 16:14:09 2010
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "mymenu.h"

#include <qvariant.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qdatatable.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "mymenu.ui.h"
extern QSocket socket;
/*
 *  Constructs a mymenu as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
mymenu::mymenu( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "mymenu" );

    dishid = new QLineEdit( this, "dishid" );
    dishid->setGeometry( QRect( 300, 20, 170, 22 ) );

    querybutton = new QPushButton( this, "querybutton" );
    querybutton->setGeometry( QRect( 570, 20, 95, 27 ) );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setGeometry( QRect( 230, 60, 620, 460 ) );

    dataTable2 = new QDataTable( groupBox1, "dataTable2" );
    dataTable2->addColumn( "name", tr( "Name" ) );
    dataTable2->addColumn( "source", tr( "Source" ) );
    dataTable2->addColumn( "intergrent", tr( "Intergrent" ) );
    dataTable2->addColumn( "price", tr( "Price" ) );
    dataTable2->setGeometry( QRect( 0, 40, 620, 80 ) );
    dataTable2->setReadOnly( TRUE );
    dataTable2->setSorting( TRUE );
    dataTable2->setAutoEdit( TRUE );

    textLabel1 = new QLabel( groupBox1, "textLabel1" );
    textLabel1->setGeometry( QRect( 100, 120, 460, 330 ) );
    textLabel1->setPixmap( QPixmap::fromMimeSource( "1.jpg" ) );

    orderbutton = new QPushButton( this, "orderbutton" );
    orderbutton->setGeometry( QRect( 480, 570, 95, 27 ) );

    dataTable1 = new QDataTable( this, "dataTable1" );
    dataTable1->addColumn( "name", tr( "Name" ) );
    dataTable1->setGeometry( QRect( 0, 0, 210, 640 ) );
    dataTable1->setReadOnly( TRUE );
    dataTable1->setSorting( TRUE );
    dataTable1->setAutoEdit( TRUE );

    myconnConnection = QSqlDatabase::database( "myconn" );
    languageChange();
    resize( QSize(866, 648).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( querybutton, SIGNAL( clicked() ), this, SLOT( querypressed() ) );
    connect( orderbutton, SIGNAL( clicked() ), this, SLOT( orderpressed() ) );
    connect(&socket,SIGNAL(connected()),this,SLOT(emitdishid()));
    connect(&socket,SIGNAL(error(int)),this,SLOT(socketError(int)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
mymenu::~mymenu()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Widget polish.  Reimplemented to handle
 *  default data table initialization
 */
void mymenu::polish()
{
    if ( dataTable2 ) {
        QSqlCursor* cursor = dataTable2->sqlCursor();
        if ( !cursor ) {
            cursor = new QSqlCursor( "mymenu", TRUE, myconnConnection );
            if ( dataTable2->isReadOnly() ) 
                cursor->setMode( QSqlCursor::ReadOnly );
            dataTable2->setSqlCursor( cursor, FALSE, TRUE );
        }
        if ( !cursor->isActive() )
            dataTable2->refresh( QDataTable::RefreshAll );
    }
    if ( dataTable1 ) {
        QSqlCursor* cursor = dataTable1->sqlCursor();
        if ( !cursor ) {
            cursor = new QSqlCursor( "mymenu", TRUE, myconnConnection );
            if ( dataTable1->isReadOnly() ) 
                cursor->setMode( QSqlCursor::ReadOnly );
            dataTable1->setSqlCursor( cursor, FALSE, TRUE );
        }
        if ( !cursor->isActive() )
            dataTable1->refresh( QDataTable::RefreshAll );
    }
    QDialog::polish();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mymenu::languageChange()
{
    setCaption( tr( "menu" ) );
    querybutton->setText( tr( "query" ) );
    groupBox1->setTitle( tr( "dish detail" ) );
    dataTable2->setFilter( tr( "id=1" ) );
    textLabel1->setText( QString::null );
    orderbutton->setText( tr( "order" ) );
}

