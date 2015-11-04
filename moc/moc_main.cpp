/****************************************************************************
** Meta object code from reading C++ file 'main.h'
**
** Created: Fri Nov 21 10:44:36 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Serveur[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,    8,    8,    8, 0x08,
      71,   58,   50,    8, 0x08,
     101,   93,   50,    8, 0x28,
     115,   58,   50,    8, 0x08,
     147,   93,   50,    8, 0x28,
     171,   93,   50,    8, 0x08,
     188,   93,   50,    8, 0x08,
     204,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Serveur[] = {
    "Serveur\0\0texte\0donneesRecues(QString)\0"
    "connexion()\0QString\0requete,lang\0"
    "lemm(QString,QString)\0requete\0"
    "lemm(QString)\0analyse_morpho(QString,QString)\0"
    "analyse_morpho(QString)\0flexion(QString)\0"
    "mesure(QString)\0exec()\0"
};

const QMetaObject Serveur::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Serveur,
      qt_meta_data_Serveur, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Serveur::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Serveur::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Serveur::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Serveur))
        return static_cast<void*>(const_cast< Serveur*>(this));
    return QObject::qt_metacast(_clname);
}

int Serveur::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: donneesRecues((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: connexion(); break;
        case 2: { QString _r = lemm((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = lemm((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = analyse_morpho((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = analyse_morpho((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = flexion((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = mesure((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: exec(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Serveur::donneesRecues(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
