/****************************************************************************
** Meta object code from reading C++ file 'cocoseditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cocoseditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cocoseditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CocosEditor_t {
    QByteArrayData data[9];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CocosEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CocosEditor_t qt_meta_stringdata_CocosEditor = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CocosEditor"
QT_MOC_LITERAL(1, 12, 10), // "newProject"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "loadModel"
QT_MOC_LITERAL(4, 34, 12), // "loadParticle"
QT_MOC_LITERAL(5, 47, 11), // "openProject"
QT_MOC_LITERAL(6, 59, 11), // "saveProject"
QT_MOC_LITERAL(7, 71, 6), // "saveAs"
QT_MOC_LITERAL(8, 78, 5) // "about"

    },
    "CocosEditor\0newProject\0\0loadModel\0"
    "loadParticle\0openProject\0saveProject\0"
    "saveAs\0about"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CocosEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,

       0        // eod
};

void CocosEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CocosEditor *_t = static_cast<CocosEditor *>(_o);
        switch (_id) {
        case 0: _t->newProject(); break;
        case 1: _t->loadModel(); break;
        case 2: _t->loadParticle(); break;
        case 3: _t->openProject(); break;
        case 4: { bool _r = _t->saveProject();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->saveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->about(); break;
        default: ;
        }
    }
}

const QMetaObject CocosEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CocosEditor.data,
      qt_meta_data_CocosEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CocosEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CocosEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CocosEditor.stringdata))
        return static_cast<void*>(const_cast< CocosEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CocosEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
