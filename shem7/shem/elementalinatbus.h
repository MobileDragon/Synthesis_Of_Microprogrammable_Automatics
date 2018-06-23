#ifndef ELEMENTALINATBUS_H
#define ELEMENTALINATBUS_H
#include "element.h"
#include "authomatbus.h"

class ElementalInAtBus : public Element//выход из схемы вход в шину
{//перемещается по высоте в пределах шины
public:

    ElementalInAtBus(QString nameOutput, QString id, AuthomatBus* bus, int numSlot, int posHeight);
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

#endif // ELEMENTALINATBUS_H
