/****************************************************************************
** Meta object code from reading C++ file 'painter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../zyf SA16006158 graphic/painter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'painter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Painter_t {
    QByteArrayData data[19];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Painter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Painter_t qt_meta_stringdata_Painter = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Painter"
QT_MOC_LITERAL(1, 8, 5), // "clear"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 4), // "undo"
QT_MOC_LITERAL(4, 20, 7), // "enabled"
QT_MOC_LITERAL(5, 28, 8), // "penWidth"
QT_MOC_LITERAL(6, 37, 8), // "penColor"
QT_MOC_LITERAL(7, 46, 4), // "func"
QT_MOC_LITERAL(8, 51, 2), // "ra"
QT_MOC_LITERAL(9, 54, 2), // "rb"
QT_MOC_LITERAL(10, 57, 5), // "angle"
QT_MOC_LITERAL(11, 63, 7), // "vthetax"
QT_MOC_LITERAL(12, 71, 7), // "lthetax"
QT_MOC_LITERAL(13, 79, 7), // "vthetay"
QT_MOC_LITERAL(14, 87, 7), // "lthetay"
QT_MOC_LITERAL(15, 95, 7), // "vthetaz"
QT_MOC_LITERAL(16, 103, 7), // "lthetaz"
QT_MOC_LITERAL(17, 111, 6), // "widget"
QT_MOC_LITERAL(18, 118, 8) // "QWidget*"

    },
    "Painter\0clear\0\0undo\0enabled\0penWidth\0"
    "penColor\0func\0ra\0rb\0angle\0vthetax\0"
    "lthetax\0vthetay\0lthetay\0vthetaz\0lthetaz\0"
    "widget\0QWidget*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Painter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
      14,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x02 /* Public */,
       3,    0,   25,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::Bool, 0x00095103,
       5, QMetaType::Int, 0x00095103,
       6, QMetaType::QColor, 0x00095103,
       7, QMetaType::Int, 0x00095103,
       8, QMetaType::Int, 0x00095103,
       9, QMetaType::Int, 0x00095103,
      10, QMetaType::Int, 0x00095103,
      11, QMetaType::Int, 0x00095103,
      12, QMetaType::Int, 0x00095103,
      13, QMetaType::Int, 0x00095103,
      14, QMetaType::Int, 0x00095103,
      15, QMetaType::Int, 0x00095103,
      16, QMetaType::Int, 0x00095103,
      17, 0x80000000 | 18, 0x0009510b,

       0        // eod
};

void Painter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Painter *_t = static_cast<Painter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clear(); break;
        case 1: _t->undo(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        Painter *_t = static_cast<Painter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isEnabled(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->penWidth(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = _t->penColor(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->func(); break;
        case 4: *reinterpret_cast< int*>(_v) = _t->ra(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->rb(); break;
        case 6: *reinterpret_cast< int*>(_v) = _t->angle(); break;
        case 7: *reinterpret_cast< int*>(_v) = _t->vthetax(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->lthetax(); break;
        case 9: *reinterpret_cast< int*>(_v) = _t->vthetay(); break;
        case 10: *reinterpret_cast< int*>(_v) = _t->lthetay(); break;
        case 11: *reinterpret_cast< int*>(_v) = _t->vthetaz(); break;
        case 12: *reinterpret_cast< int*>(_v) = _t->lthetaz(); break;
        case 13: *reinterpret_cast< QWidget**>(_v) = _t->widget(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        Painter *_t = static_cast<Painter *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setPenWidth(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setPenColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: _t->setFunc(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setRa(*reinterpret_cast< int*>(_v)); break;
        case 5: _t->setRb(*reinterpret_cast< int*>(_v)); break;
        case 6: _t->setAngle(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setVthetax(*reinterpret_cast< int*>(_v)); break;
        case 8: _t->setLthetax(*reinterpret_cast< int*>(_v)); break;
        case 9: _t->setVthetay(*reinterpret_cast< int*>(_v)); break;
        case 10: _t->setLthetay(*reinterpret_cast< int*>(_v)); break;
        case 11: _t->setVthetaz(*reinterpret_cast< int*>(_v)); break;
        case 12: _t->setLthetaz(*reinterpret_cast< int*>(_v)); break;
        case 13: _t->setWidget(*reinterpret_cast< QWidget**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

const QMetaObject Painter::staticMetaObject = {
    { &QQuickPaintedItem::staticMetaObject, qt_meta_stringdata_Painter.data,
      qt_meta_data_Painter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Painter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Painter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Painter.stringdata0))
        return static_cast<void*>(const_cast< Painter*>(this));
    return QQuickPaintedItem::qt_metacast(_clname);
}

int Painter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
