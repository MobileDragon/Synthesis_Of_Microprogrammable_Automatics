#ifndef WIREOFE_H
#define WIREOFE_H
#include <stdlib.h>
#include <math.h>
#include <QtCore/QList>
#include "element.h"
#include <QList>
#include <QPoint>
#include "Line.h"

class WireOfE//������� �� ������� �� ������
{

public:

    WireOfE(Element *start, Element *end, int startN, int endN);
    void draw(QPainter *painter, QList<QPoint> lcp = QList<QPoint>() );
    Element* startP;//������� �������� ������
    Element* endP;//������� ����������� ������
    int startN, endN;//������ ������ � �������

    int sizeBright;
    void updateStart();
    void update();
    bool isOnclic(int x, int y);
    void move(int x);
    void defaultW();//������������� ��� ���������//���������� �����(����� ����� ��������� ����� �� ���������)
    //����� ��� ������������ ������� ����� �� ��������� ��� �����������//��������� ������ �������������
    //����� ����� ��������� ����� �����(����� ���������� ������������ ����� ������� ��� �����)
    //������� ���� ���(�������� ����� �������� ��� ����)
    void mowLine(int num, int h, int w);//num-����� �����//h-(�)//w-(�)//���������
    bool addX();//��������� ��� ����� � ����� ����� ������� ��� ����� �����(1-������������ 2-��������������)
    //��� ���� ����� �������� ������(��� -�)
    bool mowX();


    bool generic(QList<Line> linesL);//�������� ������ ����� � �������� ���������� �������� ������������
    bool isContact(QList<Line> lines, Line line);//�������� ������ ����� � �������� ����������� ������������


    int d;//���������� �� ������ �����
    QList<Line> getLines();//������ �� ������ �����



    QList<QPoint> dotLine;//� �������� 4 �����(3 �����)
protected:
    bool activatedX;
    int startXvalue;//�������� �� �, ������� ����� ��� ����� �� ���������(addX())

    //QList<LineW> Lines;//� �������� 3 �����


};

#endif // WIREOFE_H
