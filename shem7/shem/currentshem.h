#ifndef CURRENTSHEM_H
#define CURRENTSHEM_H
#include <QVector>
#include "sheme.h"

class CurrentShem//������ ������ ���������� �� �������� �����
{
public:
    CurrentShem();
    CurrentShem(CurrentShem* currentShem);
    ~CurrentShem();
    bool operator== (CurrentShem* A);
    bool operator!= (CurrentShem* A);


    QVector<ShemE*> elements;//������ ���������� �� ��������
};

#endif // CURRENTSHEM_H
