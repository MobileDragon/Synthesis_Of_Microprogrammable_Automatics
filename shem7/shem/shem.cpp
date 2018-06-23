#include "shem.h"


Shem::Shem()
{
    start=0;
    end=0;
}

Shem::~Shem()
{
    CurrentShem* current;
    if(start==0)
        return;
    while(start!=end)
    {
        current=Shem::next(start);
        delete(start);
        start=current;
    }
    delete(start);
    start=end=0;
}



void Shem::addBack(ShemE* element)
{
    if(start==0)
    {
        CurrentShem* arr=new CurrentShem();
        arr->elements.append(element);
        start=arr;
        end=start;
        return;
    }
    start->elements.append(element);
    if(start!=end)//���� ����� ��������� ��� ��������� ��������
        end->elements.append(element);
}

void Shem::addBack(ShemE* element, QVector<QString> inputs)//inputs - ������ ������������ ������
{
    QString name = element->name;
    QVector<int> numInputs=ShemE::getInput(name);//����� � ����������� ��������
    int num=0;
    foreach(QString input, inputs)//����� ������� ���������� ����������
    {
        int kolOut = end -> elements.size();
        for(int i = 0;i < kolOut;i++)
        {
            if(end -> elements[i]->id == input )
            {
                //
                currentWire* wire = new currentWire(element, numInputs[num]);
                int numOut = ShemE::getFreeOut(end -> elements[i] );
                end -> elements[i] -> wire[numOut] = wire;
                num++;
                break;//����� �� ����� �������� ��� ��������� �� ������ �������
            }
        }
    }
    end=Shem::next(end);//��������� end � �����
}

void Shem::addWilka(QString id, int numOuts)
{
    int kolOut = end -> elements.size();
    for(int i = 0;i < kolOut;i++)
    {
        if(end -> elements[i]->id == id )
        {
            ShemE* e =new ShemE(id, numOuts);
            currentWire* wire = new currentWire(e, 0);//��������� �� �����
            int numOut = ShemE::getFreeOut(end -> elements[i] );
            end -> elements[i] -> wire[numOut] = wire;
            //currentWire* out=getFreeOut(end -> elements[i] );//������ ���������� ������� ������������ ��� ��� ���������
                                                   //� ������������
            //out = wire;
            end=Shem::next(end);//��������� end � �����
            return;
        }
    }
}





CurrentShem* Shem::next(CurrentShem * curent)
{
    QVector<ShemE*> tempElements;
    foreach(ShemE* element, curent->elements)//������ �������
    {

        int maxKol=element->wire.size();
        int tempKol=0;
        foreach(currentWire* value, element->wire.values())//������ �����
        {
            bool flag=false;
            if(value->nextE != 0)
            {
                tempElements.append(value->nextE);
                /*
                //��� �� ��� ����� �������� � curent
                foreach(ShemE* element2, curent->elements)
                {
                    if(value->nextE == element2)
                    {
                        flag=true;//������� ��� ����
                        break;
                    }
                }

                if(!flag)
                {
                    tempElements.append(value->nextE);
                }
                */
                tempKol++;
            }
        }
        //����� ����� �� ��� ����� ������ � �������� �� ������� �������� ������
        if(tempKol != maxKol)//����� �� ��� ������ ������
            tempElements.append(element);//��� ������� ������� �� ����� �������(��������� ��� ������ �������-�1,�2)
    }
    //������ ������������� ���������
    int length =tempElements.size();
    CurrentShem* select = new CurrentShem;

    for(int i=0; i < length; i++)
    {
        bool flag=true;
        for(int j=0; j < i; j++)
        {
            if(tempElements[i]==tempElements[j])
            {
                flag=false;
                break;
            }
        }
        if(flag)
            select->elements.append(tempElements[i]);
    }
    tempElements.clear();

    return select;
}
