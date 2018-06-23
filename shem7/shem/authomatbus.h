#ifndef AUTHOMATBUS_H
#define AUTHOMATBUS_H
#include <stdlib.h>
#include <math.h>
#include <QtCore/QList>
#include "element.h"
#include <QList>
#include <QPoint>
#include "Line.h"

class AuthomatBus//������ �����
{//�������� ������ � ������ �� ������ ���������
    //���� ������ �,U,I ��������, �=3 U=2 I=1.
    //������� ���������� ���� ��������� startP, ��� ������������ � ������� ����� ����� - ������ �����-
    //������� ����� �� ��������� ���������������-
    // �,U ������������ �����������.
    // �,U ����� ��� ������������ ����� �����������(��������� ����� ��� ��������� ������� �) I-1
public:
    AuthomatBus(int type, QPoint start,int height, int width=0);
    int getSlotX(int numSlot);//��������� ������ � 1
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
    QMap<int,Line> mapLine;//�����: �����, �����//��������� � 0
    int widthB;
    int heightB;

    void generic_n(QPoint start,int height, int width);
    void generic_u(QPoint start,int height, int width);
    void generic_i(QPoint start,int height);
};

#endif // AUTHOMATBUS_H
