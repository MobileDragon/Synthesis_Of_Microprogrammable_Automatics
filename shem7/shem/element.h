#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>
#include <math.h>
#include <Qvector>
#include <QStringList>
#include <QDebug>
#include <QPainter>
#include "bde.h"
//#include "wireofe.h"
#include <QMap>

class Element//виртуальный класс для отрисовки элементарных и сложных логических элементов
{
public:
    Element();
    Element(QString nameElementFromBase);
    Element(QString nameElementFromBase, QString id);
    virtual void draw(QPainter*)=0;

    virtual bool isOnclic(int x,int y);
    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getNumInput();
    int getNumOutput();
    QString getId();
    void setId(QString id);
    void setPos(int x,int y);
    void setPosX(int x);
    void setPosY(int y);
    virtual void move(int x,int y);

    QPoint getPosInput(int numIn);//берет входы из массива wiresPos
    QPoint getPosOutput(int numOut);

    QString type;
    QString name;//имя
    QString id;//операция которая выходит("X1+~X2")


protected://добавить имя и время для быстрой подсказки
    int width;
    int height;
    int posx;
    int posy;
    int diffX;
    int diffY;
    int numInput;//c 0 везде с 0 начинается
    int numOutput;

    int interval;
    //и так далее заполнять из базы
    QMap <int,QPoint> wiresPos;//список точек для подключения проводов
    //QVector<WireOfE> wiresPos;//необходим ассоциативный список
};

#endif // ELEMENT_H
