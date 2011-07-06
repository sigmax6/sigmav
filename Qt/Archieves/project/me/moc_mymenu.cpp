/****************************************************************************
** mymenu meta object code from reading C++ file 'mymenu.h'
**
** Created: Sat Sep 4 16:20:14 2010
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mymenu.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *mymenu::className() const
{
    return "mymenu";
}

QMetaObject *mymenu::metaObj = 0;
static QMetaObjectCleanUp cleanUp_mymenu( "mymenu", &mymenu::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString mymenu::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mymenu", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString mymenu::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mymenu", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* mymenu::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"polish", 0, 0 };
    static const QUMethod slot_1 = {"querypressed", 0, 0 };
    static const QUMethod slot_2 = {"orderpressed", 0, 0 };
    static const QUMethod slot_3 = {"emitdishid", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"socketError", 1, param_slot_4 };
    static const QUMethod slot_5 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "polish()", &slot_0, QMetaData::Public },
	{ "querypressed()", &slot_1, QMetaData::Public },
	{ "orderpressed()", &slot_2, QMetaData::Public },
	{ "emitdishid()", &slot_3, QMetaData::Public },
	{ "socketError(int)", &slot_4, QMetaData::Public },
	{ "languageChange()", &slot_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"mymenu", parentObject,
	slot_tbl, 6,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_mymenu.setMetaObject( metaObj );
    return metaObj;
}

void* mymenu::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "mymenu" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool mymenu::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: polish(); break;
    case 1: querypressed(); break;
    case 2: orderpressed(); break;
    case 3: emitdishid(); break;
    case 4: socketError((int)static_QUType_int.get(_o+1)); break;
    case 5: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool mymenu::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool mymenu::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool mymenu::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
