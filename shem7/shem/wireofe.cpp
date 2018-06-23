#include "wireofe.h"

#include <QDebug>

#include <QMessageBox>
WireOfE::WireOfE(Element *start, Element *end, int startN, int endN)//номера начала и конца(выхода и входа)
{
    activatedX = false;

    d=3;//расстояние до других линий
    sizeBright = 5;
    this->startP=start;
    this->endP=end;
    this->endN=endN;

    qDebug()<<"MEGARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR: "<< startN <<" : "<<endN;

    this->startN=startN;
    defaultW();

}
void WireOfE::draw(QPainter *painter, QList<QPoint> lcp)//lcp список точек где лолжні біть миости
{

    //defaultW();


    QList<Line>LineW = getLines();


    Line Tline;
    foreach(Line L, LineW)
    {
        if (L.start.x() != L.end.x())
            painter->drawLine(L.start, L.end);
        else
            Tline = L;
    }


/*
    dotLine[1];


    QPoint min_y=lcp.at(0);

    foreach(QPoint p_, lcp)
    {

        for(int i=0: i < lcp.)

        foreach(QPoint p, lcp)
        {
            if(p.y() < min_y.y())
                min_y.y() = p.y();
        }

    }
*/

    int Ty = (Tline.start.y() < Tline.end.y())?Tline.start.y():Tline.end.y();
    int My = (Tline.start.y() > Tline.end.y())?Tline.start.y():Tline.end.y();
    int Ymin=3;

    for (int i = 0; i < lcp.size(); i++)
    {
        qDebug()<<"<<<<<<<<<<<<<<<<<<<<<<"<<i;
        foreach(QPoint PP, lcp)
        {
            qDebug()<<775;
            if (PP.y() <= Ty-sizeBright) continue;
            Ymin = PP.y(); break;
        }
        qDebug()<<3;
        foreach(QPoint PP, lcp)
        {
            qDebug()<<77;
            if (PP.y() <= Ty-sizeBright) continue;
            if (PP.y() < Ymin) Ymin = PP.y();
        }
        qDebug()<<4;
        painter->drawLine(QPoint(Tline.start.x(), Ty), QPoint(Tline.start.x(), Ymin - sizeBright) );
        painter->drawArc(Tline.start.x()-sizeBright, Ymin-sizeBright, sizeBright*2, sizeBright*2, 90*16, 180*16);

        qDebug()<<"<<<<<<<<<<<<<<<<<<<<<<Ty "<<Ty;
        qDebug()<<"<<<<<<<<<<<<<<<<<<<<<<Ymin "<<Ymin;

        Ty = Ymin+sizeBright;
        qDebug()<<5;
    }

    painter->drawLine(QPoint(Tline.start.x(), Ty ), QPoint(Tline.start.x(), My));
    qDebug()<<6;

}

void WireOfE::update()
{
    updateStart();
    QPoint end=endP->getPosInput(endN);
    dotLine[3]=end;
    QPoint knee2(dotLine[2].x(), end.y());
    dotLine[2]=knee2;
}

bool WireOfE::isOnclic(int x, int y)
{
    if(x > dotLine[1].x()-3 && x < dotLine[1].x()+3 &&
      ( y > dotLine[1].y() == y < dotLine[2].y() )    )
       return true;
    return false;
}

void WireOfE::move(int x)
{
    if(x < dotLine[0].x()+2 || x > dotLine[3].x()-2 )
        return;
    dotLine[1].setX(x);
    dotLine[2].setX(x);
}

void WireOfE::updateStart()
{
    QPoint start=startP->getPosOutput(startN);
    dotLine[0]=start;
    QPoint knee1(dotLine[1].x(), start.y());
    dotLine[1]=knee1;
}

void WireOfE::defaultW()
{
    dotLine.clear();
    QPoint start=startP->getPosOutput(startN);
    QPoint end=endP->getPosInput(endN);
    int midle_lenght=end.x()-start.x()-3;//(end.x()-start.x())/20;
    dotLine.append(start);
    dotLine.append(QPoint(start.x() + midle_lenght, start.y()));
    dotLine.append(QPoint(start.x() + midle_lenght, end.y()));
    dotLine.append(end);
    //
    if(activatedX)
        addX();
}

