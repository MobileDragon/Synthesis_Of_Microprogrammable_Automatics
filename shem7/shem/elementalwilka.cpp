#include "elementalwilka.h"

ElementalWilka::ElementalWilka()
{
}

ElementalWilka::ElementalWilka(QString name)
{
    generic(name);
}

ElementalWilka::ElementalWilka(QString name, QString id):Element(name, id)//�������� ����, � ����� ���� � ���������
{
    generic(name);//id ������� �� ���� ����� �������������� ��������
    this->id=id;
}

void ElementalWilka::draw(QPainter *painter)
{
    painter->setBrush(QBrush (Qt::black));
    //painter->drawEllipse(posx, posy, width, 6);
    painter->setBrush( QBrush (Qt::transparent) );

    painter->drawLine(posx+6/2-4,posy+width/2+3,posx+6/2, posy+width/2+3 );
    painter->drawLine(posx+6, posy+width/2,posx+6, posy+width/2 + interval*(numOutput-1) );
    wiresPos.insert(0,QPoint(posx+6/2-4,posy+width/2+3) );//����
    int kol=0;
    for(int i=1;i<=numOutput;i++)
    {
        painter->setBrush(QBrush (Qt::black));
        painter->drawEllipse(posx+6/2, posy+width/2 + interval*(i-1), width, 6);
        painter->setBrush( QBrush (Qt::transparent) );
        wiresPos.insert(i,QPoint(posx+6,posy+width/2 + interval*(i-1)+3 ) );//������
        kol++;
    }

    //painter->drawText(posx+5,posy+height/2,type);

}

void ElementalWilka::generic(QString name)//�������� name-��� ����� �������
{
    posx=10;
    posy=20;
    width=6;

    type=name;
    this->name=name;
    numOutput=type.toInt();//Comply::fillShema() ������ ����� �������
    height=interval*numOutput;
    qDebug()<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRY"<<numOutput;
    numInput=1;
    interval=10;//
}
