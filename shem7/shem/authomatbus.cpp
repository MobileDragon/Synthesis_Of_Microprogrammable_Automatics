#include "authomatbus.h"


AuthomatBus::AuthomatBus(int type, QPoint start,int height, int width)
{
    pos=start;
    cur_type=type;
    widthB=width;
    heightB=height;

    switch(type)
        {
            case 1:generic_i(start, height);
            break;
            case 2:generic_u(start, height, width);
            break;
            case 3:generic_n(start, height, width);
        }

}

QPoint AuthomatBus::getPos()
{
    return pos;
}

void AuthomatBus::setPos(QPoint start)
{
    pos=start;
    switch(cur_type)
        {
            case 1:generic_i(pos, heightB);
            break;
            case 2:generic_u(pos, heightB, widthB);
            break;
            case 3:generic_n(pos, heightB, widthB);
        }
}

void AuthomatBus::draw(QPainter *painter)
{
    foreach(Line L, mapLine.values())
    {
        painter->drawLine(L.start, L.end);
    }
}

void AuthomatBus::generic_n(QPoint start,int height, int width)
{
    Line line1;
    line1.start=start;
    line1.end=QPoint(start.x(),start.y()+height);
    mapLine.insert(0,line1);
    Line line2;
    line2.start=start;
    line2.end=QPoint(start.x()+width,start.y());
    mapLine.insert(1,line2);
    Line line3;
    line3.start=QPoint(start.x()+width, start.y());
    line3.end=QPoint(start.x()+width,start.y()+height);
    mapLine.insert(2,line3);

}

void AuthomatBus::generic_u(QPoint start,int height, int width)
{
    Line line1;
    line1.start=start;
    line1.end=QPoint(start.x(),start.y()+height);
    mapLine.insert(0,line1);
    Line line2;
    line2.start=QPoint(start.x(),start.y()+height);
    line2.end=QPoint(start.x()+width,start.y()+height);
    mapLine.insert(1,line2);
    Line line3;
    line3.start=QPoint(start.x()+width,start.y()+height);
    line3.end=QPoint(start.x()+width, start.y());
    mapLine.insert(2,line3);
}

void AuthomatBus::generic_i(QPoint start,int height)
{
    Line line1;
    line1.start=start;
    line1.end=QPoint(start.x(),start.y()+height);
    mapLine.insert(0,line1);
}

int AuthomatBus::getSlotX(int numSlot)
{
    if(numSlot == 1)
        return mapLine[0].start.x();
    else
        if(numSlot == 2)
            return mapLine[2].start.x();
}

void AuthomatBus::setWidth(int w)
{
    widthB=w;
    setPos(pos);
}

void AuthomatBus::setHeight(int h)
{
    heightB=h;
    setPos(pos);
}

int AuthomatBus::getWidth()
{
    return widthB;
}

int AuthomatBus::getHeight()
{
    return heightB;
}