bool WireOfE::generic(QList<Line> linesL)
{
    if(linesL.size()<1)
        return true;
    QList<Line> lines;
    foreach(Line L, linesL)
        lines.append(L);

    QList<Line> wire=getLines();
    int size=wire.size();
    for(int i=0; i<size; i++)//перемешать 1-й провод
    {
        Line curL=wire[i];
        //bool znak=true;
        int offset=-1;
        bool flag=false;
        while(!isContact(lines, curL) )
        {
            if(dotLine[1].x()>204 && dotLine[1].x()<240 && dotLine[1].y()>40)
            {
                qDebug()<<dotLine[1].x();
                qDebug()<<"joi";
            }


            flag=true;

            if(activatedX)
            {
                if(!mowX())
                {
                 /* QMessageBox msgBox;
                    msgBox.setText( QString("First Name:     %1%2%3%4%5%6%7  ").arg( QString::number(dotLine[0].x()) ,"|",QString::number(dotLine[0].y()),"|1>",QString::number(dotLine[1].x()),"|",QString::number(dotLine[1].y() ) ) );
                    msgBox.exec();*/
                    return false;
                }
            }
            else
            {
                mowLine(1, offset, 0);
                if(dotLine[1].x() < dotLine[0].x() )//ширина бега
                    return false;
            }
            wire=getLines();
            curL=wire[i];

            //i=0;//проверить предыдущие линии//
        }
        //
        //






        //lines.append(curL);
        if(flag)
            i=-1;

    }
    return true;
}

bool WireOfE::addX()
{
    if(dotLine.size()==6)
        return false;
    activatedX = true;
    startXvalue = dotLine[1].x();

    QPoint point1;
    point1.setX( dotLine[1].x() );
    point1.setY(  dotLine[2].y() );//установка линии в верхней позиции
    QPoint point2;
    point2.setX( dotLine[1].x() );
    point2.setY(  dotLine[2].y() );

    dotLine.insert(2, point1);
    dotLine.insert(3, point2);
    return true;//провод расширен
}

bool  WireOfE::mowX()
{

    int leftMax = dotLine[0].x();
    int rightMax = dotLine[5].x();
    int lowLimit;
    if(dotLine[0].y()>dotLine[5].y() )
        lowLimit=dotLine[0].y();
    else
        lowLimit=dotLine[5].y();
    //для каждого левого все правые
    if(dotLine[3].x() < rightMax)
    {
        dotLine[3].setX( dotLine[3].x() + 3 );
        //
        qDebug()<<"right<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        dotLine[4].setX( dotLine[3].x() );
        return true;
    }
    else
        if(dotLine[2].x() > leftMax )
        {
            dotLine[3].setX(startXvalue);//умолчание
            dotLine[4].setX(startXvalue);//умолчание
            dotLine[2].setX( dotLine[2].x() - 3 );
            //
            qDebug()<<"left<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
            dotLine[1].setX( dotLine[2].x() );
            return true;
        }
        else
            if(dotLine[2].y() < lowLimit )
            {
                dotLine[3].setX(startXvalue);//умолчание
                dotLine[4].setX(startXvalue);//умолчание
                dotLine[2].setX(startXvalue);//умолчание
                dotLine[1].setX(startXvalue);//умолчание

                qDebug()<<"down<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
                dotLine[2].setY(dotLine[2].y()+3);
                dotLine[3].setY(dotLine[2].y());//горизонтальная линия

/*
                if(dotLine[1].x()-dotLine[0].x()>1 && dotLine[1].x()-dotLine[0].x()<5 && dotLine[2].y() > 80)
                {
                    QMessageBox msgBox;
                    msgBox.setText( QString("fail") );
                    msgBox.exec();
                }*/
                return true;
            }
            else
                return false;
}

