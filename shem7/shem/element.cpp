#include "element.h"


Element::Element()
{
    //this->id="";
}

Element::Element(QString nameElementFromBase)
{
}
Element::Element(QString nameElementFromBase,QString id)
{
}
//setId
int Element::getX()
{
    return posx;
}

int Element::getY()
{
    return posy;
}

void Element::setPos(int x, int y)
{
    this->posx=x;
    this->posy=y;
}

void Element::setPosX(int x)
{
    this->posx=x;
}

void Element::setPosY(int y)
{
    this->posy=y;
}

void Element::move(int x, int y)
{
    this->posx=x-diffX;
    this->posy=y-diffY;
}

int Element::getWidth()
{
    return width;
}

int Element::getHeight()
{
    return height;
}
QString Element::getId()
{
    return id;
}
void Element::setId(QString id)
{
    this->id=id;
}

//
QPoint Element::getPosInput(int numIn)//подсчеет начинается с 0
{
    QPoint dot=wiresPos[numIn];
    return dot;
}

QPoint Element::getPosOutput(int numOut)
{
    QPoint dot=wiresPos[numOut];
    return dot;
}

int Element::getNumInput()
{
    return numInput;
}

int Element::getNumOutput()
{
    return numOutput;
}


bool Element::isOnclic(int x, int y)
{
    if(x<posx||x>posx+width)
        return false;
    if(y<posy||y>posy+height)
        return false;
    diffX=x-posx;
    diffY=y-posy;
    return true;
}
