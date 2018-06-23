#ifndef SHEME_H
#define SHEME_H
#include <QString>
//#include "currentwire.h"
#include <QMap>
#include <QVector>
#include "bde.h"
#include <QVariant>

class ShemE;

class currentWire//�������� ������ �� ��������� ������� � �����,
                 //� ����� ����� ������� ��� ������� � ��� � ������� ������
{
public:
    currentWire();
    currentWire(ShemE *nextElement,int numInput);
    ~currentWire();
    currentWire(currentWire* wire);
    void makeLink(ShemE *nextElement,int numInput);
    //int curWire; ������� � ����� ������

    int nextWire;//����� ����� � ��������� ��������
    ShemE* nextE;//��������� �� ��������� �������
};


class ShemE//��������� ��������� ����� ��������
           //��� �������� � �����������(������ ������� ��������) �� ��������� �������
{
public:
    ShemE();
    ~ShemE();
    ShemE(QString name,QString id);//elemental
    ShemE(QString id, int kolOut);//������������ ��� ���������� �����
    ShemE(QString name);//������������ ��� ���������� ������
    ShemE(QString name,QString id, bool);
    ShemE(int numY, QString nameOut);//������������ ��� ���������� �������(��� ����� � id)
    ShemE(ShemE& element);
    static QVector<int> getInput(QString name);//������ ������ ������ �� ����
    static QVector<int> getOut(QString name);//������ ������ ������� �� ����
    static int getFreeOut(ShemE* element);//���������� ��������� �� ����� ��������� �����

    QString name;
    QString id;//@1,@2,@3,.. ��� ��������� ������������ ����� � ������ �1, �1
    //������ �������
    QMap <int,currentWire*> wire;//int - ����� ������//������� ��������� �� ���������//� �������� ����
};

#endif // SHEME_H