bool WireOfE::isContact(QList<Line> lines, Line line)
{
    foreach(Line cur, lines)
    {
        bool contact1=false;//хотябы одна из линий содержит точку пересечения,
        //или есть нахлест для паралельных
        if(cur.start.x() == cur.end.x())
        {//вертикальная
            if(line.start.x() == line.end.x())
            {//также вертикальная
                bool flag=false;
                if( (cur.start.y() - line.start.y()) * (cur.start.y() - line.end.y() )<=0 )
                    flag = true;
                else if( (cur.end.y() - line.start.y()) * (cur.end.y() - line.end.y() )<=0 )
                    flag = true;
                //
                else if( (line.start.y() - cur.start.y()) * (line.start.y() - cur.end.y() )<=0 )
                    flag = true;
                else if( (line.end.y() - cur.start.y()) * (line.end.y() - cur.end.y() )<=0 )
                    flag = true;

                if(flag)
                {
                    contact1=true;
                    if(abs(cur.start.x() - line.start.x())  < d )
                        return false;
                }
            }
            else
            {//горизонтальная, линии перпендикулярны
                QPoint H(cur.start.x(), line.start.y());//точка пересечения
                if( (H.x() - line.start.x()) * (H.x() - line.end.x() )<=0 )
                {//H находится на line
                    contact1=true;
                    if( (H.y() - cur.start.y()) * (H.y() - cur.end.y() )<=0 )
                    {//H находится на cur
                        return false;
                    }
                    else if( abs(H.y() - cur.start.y() ) <= d || abs(H.y() - cur.end.y() ) <= d)
                    {//H не находится на cur
                        return false;
                    }
                }
                else if( (H.y() - cur.start.y()) * (H.y() - cur.end.y() )<=0 )
                {//H находится на cur
                    contact1=true;
                    if( abs(H.x() - line.start.x() ) <= d || abs(H.x() - line.end.x() ) <= d)
                        return false;
                }
            }
        }
        //--
        else
        {//горизонтальная
            if(line.start.y() == line.end.y())
            {//также горизонтальная
                bool flag=false;
                if( (cur.start.x() - line.start.x()) * (cur.start.x() - line.end.x() )<=0 )
                    flag = true;
                else if( (cur.end.x() - line.start.x()) * (cur.end.x() - line.end.x() )<=0 )
                    flag = true;
                //
                else if( (line.start.x() - cur.start.x()) * (line.start.x() - cur.end.x() )<=0 )
                    flag = true;
                else if( (line.end.x() - cur.start.x()) * (line.end.x() - cur.end.x() )<=0 )
                    flag = true;

                if(flag)
                {
                    contact1=true;
                    if( abs(cur.start.y() - line.start.y()) < d )
                        return false;
                }
            }
            else
            {//вертикальная, линии перпендикулярны
                QPoint H(line.start.x(), cur.start.y());//точка пересечения
                if( (H.y() - line.start.y()) * (H.y() - line.end.y() )<=0 )
                {//H находится на line
                    contact1=true;
                    if( (H.x() - cur.start.x()) * (H.x() - cur.end.x() )<=0 )
                    {//H находится на cur
                        return false;
                    }
                    else
                    {
                        if( (abs(H.x() - cur.start.x() )) <= d || (abs(H.x() - cur.end.x() )) <= d)
                        {//H не находится на cur, проверка расстояния
                            qDebug()<<abs(H.x() - cur.start.x() );
                            qDebug()<<abs(H.x() - cur.end.x() );
                            qDebug()<<"joi2";
                            return false;
                        }
                    }
                }
                else if( (H.x() - cur.start.x()) * (H.x() - cur.end.x() )<=0 )//
                {//H находится на cur
                    contact1=true;
                    if( abs(H.y() - line.start.y() ) <= d || abs(H.y() - line.end.y() ) <= d)
                        return false;
                }
            }
        }
        //--
        if(!contact1)
        {
            if( sqrt(pow( line.start.x() - cur.start.x(), 2)+pow( line.start.y() - cur.start.y(), 2))<d )
                return false;
            else if( sqrt(pow( line.start.x() - cur.end.x(), 2)+pow( line.start.y() - cur.end.y(), 2))<d )
                return false;
            //
            else if( sqrt(pow( line.end.x() - cur.start.x(), 2)+pow( line.end.y() - cur.start.y(), 2))<d )
                return false;
            else if( sqrt(pow( line.end.x() - cur.end.x(), 2)+pow( line.end.y() - cur.end.y(), 2))<d )
                return false;
        }
        //
    }
    return true;
}

void WireOfE::mowLine(int num, int h, int w)//сдвиг линии по Х(h)
{
    w=0;
    num=1;
    dotLine[num].setX(dotLine[num].x() + h);
    dotLine[num+1].setX(dotLine[num+1].x() + h);

}

QList<Line> WireOfE::getLines()
{
    QList<Line> wire;//
    for(int i=0; i < dotLine.size()-1; i++)
    {
        Line L;
        L.start = dotLine[i];
        L.end = dotLine[i+1];
        wire.append(L);
    }
    return wire;
}
