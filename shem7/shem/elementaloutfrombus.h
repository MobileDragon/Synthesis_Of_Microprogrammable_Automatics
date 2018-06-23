#ifndef ELEMENTALOUTFROMBUS_H
#define ELEMENTALOUTFROMBUS_H
#include "element.h"
#include "authomatbus.h"

class ElementalOutFromBus : public Element//вход на схему, выход из шины
{
public:

    ElementalOutFromBus(QString nameOutput, QString id, AuthomatBus* bus, int numSlot, int posHeight);
    void connectAtBus(AuthomatBus* bus, int numSlot, int posHeight);//подключение к шине
    //

    void setPosY(int pos);//
   // bool setPosHeight(int height);//Height считается сверху вниз(Y относительно шины)

    void draw(QPainter*);
    void move(int x,int y);
    bool isOnclic(int x,int y);
    void setHeightAtBus(int posHeight);


private:
    int heightAtBus;//высота счаитается сверху вниз
    AuthomatBus* curBus;
    int curSlot;


    void generic(QString nameOutput);
};

#endif // ELEMENTALOUTFROMBUS_H
