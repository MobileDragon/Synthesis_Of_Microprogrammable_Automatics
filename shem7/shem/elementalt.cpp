#include "elementalt.h"
#include <QVariant>

ElementalT::ElementalT()
{
}

ElementalT::ElementalT(QString nameElementFromBase):Element(nameElementFromBase)
{
    generic(nameElementFromBase);
}
ElementalT::ElementalT(QString nameElementFromBase, QString id):Element(nameElementFromBase, id)//добавить всем, а здесь пока к заголовку
{
    generic(nameElementFromBase);//id хранить по нему будет взаимодействие работать
    this->id=id;
}

void ElementalT::draw(QPainter *painter)
{
    painter->setFont(QFont("Arial", 10, 100, 0) );

    painter->drawRect(posx, posy, 20, height);
    painter->drawRect(posx, posy, width, height);
    int yd;//=height/(numInput+1);
    yd = (height - (numInput)*interval)/2 + interval/2;
    for(int i=0;i<numInput;i++)
    {
        int d=yd;
        yd += interval;
        painter->drawLine(posx,posy+d,posx-10,posy+d);
        wiresPos.insert(i,QPoint(posx-10,posy+d));//координаты входов(точки подключения) от 0
        if(type=="RS")
        {
            QString in = "S";
            if(i==1)
            {
                in="R";
                painter->drawText(posx+26,posy+17,"T");
            }
            painter->drawText(posx+4,posy+d+4,in);
        }
        else
        {
            QString in = "T";
            if(i==1)
            {
                in="C";
                painter->drawText(posx+24,posy+17,"TT");
            }
            painter->drawText(posx+4,posy+d+4,in);
        }
    }

    yd = (height - (numOutput)*interval)/2 + interval/2;

    int d=yd;
    yd += interval;
    painter->drawLine(posx+width,posy+d,posx+width+10,posy+d);
    wiresPos.insert( numInput,QPoint(posx+width+10,posy+d) );//координаты входов(точки подключения) от 0

    d=yd;
    painter->drawEllipse(posx+width-2,posy+d-4,7,7);
    painter->drawLine(posx+width+5,posy+d,posx+width+10,posy+d);
    wiresPos.insert( 1+numInput,QPoint(posx+width+10,posy+d) );//координаты входов(точки подключения) от 0
}

void ElementalT::generic(QString nameElementFromBase)//заполняй
{
    posx=10;
    posy=20;//задается при построении схемы
    width=30;
    height=50;

    interval=25;
    name = nameElementFromBase;

    QSqlQuery *query = BDE::getInstance()->make(QString("select elements.name, operations.operation, wires.numInput, wires.numOutput from elements, wires, operations where (elements.idwires=wires.id and elements.idoperations=operations.id) and name LIKE '%1'").arg(nameElementFromBase));

    //Выводим значения из запроса
    width=45;
    height=70;
    query->next();

    type = query->value(1).toString();
    numInput = query->value(2).toInt();
    numOutput = query->value(3).toInt();
    //numInput=numOutput=2;

    height= (numInput)*interval;
    if(height<width)
        height=width;

}
