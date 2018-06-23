#ifndef ELEMENTALT_H
#define ELEMENTALT_H
#include "element.h"

class ElementalT : public Element
{
public:
    ElementalT();
    ElementalT(QString nameElementFromBase);
    ElementalT(QString nameElementFromBase, QString id);
    void draw(QPainter*);

private:
    void generic(QString nameElementFromBase);
};

#endif // ELEMENTALT_H
