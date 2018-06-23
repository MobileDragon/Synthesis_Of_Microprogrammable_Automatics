#ifndef ELEMENTALINPUT_H
#define ELEMENTALINPUT_H
#include "element.h"

class ElementalInput: public Element//входной сигнал
{
public:
    ElementalInput();
    ElementalInput(QString nameInput);
    ElementalInput(QString nameElementFromBase, QString id);
    void draw(QPainter*);

private:
    void generic(QString nameInput);
};

#endif // ELEMENTALINPUT_H
