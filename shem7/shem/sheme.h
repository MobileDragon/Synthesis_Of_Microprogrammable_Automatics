#ifndef SHEME_H
#define SHEME_H
#include <QString>
//#include "currentwire.h"
#include <QMap>
#include <QVector>
#include "bde.h"
#include <QVariant>

class ShemE;

class currentWire//содержит ссылку на следующий элемент в схеме,
                 //а также номер провода что выходит и тот в который входит
{
public:
    currentWire();
    currentWire(ShemE *nextElement,int numInput);
    ~currentWire();
    currentWire(currentWire* wire);
    void makeLink(ShemE *nextElement,int numInput);
    //int curWire; указано в ключе списка

    int nextWire;//номер входа в следующем элементе
    ShemE* nextE;//указатель на следующий элемент
};


class ShemE//структура элементов схемы содержит
           //имя элемента с указателями(массив выходов проводов) на следующий элемент
{
public:
    ShemE();
    ~ShemE();
    ShemE(QString name,QString id);//elemental
    ShemE(QString id, int kolOut);//использовать для дебавления вилки
    ShemE(QString name);//использовать для добавления входов
    ShemE(QString name,QString id, bool);
    ShemE(int numY, QString nameOut);//использовать для добавления выходов(имя запис в id)
    ShemE(ShemE& element);
    static QVector<int> getInput(QString name);//вернет список входов из базы
    static QVector<int> getOut(QString name);//вернет список выходов из базы
    static int getFreeOut(ShemE* element);//возвращает указатель на любой свободный выход

    QString name;
    QString id;//@1,@2,@3,.. для элементов обознвчающих входы и выходы Х1, У1
    //массив выходов
    QMap <int,currentWire*> wire;//int - номер выхода//провода выходящие из элементов//в основном один
};

#endif // SHEME_H
