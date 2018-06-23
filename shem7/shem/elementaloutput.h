#ifndef ELEMENTALOUTPUT_H
#define ELEMENTALOUTPUT_H
#include "element.h"

class ElementalOutput: public Element//выходной сигнал
{
public:
    ElementalOutput();
    ElementalOutput(QString nameOutput);
    ElementalOutput(QString nameOutput, QString id);

    void draw(QPainter*);

private:
    void generic(QString nameOutput);

};

#endif // ELEMENTALOUTPUT_H
