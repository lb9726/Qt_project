/****************************************************************************
** Meta object code from reading C++ file 'mplayerqprocess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TestQProcessControlMplayer/mplayerqprocess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mplayerqprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MPlayerQProcess_t {
    QByteArrayData data[11];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MPlayerQProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MPlayerQProcess_t qt_meta_stringdata_MPlayerQProcess = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MPlayerQProcess"
QT_MOC_LITERAL(1, 16, 13), // "onStartStatus"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "onOccurError"
QT_MOC_LITERAL(4, 44, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(5, 67, 8), // "pErrorNo"
QT_MOC_LITERAL(6, 76, 10), // "onFinished"
QT_MOC_LITERAL(7, 87, 8), // "exitCode"
QT_MOC_LITERAL(8, 96, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(9, 117, 9), // "onTimeOut"
QT_MOC_LITERAL(10, 127, 25) // "onReadyReadStandardOutput"

    },
    "MPlayerQProcess\0onStartStatus\0\0"
    "onOccurError\0QProcess::ProcessError\0"
    "pErrorNo\0onFinished\0exitCode\0"
    "QProcess::ExitStatus\0onTimeOut\0"
    "onReadyReadStandardOutput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MPlayerQProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    1,   40,    2, 0x0a /* Public */,
       6,    2,   43,    2, 0x0a /* Public */,
       9,    0,   48,    2, 0x0a /* Public */,
      10,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    7,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MPlayerQProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MPlayerQProcess *_t = static_cast<MPlayerQProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onStartStatus(); break;
        case 1: _t->onOccurError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 2: _t->onFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 3: _t->onTimeOut(); break;
        case 4: _t->onReadyReadStandardOutput(); break;
        default: ;
        }
    }
}

const QMetaObject MPlayerQProcess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MPlayerQProcess.data,
      qt_meta_data_MPlayerQProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MPlayerQProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MPlayerQProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MPlayerQProcess.stringdata0))
        return static_cast<void*>(const_cast< MPlayerQProcess*>(this));
    return QObject::qt_metacast(_clname);
}

int MPlayerQProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
