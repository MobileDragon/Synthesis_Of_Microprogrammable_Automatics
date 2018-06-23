#ifndef SHEM_H
#define SHEM_H
#include "currentshem.h"
#include "bde.h"
#include <QVariant>

class Shem
{
public:
    Shem();
    ~Shem();
    static CurrentShem* next(CurrentShem* curent);//возвращает указатель на следующие элементы
    void addBack(ShemE* element);//добавление елементов без подключений входов(начальных)
    void addBack(ShemE* element, QVector<QString> inputs);//доавление с подключением входов
    void addWilka(QString id, int numOuts);//добавляет разветвлитель на выход(вызывается тем кто добавил элемент)
    //получает id по которому можео обратиться и число развлетлений



    CurrentShem* start;
    CurrentShem* end;

};



#endif // SHEM_H
