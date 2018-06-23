#ifndef CURRENTSHEM_H
#define CURRENTSHEM_H
#include <QVector>
#include "sheme.h"

class CurrentShem//хранит массив указателей на элементы схемы
{
public:
    CurrentShem();
    CurrentShem(CurrentShem* currentShem);
    ~CurrentShem();
    bool operator== (CurrentShem* A);
    bool operator!= (CurrentShem* A);


    QVector<ShemE*> elements;//массив указателей на элементы
};

#endif // CURRENTSHEM_H
