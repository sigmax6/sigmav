/****************************************************************************
** HelloBaseForm meta object code from reading C++ file 'template_base.h'
**
** Created: Sat Sep 25 20:34:17 2010
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.12   edited 2005-10-27 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "../../.ui/release-shared/template_base.h"
#include <qmetaobject.h>
#include <qapplication.h>
#ifdef QWS
#include <qobjectdict.h>
#endif



const char *HelloBaseForm::className() const
{
    return "HelloBaseForm";
}

QMetaObject *HelloBaseForm::metaObj = 0;

#ifdef QWS
static class HelloBaseForm_metaObj_Unloader {
public:
    ~HelloBaseForm_metaObj_Unloader()
    {
         if ( objectDict )
             objectDict->remove( "HelloBaseForm" );
    }
} HelloBaseForm_metaObj_unloader;
#endif

void HelloBaseForm::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("HelloBaseForm","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString HelloBaseForm::tr(const char* s)
{
    return qApp->translate( "HelloBaseForm", s, 0 );
}

QString HelloBaseForm::tr(const char* s, const char * c)
{
    return qApp->translate( "HelloBaseForm", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* HelloBaseForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    QMetaData::Access *slot_tbl_access = 0;
    metaObj = QMetaObject::new_metaobject(
	"HelloBaseForm", "QWidget",
	0, 0,
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
