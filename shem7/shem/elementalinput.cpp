#include "elementalinput.h"

ElementalInput::ElementalInput()
{
}

ElementalInput::ElementalInput(QString nameInput)
{
    generic(nameInput);
}

ElementalInput::ElementalInput(QString nameInput, QString id):Element(nameInput, id)//добавить всем, а здесь пока к заголовку
{
    generic(nameInput);//id хранить по нему будет взаимодействие работать
    this->id=id;
}

void ElementalInput::draw(QPainter *painter)
{
    painter->drawRect(posx, posy, width, height);

    painter->drawLine(posx+width,posy+height/2,posx+width+10,posy+height/2);
    wiresPos.insert(0,QPoint(posx+width+10,posy+height/2));
    painter->drawText(posx+5,posy+height/2,type);

}

void ElementalInput::generic(QString nameInput)//заполняй
{
    posx=10;
    posy=20;
    width=30;
    height=30;
    numInput=0;
    numOutput=1;
    type=nameInput;
    name=nameInput;
    interval=10;
}
