#ifndef SHEM_H
#define SHEM_H
#include "currentshem.h"
#include "bde.h"
#include <QVariant>

class Shem
{
public:
    Shem();
    ~Shem();
    static CurrentShem* next(CurrentShem* curent);//���������� ��������� �� ��������� ��������
    void addBack(ShemE* element);//���������� ��������� ��� ����������� ������(���������)
    void addBack(ShemE* element, QVector<QString> inputs);//��������� � ������������ ������
    void addWilka(QString id, int numOuts);//��������� ������������� �� �����(���������� ��� ��� ������� �������)
    //�������� id �� �������� ����� ���������� � ����� ������������



    CurrentShem* start;
    CurrentShem* end;

};



#endif // SHEM_H
