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

struct out//список проводов из шины подаваемых на конкретный выход
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

    QList<QString> getListOutA();//список выходов автомата
    QList<QString> getListInputA();

protected:
    QString makeKey(int num);

    int idWire;//номер для представления выхода схемы в шине
    void fielMapBush();
    QMap<QString,QString> busIn;// номер провода и id триггера, который подается на него из шины
    //только на какой триггер устанавливается сигнал
    QMap<QString,QString> busInR;//id reset сигнала установленного на тригер

    void fielMapBush2RS();
    void fielMapBush3RS();
    void fielMapBush4RS();

    QMap<int, ShemAtBush> mapBush;//карта шин

    QMap<QString, trigerIn> mapTrigerIn; //карта входов триггеров

    QMap<QString, out> mapOut;



};


#endif // BUILDAUTHOMAT_H
