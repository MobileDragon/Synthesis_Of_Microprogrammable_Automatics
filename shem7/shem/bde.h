#ifndef BDE_H
#define BDE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>


class BDE
{
public:
    static BDE* getInstance();

    QSqlQuery *make(QString request);
    void insert(QString request);
    //управление selectedE
    void setSelectedE(QList<QString> elements);
    void clearSelectedE();


protected:
    QSqlDatabase db;
    QSqlQuery *query;
    QList<QString> selectedE;//элементы что можно выбирать
    //когда список пустой-значит можно вибирать все
    //
    void inicialize();
private:
    BDE();
    //~BDE();
    BDE(BDE const&) = delete;
    BDE& operator= (BDE const&) = delete;




};

#endif // BDE_H
