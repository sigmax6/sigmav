/****************************************************************************
** Meta object code from reading C++ file 'QespTest.h'
**
** Created: Tue Aug 17 21:08:02 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qespta/QespTest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QespTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QespTest[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      24,    9,    9,    9, 0x08,
      37,    9,    9,    9, 0x08,
      48,    9,    9,    9, 0x08,
      59,    9,    9,    9, 0x08,
      71,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QespTest[] = {
    "QespTest\0\0transmitMsg()\0receiveMsg()\0"
    "appendCR()\0appendLF()\0closePort()\0"
    "openPort()\0"
};

const QMetaObject QespTest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QespTest,
      qt_meta_data_QespTest, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QespTest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QespTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QespTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QespTest))
        return static_cast<void*>(const_cast< QespTest*>(this));
    return QWidget::qt_metacast(_clname);
}

int QespTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: transmitMsg(); break;
        case 1: receiveMsg(); break;
        case 2: appendCR(); break;
        case 3: appendLF(); break;
        case 4: closePort(); break;
        case 5: openPort(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
