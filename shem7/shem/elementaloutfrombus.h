#ifndef ELEMENTALOUTFROMBUS_H
#define ELEMENTALOUTFROMBUS_H
#include "element.h"
#include "authomatbus.h"

class ElementalOutFromBus : public Element//���� �� �����, ����� �� ����
{
public:

    ElementalOutFromBus(QString nameOutput, QString id, AuthomatBus* bus, int numSlot, int posHeight);
    void connectAtBus(AuthomatBus* bus, int numSlot, int posHeight);//����������� � ����
    //

    void setPosY(int pos);//
   // bool setPosHeight(int height);//Height ��������� ������ ����(Y ������������ ����)

    void draw(QPainter*);
    void move(int x,int y);
    bool isOnclic(int x,int y);
    void setHeightAtBus(int posHeight);


private:
    int heightAtBus;//������ ���������� ������ ����
    AuthomatBus* curBus;
    int curSlot;


    void generic(QString nameOutput);
};

#endif // ELEMENTALOUTFROMBUS_H
