#ifndef PAINTAUTHOMAT_H
#define PAINTAUTHOMAT_H

#include "paintwidget.h"
#include "TransitionAuthomat.h"
#include <QList>
#include <QMap>
#include <QString>
#include "ShemAtBush.h"
#include "authomatbus.h"

#include "elementalinatbus.h"
#include "elementaloutfrombus.h"

class paintAuthomat : public PaintWidget
{
    Q_OBJECT
public:
    explicit paintAuthomat(QList<TransitionAuthomat> list_transit, PaintWidget *parent = 0);

    void setMapBus();
    QList<QString> getListInputs();//вернет список входов на автомат

private:
    int spaceConnect;//расстояние между выводами из шин

    QList<TransitionAuthomat> listTransit;//переходы
    QMap<int, ShemAtBush> mapBush;//карта шин
    void paintMainInputs();//отрисовка первой шины(ее позиция) и входы на неё

    //AuthomatBus busBuffer;//список шин
    //virtual void mousePressEvent(QMouseEvent *event);
    //virtual void mouseReleaseEvent(QMouseEvent *event);
    //virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    QList<AuthomatBus*> bBuffer;//список нарисованых шин(кривые)
    //QList<QString> getInputs();
    
signals:
    
public slots:
    
};

#endif // PAINTAUTHOMAT_H
