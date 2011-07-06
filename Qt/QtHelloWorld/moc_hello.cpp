/****************************************************************************
** hello meta object code from reading C++ file 'hello.h'
**
** Created: Sat Sep 25 20:32:28 2010
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.12   edited 2005-10-27 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "hello.h"
#include <qmetaobject.h>
#include <qapplication.h>
#ifdef QWS
#include <qobjectdict.h>
#endif



const char *hello::className() const
{
    return "hello";
}

QMetaObject *hello::metaObj = 0;

#ifdef QWS
static class hello_metaObj_Unloader {
public:
    ~hello_metaObj_Unloader()
    {
         if ( objectDict )
             objectDict->remove( "hello" );
    }
} hello_metaObj_unloader;
#endif

void hello::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("hello","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString hello::tr(const char* s)
{
    return qApp->translate( "hello", s, 0 );
}

QString hello::tr(const char* s, const char * c)
{
    return qApp->translate( "hello", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* hello::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (hello::*m1_t0)();
    typedef void (QObject::*om1_t0)();
    m1_t0 v1_0 = &hello::user_button;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "user_button()";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"hello", "QWidget",
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
