/****************************************************************************
** Meta object code from reading C++ file 'formselectelements.h'
**
** Created: Fri 2. Jun 07:46:01 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shem/formselectelements.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formselectelements.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormSelectElements[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      37,   33,   19,   19, 0x0a,
      68,   62,   19,   19, 0x0a,
      90,   19,   19,   19, 0x0a,
     100,   19,   19,   19, 0x0a,
     110,   19,   19,   19, 0x0a,
     120,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FormSelectElements[] = {
    "FormSelectElements\0\0editFields()\0__e\0"
    "closeEvent(QCloseEvent*)\0index\0"
    "slotClik(QModelIndex)\0slotAdd()\0"
    "slotDel()\0nextClk()\0cancelClk()\0"
};

void FormSelectElements::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FormSelectElements *_t = static_cast<FormSelectElements *>(_o);
        switch (_id) {
        case 0: _t->editFields(); break;
        case 1: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 2: _t->slotClik((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->slotAdd(); break;
        case 4: _t->slotDel(); break;
        case 5: _t->nextClk(); break;
        case 6: _t->cancelClk(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FormSelectElements::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FormSelectElements::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FormSelectElements,
      qt_meta_data_FormSelectElements, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FormSelectElements::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FormSelectElements::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FormSelectElements::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FormSelectElements))
        return static_cast<void*>(const_cast< FormSelectElements*>(this));
    return QDialog::qt_metacast(_clname);
}

int FormSelectElements::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
