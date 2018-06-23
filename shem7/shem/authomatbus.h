#ifndef AUTHOMATBUS_H
#define AUTHOMATBUS_H
#include <stdlib.h>
#include <math.h>
#include <QtCore/QList>
#include "element.h"
#include <QList>
#include <QPoint>
#include "Line.h"

class AuthomatBus//массив линий
{//задаютс€ высота и ширина от начала посроени€
    //шины бывают ѕ,U,I образные, ѕ=3 U=2 I=1.
    //началом построени€ есть положение startP, что установленно в верхней левой части - перва€ лини€-
    //поэтому линии не соединены последовательно-
    // ѕ,U двухсторонне симетричные.
    // ѕ,U имеют два вертикальных слота подключени€(указывать номер дл€ получении позиции х) I-1
public:
    AuthomatBus(int type, QPoint start,int height, int width=0);
    int getSlotX(int numSlot);//номераци€ слотов с 1
    QPoint getPos();
    void setWidth(int w);
    void setHeight(int h);
    void setPos(QPoint start);
    int getWidth();
    int getHeight();
    void draw(QPainter *painter);
private:
    QPoint pos;
    int cur_type;
    Element* startP;
    Element* endP;
    QMap<int,Line> mapLine;//карта: номер, лини€//нумераци€ с 0
    int widthB;
    int heightB;

    void generic_n(QPoint start,int height, int width);
    void generic_u(QPoint start,int height, int width);
    void generic_i(QPoint start,int height);
};

#endif // AUTHOMATBUS_H
