/****************************************************************************
** Meta object code from reading C++ file 'formcreateauthomat_2.h'
**
** Created: Wed 17. May 17:06:01 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../shem/formcreateauthomat_2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'formcreateauthomat_2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FormCreateAuthomat_2[] = {

 // content:
       5,       // revision
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

const QMetaObject FormCreateAuthomat_2::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FormCreateAuthomat_2,
      qt_meta_data_FormCreateAuthomat_2, 0 }
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
        switch (_id) {
        case 0: nextClk(); break;
        case 1: cancelClk(); break;
        case 2: closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 3: resetChanged(); break;
        case 4: tabChanged((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE