#ifndef TRANSITIONAUTHOMAT_H
#define TRANSITIONAUTHOMAT_H
#include <QString>
struct TransitionAuthomat
{
    QString idInput;//����� ������ �������
    QString idOut;//������ ������� ���������� ������������
    QString prevState;//id �������� ��� ������ 1
    QString nextState;//������� ������� ������ ��������������
};

#endif // TRANSITIONAUTHOMAT_H
