#include "paintauthomat.h"

paintAuthomat::paintAuthomat(QList<TransitionAuthomat> list_transit, PaintWidget *parent) :
    PaintWidget(parent)
{
    listTransit = list_transit;

    //������ ������� ���
    /*
    AuthomatBus* bus = new AuthomatBus(3, QPoint(2,10),150, 250);
    //bus.draw(this->pain);
    bBuffer.insert(1,bus);

    AuthomatBus* bus1 = new AuthomatBus(2, QPoint(4,12),150, 250);
    //bus.draw(this->pain);
    bBuffer.insert(2,bus1);*/
    spaceConnect=25;//���������� �� ��������� ��� ������� �� ���


    paintMainInputs();//��������� ������
}

void paintAuthomat::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    setFont(QFont("Arial", 12, 100, 0));
    //
    for(int i=0;i<eBuffer.size();i++)
    {

        //eBuffer[i]->setPos(MapRung[eBuffer[i]]*50, 30);
        eBuffer[i]->draw(&painter);
    }
    setFont(QFont("Arial", 12, 100, 0));
    for(int i=0;i<wBuffer.size();i++)
    {
        wBuffer[i]->draw(&painter);
        //���������� ����� �������� ������ �����
    }
    setFont(QFont("Arial", 12, 100, 0));

    painter.setPen(QPen(Qt::black,3,Qt::SolidLine));
    foreach(AuthomatBus* bus, bBuffer)
    {
        bus->draw(&painter);
    }

}

QList<QString> paintAuthomat::getListInputs()//����� � �������
{
    QList<QString> inputs;
    foreach(TransitionAuthomat transit, listTransit)
    {
        QString in = transit.idInput;
        if(inputs.indexOf(in) < 0 )
        {
            inputs.append(in);
        }
    }
    inputs.append("R");//����� ������
    return inputs;
}


void paintAuthomat::paintMainInputs()//��������� ������ ����(�� �������) � ����� �� ��
{
    QList<QString> inputs=getListInputs();
    //�������� ������ ���� U
    AuthomatBus* bus = new AuthomatBus(2, QPoint(40,20),150, 250);//������ � ������ � ���������� ��������������
    //bus.draw(this->pain);
    bBuffer.insert(1,bus);//������ ����


    int num=0;
    foreach(QString in, inputs)
    {
        if( (num+2) * spaceConnect > bus->getHeight()  )
            bus->setHeight((num+2) * spaceConnect);//��������� ����� ������ ��� ��������� ������

        Element* inB = new ElementalInAtBus(in, in, bus, 1, num * spaceConnect);
        Element* testOut = new ElementalInAtBus("out", "out", bus, 1, num * spaceConnect);//�����
        addElement(inB);
        num++;
    }
    Element* testOut = new ElementalOutFromBus("out", "out", bus, 2, num * spaceConnect);//�����
    addElement(testOut);
}
