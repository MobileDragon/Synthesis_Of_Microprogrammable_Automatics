#include "sheme.h"

ShemE::ShemE()
{

}
ShemE::~ShemE()
{
    wire.clear();
}

ShemE::ShemE(ShemE &element)
{
    this->name=element.name;
    this->id=element.id;
    foreach(int key,element.wire.keys())
    {
        currentWire* y=new currentWire();
        this->wire.insert(key,y);
    }
}



ShemE::ShemE(QString name, QString id)//�������� ������ ������� �� ����
{
    QVector<int> massOutput = getOut(name);
    this->name=name;
    this->id=id;
    //�������� ����� �����������
    foreach(int n,massOutput)//massOutput �������� �� ����//�������� ��� ����� �������
    {
        this->wire.insert(n, new currentWire());
        //wire.value(1).nextWire=5;
    }
}

ShemE::ShemE(QString id,int kolOut)//�����
{
    this->name=QString("%1").arg(kolOut);//��� �������� ������ ����� ������������
    this->id=id;
    for(int i=1;i<=kolOut;i++)//������ �������
    {
        this->wire.insert(i, new currentWire());//������ ������� � �����������
    }
}


ShemE::ShemE(QString name)//������������ ��� ���������� ������(X)
{//id � name ������ ������������ ����������
    this->name=name;
    this->id=name;

    this->wire.insert(0, new currentWire());
}

ShemE::ShemE(QString name,QString id, bool)//������������ ��� ���������� ����������(rs)
{//id � name ������ ������������ ����������
    this->name=name;
    this->id=id;

    this->wire.insert(2, new currentWire());
    this->wire.insert(3, new currentWire());
}

ShemE::ShemE(int numY, QString nameOut)//������������ ��� ���������� �������(��� ����� � id)(Y)
{
    QString name;
    if(nameOut.length()<1)
    {
        name = QString("y%1").arg(numY);
    }
    else
        name = nameOut;
    this->name=name;
    this->id=name;
}


int ShemE::getFreeOut(ShemE* element)
{
    foreach(int numOut, element->wire.keys())
    {
        currentWire* wireOut=element->wire[numOut];
        if(wireOut->nextE==0)
            return numOut;
    }
    return 0;

}

QVector<int> ShemE::getInput(QString name)
{
    QVector<int> listInputs(0);
    if(name[0] == 'y' || name[0] == 'Y')
    {
        listInputs.append(0);
        return listInputs;
    }
    //� ���� ��� ������ ������:( ���� ����� ������ � ������� ��������� ������� ����� ����� ������:)
    //��������� ���������� � 0

    QString t="";
    QSqlQuery *query = BDE::getInstance()->make(t+"SELECT wires.numInput, elements.name"
                               +" FROM elements,wires where (wires.id=elements.idwires) and name LIKE '"
                               +name+"'");
    query->next();
    int numInput = query->value(0).toInt();
    for(int i=0;i<numInput;i++)
    {
        listInputs.append(i);
    }
    return listInputs;
}

QVector<int> ShemE::getOut(QString name)
{
    QVector<int> listOut(0);
    //� ���� ��� ������ ������:( ���� ����� ������ � ������� ��������� ������� ����� ����� ������:)
    //��������� ���������� � 0

    QString t="";
    QSqlQuery *queryOut = BDE::getInstance()->make(t+"SELECT wires.numOutput, elements.name"
                               +" FROM elements,wires where (wires.id=elements.idwires) and name LIKE '"
                               +name+"'");
    queryOut->next();
    int numOut = queryOut->value(0).toInt();

    QSqlQuery *queryIn = BDE::getInstance()->make(t+"SELECT wires.numInput, elements.name"
                               +" FROM elements,wires where (wires.id=elements.idwires) and name LIKE '"
                               +name+"'");
    queryIn->next();
    int numIn = queryIn->value(0).toInt();
    for(int i=numIn;i<numOut+numIn;i++)
    {
        listOut.append(i);
    }
    return listOut;
}
