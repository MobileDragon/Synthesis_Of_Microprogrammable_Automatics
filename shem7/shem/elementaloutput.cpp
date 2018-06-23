#include "elementaloutput.h"

ElementalOutput::ElementalOutput()
{
}

ElementalOutput::ElementalOutput(QString nameOutput)
{
    generic(nameOutput);
}

ElementalOutput::ElementalOutput(QString nameOutput, QString id):Element(nameOutput, id)//добавить всем, а здесь пока к заголовку
{
    generic(nameOutput);//id хранить по нему будет взаимодействие работать
    this->id=id;
}

void ElementalOutput::draw(QPainter *painter)
{
    painter->drawRect(posx, posy, width, height);

    painter->drawLine(posx,posy+height/2,posx-10,posy+height/2);
    wiresPos.insert(0,QPoint(posx-10,posy+height/2));
    painter->drawText(posx+5,posy+height/2,type);

}

void ElementalOutput::generic(QString nameOutput)//заполняй
{
    posx=10;
    posy=20;
    width=30;
    height=30;
    numInput=1;
    numOutput=0;
    type=nameOutput;
    name=nameOutput;
    interval=10;
}
