/****************************************************************************
** Meta object code from reading C++ file 'gomoku_bak.h'
**
** Created: Tue Oct 27 14:09:38 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gomoku_bak.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gomoku_bak.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Gomoku_bak[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_Gomoku_bak[] = {
    "Gomoku_bak\0"
};

const QMetaObject Gomoku_bak::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Gomoku_bak,
      qt_meta_data_Gomoku_bak, 0 }
};

const QMetaObject *Gomoku_bak::metaObject() const
{
    return &staticMetaObject;
}

void *Gomoku_bak::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gomoku_bak))
        return static_cast<void*>(const_cast< Gomoku_bak*>(this));
    return QWidget::qt_metacast(_clname);
}

int Gomoku_bak::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
