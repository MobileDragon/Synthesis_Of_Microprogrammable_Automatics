#include "elementale.h"
#include <QVariant>

ElementalE::ElementalE()
{
    //generic();
}

ElementalE::ElementalE(QString nameElementFromBase):Element(nameElementFromBase)
{
    generic(nameElementFromBase);
}
ElementalE::ElementalE(QString nameElementFromBase, QString id):Element(nameElementFromBase, id)//добавить всем, а здесь пока к заголовку
{
    generic(nameElementFromBase);//id хранить по нему будет взаимодействие работать
    this->id=id;
}

void ElementalE::draw(QPainter *painter)
{
    painter->drawRect(posx, posy, width, height);
    int yd;//=height/(numInput+1);
    yd = (height - (numInput)*interval)/2 + interval/2;
    for(int i=0;i<numInput;i++)
    {
        int d=yd;
        yd += interval;
        painter->drawLine(posx,posy+d,posx-10,posy+d);
        wiresPos.insert(i,QPoint(posx-10,posy+d));//координаты входов(точки подключения) от 0
    }
    if(type=="!1" || type=="!&")
    {
        painter->drawEllipse(posx+width-2,posy+height/2-4,7,7);
        painter->drawLine(posx+width+5,posy+height/2,posx+width+10,posy+height/2);
    }
    else
        painter->drawLine(posx+width,posy+height/2,posx+width+10,posy+height/2);
    wiresPos.insert(numInput,QPoint(posx+width+10,posy+height/2));//координаты выхода
    painter->drawText(posx+5,posy+height/2,this->type);


}

void ElementalE::generic(QString nameElementFromBase)//заполняй
{
    posx=10;
    posy=20;//задается при построении схемы
    width=30;
    height=50;

    interval=10;
    name = nameElementFromBase;
    QString t="";
    QSqlQuery *query = BDE::getInstance()->make(t+"select elements.name, operations.operation, wires.numInput, wires.numOutput from elements, wires, operations where (elements.idwires=wires.id and elements.idoperations=operations.id) and name LIKE '"+nameElementFromBase+"'");

    //Выводим значения из запроса
    width=30;
    query->next();

    type = query->value(1).toString();
    numInput = query->value(2).toInt();
    numOutput = query->value(3).toInt();
    height= (numInput)*interval;
    if(height<width)
        height=width;





}
