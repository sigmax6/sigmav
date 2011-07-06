/****************************************************************************
** MyHelloForm meta object code from reading C++ file 'template.h'
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

#include "../../template.h"
#include <qmetaobject.h>
#include <qapplication.h>
#ifdef QWS
#include <qobjectdict.h>
#endif



const char *MyHelloForm::className() const
{
    return "MyHelloForm";
}

QMetaObject *MyHelloForm::metaObj = 0;

#ifdef QWS
static class MyHelloForm_metaObj_Unloader {
public:
    ~MyHelloForm_metaObj_Unloader()
    {
         if ( objectDict )
             objectDict->remove( "MyHelloForm" );
    }
} MyHelloForm_metaObj_unloader;
#endif

void MyHelloForm::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(HelloBaseForm::className(), "HelloBaseForm") != 0 )
	badSuperclassWarning("MyHelloForm","HelloBaseForm");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString MyHelloForm::tr(const char* s)
{
    return qApp->translate( "MyHelloForm", s, 0 );
}

QString MyHelloForm::tr(const char* s, const char * c)
{
    return qApp->translate( "MyHelloForm", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* MyHelloForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) HelloBaseForm::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (MyHelloForm::*m1_t0)();
    typedef void (QObject::*om1_t0)();
    typedef void (MyHelloForm::*m1_t1)();
    typedef void (QObject::*om1_t1)();
    m1_t0 v1_0 = &MyHelloForm::showMe;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    m1_t1 v1_1 = &MyHelloForm::sayHello;
    om1_t1 ov1_1 = (om1_t1)v1_1;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(2);
    slot_tbl[0].name = "showMe()";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Private;
    slot_tbl[1].name = "sayHello()";
    slot_tbl[1].ptr = (QMember)ov1_1;
    slot_tbl_access[1] = QMetaData::Private;
    metaObj = QMetaObject::new_metaobject(
	"MyHelloForm", "HelloBaseForm",
	slot_tbl, 2,
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
