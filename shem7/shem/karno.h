#ifndef KARNO_H
#define KARNO_H
#include <QString>
#include <QFile>
#include <math.h>
#include <Qvector>
#include <QStringList>
#include <QDebug>

class Karno
{
public:
    static QVector<QString> buildDNF(QString);
    static QString convert(QString);
private:
    Karno();
    static QString makeBinOfDec(uint32_t ,uint32_t);
    static uint32_t makeDecOfBin(QString);
    static QString getGlobal(QString,QString);
    static QVector<uint32_t> searchGlobal(QString,QVector<uint32_t>);
    static bool chekNum(QVector<uint32_t>,QVector<uint32_t>, int);
    static QVector<QString> separator(QVector<QString>);

    static QVector<QString> delExcessJoin(QVector<QString> ,QVector<uint32_t> );
    static bool isContain(QString,QString);

    static QVector<uint32_t> readCSV(QString);
};

#endif // KARNO_H
