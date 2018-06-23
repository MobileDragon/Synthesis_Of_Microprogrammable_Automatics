/****************************************************************************
** Meta object code from reading C++ file 'formcreateauthomat_2.h'
**
** Created: Tue 16. May 00:40:40 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shem/formcreateauthomat_2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formcreateauthomat_2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormCreateAuthomat_2[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      32,   21,   21,   21, 0x08,
      48,   44,   21,   21, 0x08,
      73,   21,   21,   21, 0x08,
      93,   88,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FormCreateAuthomat_2[] = {
    "FormCreateAuthomat_2\0\0nextClk()\0"
    "cancelClk()\0__e\0closeEvent(QCloseEvent*)\0"
    "resetChanged()\0item\0tabChanged(QStandardItem*)\0"
};

void FormCreateAuthomat_2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FormCreateAuthomat_2 *_t = static_cast<FormCreateAuthomat_2 *>(_o);
        switch (_id) {
        case 0: _t->nextClk(); break;
        case 1: _t->cancelClk(); break;
        case 2: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 3: _t->resetChanged(); break;
        case 4: _t->tabChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FormCreateAuthomat_2::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FormCreateAuthomat_2::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FormCreateAuthomat_2,
      qt_meta_data_FormCreateAuthomat_2, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FormCreateAuthomat_2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FormCreateAuthomat_2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FormCreateAuthomat_2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FormCreateAuthomat_2))
        return static_cast<void*>(const_cast< FormCreateAuthomat_2*>(this));
    return QDialog::qt_metacast(_clname);
}

int FormCreateAuthomat_2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
