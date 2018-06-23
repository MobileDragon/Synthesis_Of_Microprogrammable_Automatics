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

class Element//����������� ����� ��� ��������� ������������ � ������� ���������� ���������
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

    QPoint getPosInput(int numIn);//����� ����� �� ������� wiresPos
    QPoint getPosOutput(int numOut);

    QString type;
    QString name;//���
    QString id;//�������� ������� �������("X1+~X2")


protected://�������� ��� � ����� ��� ������� ���������
    int width;
    int height;
    int posx;
    int posy;
    int diffX;
    int diffY;
    int numInput;//c 0 ����� � 0 ����������
    int numOutput;

    int interval;
    //� ��� ����� ��������� �� ����
    QMap <int,QPoint> wiresPos;//������ ����� ��� ����������� ��������
    //QVector<WireOfE> wiresPos;//��������� ������������� ������
};

#endif // ELEMENT_H
