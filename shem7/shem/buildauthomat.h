#ifndef BUILDAUTHOMAT_H
#define BUILDAUTHOMAT_H
#include "TransitionAuthomat.h"
#include <QList>
#include <QMap>
#include <QString>
#include "ShemAtBush.h"
#include "comply.h"

struct trigerIn
{
    QString S;
    QString R;
};

struct out//������ �������� �� ���� ���������� �� ���������� �����
{
    QList<QString>in;
};

class BuildAuthomat
{
public:
    BuildAuthomat(QList<TransitionAuthomat> list_transit, QString reset_state);
    QList<TransitionAuthomat> listTransit;
    QString resetState;

    QMap<int, ShemAtBush> getBush();

    QList<QString> getListOutA();//������ ������� ��������
    QList<QString> getListInputA();

protected:
    QString makeKey(int num);

    int idWire;//����� ��� ������������� ������ ����� � ����
    void fielMapBush();
    QMap<QString,QString> busIn;// ����� ������� � id ��������, ������� �������� �� ���� �� ����
    //������ �� ����� ������� ��������������� ������
    QMap<QString,QString> busInR;//id reset ������� �������������� �� ������

    void fielMapBush2RS();
    void fielMapBush3RS();
    void fielMapBush4RS();

    QMap<int, ShemAtBush> mapBush;//����� ���

    QMap<QString, trigerIn> mapTrigerIn; //����� ������ ���������

    QMap<QString, out> mapOut;



};


#endif // BUILDAUTHOMAT_H
