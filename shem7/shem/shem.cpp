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
    if(start!=end)//если кроме начальных уже добавлены элементы
        end->elements.append(element);
}

void Shem::addBack(ShemE* element, QVector<QString> inputs)//inputs - список подключаемых входов
{
    QString name = element->name;
    QVector<int> numInputs=ShemE::getInput(name);//входы в добавл€емом элементе
    int num=0;
    foreach(QString input, inputs)//входы которые необходимо подключить
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
                break;//вилка не будет плностью вс€ указывать на данный элемент
            }
        }
    }
    end=Shem::next(end);//установка end в конец
}

void Shem::addWilka(QString id, int numOuts)
{
    int kolOut = end -> elements.size();
    for(int i = 0;i < kolOut;i++)
    {
        if(end -> elements[i]->id == id )
        {
            ShemE* e =new ShemE(id, numOuts);
            currentWire* wire = new currentWire(e, 0);//указатель на вилку
            int numOut = ShemE::getFreeOut(end -> elements[i] );
            end -> elements[i] -> wire[numOut] = wire;
            //currentWire* out=getFreeOut(end -> elements[i] );//вместо свободного провода записываетс€ тот что подключен
                                                   //к добавл€емому
            //out = wire;
            end=Shem::next(end);//установка end в конец
            return;
        }
    }
}





CurrentShem* Shem::next(CurrentShem * curent)
{
    QVector<ShemE*> tempElements;
    foreach(ShemE* element, curent->elements)//каждый элемент
    {

        int maxKol=element->wire.size();
        int tempKol=0;
        foreach(currentWire* value, element->wire.values())//каждый выход
        {
            bool flag=false;
            if(value->nextE != 0)
            {
                tempElements.append(value->nextE);
                /*
                //нет ли уже этого элемента в curent
                foreach(ShemE* element2, curent->elements)
                {
                    if(value->nextE == element2)
                    {
                        flag=true;//элемент уже есть
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
        //нужно когда не все входы зан€ты у элемента на который указывет провод
        if(tempKol != maxKol)//когда не все выходы зан€ты
            tempElements.append(element);//тот элемент который не имеет выходов(последний так вооюще никогда-у1,у2)
    }
    //убрать повтор€ющиес€ указатели
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
