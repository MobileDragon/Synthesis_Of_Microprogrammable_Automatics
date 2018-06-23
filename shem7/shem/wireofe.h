#ifndef WIREOFE_H
#define WIREOFE_H
#include <stdlib.h>
#include <math.h>
#include <QtCore/QList>
#include "element.h"
#include <QList>
#include <QPoint>
#include "Line.h"

class WireOfE//провода на графике по точкам
{

public:

    WireOfE(Element *start, Element *end, int startN, int endN);
    void draw(QPainter *painter, QList<QPoint> lcp = QList<QPoint>() );
    Element* startP;//элемент подающий сигнал
    Element* endP;//элемент принимающий сигнал
    int startN, endN;//номера входов и выходов

    int sizeBright;
    void updateStart();
    void update();
    bool isOnclic(int x, int y);
    void move(int x);
    void defaultW();//инициализация для отрисовки//вызывается извне(когда нужно начертить линии по умолчанию)
    //метод для передвижения участка линии по вертикали или горизонтали//остальные должны растягиваться
    //также может добавлять новые линии(когда изменяется вертикальная позиц начальн или конца)
    //удалять пока нет(соседние линии выглядят как одна)
    void mowLine(int num, int h, int w);//num-номер линии//h-(у)//w-(х)//отклонено
    bool addX();//добавляет две точки в конец линии образуя две новых линии(1-вертикальная 2-горизонтальная)
    //для этой линии изменять длинну(нет -х)
    bool mowX();


    bool generic(QList<Line> linesL);//получает список линий с которыми необходимо избежать столкновение
    bool isContact(QList<Line> lines, Line line);//получает список линий с которыми проверяется столкновение


    int d;//расстояние до других линий
    QList<Line> getLines();//ссылка на список линий



    QList<QPoint> dotLine;//в основном 4 точки(3 линии)
protected:
    bool activatedX;
    int startXvalue;//значение по х, которое имеет две точки пр добалении(addX())

    //QList<LineW> Lines;//в основном 3 линии


};

#endif // WIREOFE_H
