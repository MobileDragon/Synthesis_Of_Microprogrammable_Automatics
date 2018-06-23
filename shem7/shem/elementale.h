#ifndef ELEMENTALE_H
#define ELEMENTALE_H
#include "element.h"

class ElementalE : public Element//отрисовка элементарных логических элементов
{
public:
    ElementalE();
    ElementalE(QString nameElementFromBase);
    ElementalE(QString nameElementFromBase, QString id);
    void draw(QPainter*);

private:
    void generic(QString nameElementFromBase);

};

#endif // ELEMENTALE_H
