#include "elementalinatbus.h"


ElementalInAtBus::ElementalInAtBus(QString nameOutput, QString id, AuthomatBus *bus, int numSlot, int posHeight):Element(nameOutput, id)//добавить всем, а здесь пока к заголовку
{
    connectAtBus(bus, numSlot, posHeight);
    generic(nameOutput);//id хранить по нему будет взаимодействие работать
    this->id=id;
}

void ElementalInAtBus::connectAtBus(AuthomatBus* bus, int numSlot, int posHeight)
{
    curBus=bus;
    curSlot=numSlot;
    //heightAtBus=posHeight;
    setHeightAtBus(posHeight);
}

void ElementalInAtBus::draw(QPainter *painter)
{
    setHeightAtBus(heightAtBus);
    //painter->drawRect(posx, posy, width, height);
    painter->drawLine(posx-width, posy, posx, posy);

    //painter->drawLine(posx,posy+height/2,posx-10,posy+height/2);
    //wiresPos.insert(0,QPoint(posx-10,posy+height/2));
    wiresPos.insert(0, QPoint(posx-width, posy) );
    painter->drawText(posx+4-width,posy-2,name);

}

bool ElementalInAtBus::isOnclic(int x, int y)
{
    if(x>posx||x<posx-width)
        return false;
    if(y>posy||y<posy-height)
        return false;
    diffX=x-posx;
    diffY=y-posy;
    return true;
}

void ElementalInAtBus::setHeightAtBus(int posHeight)
{
    if(posHeight > curBus->getHeight() || posHeight < 0)
        return;
    heightAtBus = posHeight;
    posx=curBus->getSlotX(curSlot);
    posy=curBus->getPos().y() + heightAtBus;
}

void ElementalInAtBus::setPosY(int pos)
{
    int posHeight = pos - curBus->getPos().y();
    setHeightAtBus(posHeight);
}

void ElementalInAtBus::generic(QString nameOutput)//заполняй
{
    posx=10;
    posy=20;
    width=35;
    height=20;
    numInput=1;
    numOutput=0;
    type="out";
    name=nameOutput;
    interval=10;
}

void ElementalInAtBus::move(int x, int y)
{
    //this->posx=x-diffX;
    setPosY(y-diffY);
}
