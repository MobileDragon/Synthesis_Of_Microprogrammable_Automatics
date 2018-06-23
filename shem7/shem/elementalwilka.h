#ifndef ELEMENTALWILKA_H
#define ELEMENTALWILKA_H
#include "element.h"

class ElementalWilka: public Element//разветвляет выходной сигнал на большее число входов(жирная точка)
{
public:
    ElementalWilka();
    ElementalWilka(QString name);
    ElementalWilka(QString name, QString id);

    void draw(QPainter*);

private:
    void generic(QString name);

};

#endif // ELEMENTALWILKA_H
