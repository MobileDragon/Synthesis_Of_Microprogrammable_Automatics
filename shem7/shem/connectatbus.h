#ifndef CONNECTATBUS_H
#define CONNECTATBUS_H
#include <QString>
#include <math.h>
#include <Qvector>
#include <QStringList>
#include <QDebug>
#include <QPainter>
#include "authomatbus.h"
#include <QMap>

class ConnectAtBus
{
public:
    ConnectAtBus();
    ConnectAtBus(AuthomatBus* bus, int numSlot, int posHeight);
    bool setPosY(int pos);//
    bool setPosHeight(int height);//Height ��������� ������ ����(Y ������������ ����)
    void setBus(AuthomatBus* bus);
    void setSlot(int numSlot);

    QPoint getPosConect();//����������� ��������

    bool isOnclic(int x,int y);
protected:
    int heigtAtBus;


    int posx;
    int posy;
    int diffX;
    int diffY;
};

#endif // CONNECTATBUS_H
