/****************************************************************************
** ClientSocket meta object code from reading C++ file 'myserver.h'
**
** Created: Fri Sep 3 19:17:24 2010
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "myserver.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ClientSocket::className() const
{
    return "ClientSocket";
}

QMetaObject *ClientSocket::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ClientSocket( "ClientSocket", &ClientSocket::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ClientSocket::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClientSocket", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ClientSocket::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ClientSocket", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ClientSocket::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSocket::staticMetaObject();
    static const QUMethod slot_0 = {"readClient", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "readClient()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ClientSocket", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ClientSocket.setMetaObject( metaObj );
    return metaObj;
}

void* ClientSocket::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ClientSocket" ) )
	return this;
    return QSocket::qt_cast( clname );
}

bool ClientSocket::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: readClient(); break;
    default:
	return QSocket::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ClientSocket::qt_emit( int _id, QUObject* _o )
{
    return QSocket::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ClientSocket::qt_property( int id, int f, QVariant* v)
{
    return QSocket::qt_property( id, f, v);
}

bool ClientSocket::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *SimpleServer::className() const
{
    return "SimpleServer";
}

QMetaObject *SimpleServer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SimpleServer( "SimpleServer", &SimpleServer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SimpleServer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleServer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SimpleServer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SimpleServer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SimpleServer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QServerSocket::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "ClientSocket", QUParameter::In }
    };
    static const QUMethod signal_0 = {"newConnect", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "newConnect(ClientSocket*)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"SimpleServer", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SimpleServer.setMetaObject( metaObj );
    return metaObj;
}

void* SimpleServer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SimpleServer" ) )
	return this;
    return QServerSocket::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL newConnect
void SimpleServer::newConnect( ClientSocket* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool SimpleServer::qt_invoke( int _id, QUObject* _o )
{
    return QServerSocket::qt_invoke(_id,_o);
}

bool SimpleServer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: newConnect((ClientSocket*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QServerSocket::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool SimpleServer::qt_property( int id, int f, QVariant* v)
{
    return QServerSocket::qt_property( id, f, v);
}

bool SimpleServer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *myserver::className() const
{
    return "myserver";
}

QMetaObject *myserver::metaObj = 0;
static QMetaObjectCleanUp cleanUp_myserver( "myserver", &myserver::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString myserver::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myserver", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString myserver::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "myserver", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* myserver::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QUMethod slot_1 = {"newConnect", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected },
	{ "newConnect()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"myserver", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_myserver.setMetaObject( metaObj );
    return metaObj;
}

void* myserver::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "myserver" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool myserver::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    case 1: newConnect(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool myserver::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool myserver::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool myserver::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
