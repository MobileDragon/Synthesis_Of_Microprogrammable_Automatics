#include "paintwidget.h"

#include <QDebug>


#include <QMessageBox>
PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    mDrawMode = false;
    activE=-1;
    activW=-1;
    dh=40;//промежуток между элементами в высоту 20
    dw=70;//промежуток между рангами элементов в ширину 40 80
    //this->update();
    //this->setToolTip("qw\nr/ner\nww");
    this->setMouseTracking(true);

    this->installEventFilter(this);
    time=1;
    levelsG="| ";
}

PaintWidget::~PaintWidget()
{

}

int PaintWidget::whotElement(ShemE* element)
{
    if(element->wire.size() == 0)
        return 2;//выход

    QRegExp reg("[0-9]+");
    if(reg.exactMatch(element->name) )//имя содержит только цифры
        return 3;//вилка

    if(element->name == element->id)
        return 0;//вход

    if(element->wire.size() == 1)
        return 1;//простой элемент с одним выходом
}

bool PaintWidget::isEntryBridge(QPoint newBre)
{
    foreach(QPoint tp, bridges)
    {
        if(tp.x() == newBre.x() && tp.y() == newBre.y())
        {
            bridges.append(newBre);
            return false;
        }
        return true;
    }
}

void PaintWidget::setShem(Shem* sh)//установка Shem
{
    shem=sh;
    CurrentShem* current = sh->start;
    CurrentShem* previous = 0;

    QMap<QString, Element*> mapElement;//для быстрого обращения
    int rung=1;//везде от единицы//ранг элементов
    do
    {

        foreach(ShemE* element, current->elements)//каждый элемент текущего указателя
        {//подключение элементов
            Element* obj=0;
            switch(whotElement(element))//возвращает номер типа элемента
            {
                case 0:obj = new ElementalInput(element->name,element->id);
                break;
                case 1:obj = new ElementalE(element->name,element->id);
                break;
                case 2:obj = new ElementalOutput(element->name,element->id);
                break;
                case 3:obj = new ElementalWilka(element->name,element->id);
            }
            //нет ли уже элемента с данным id и name
            //если есть то obj=тот_что_найден и addElement отменяется
            bool alredyC=false;
            foreach(Element* objT, eBuffer)
            {
                if(objT->id == obj->id && objT->name == obj->name)
                {
                    obj=objT;
                    alredyC = true;
                    break;
                }
            }
            if(!alredyC)
                addElement(obj);
            MapRung.insert(obj,rung);//подпись рангов



            //создание проводов

            if(previous != 0)
            {//подключение провода к текущему элементу

                foreach(ShemE* element2, previous->elements)//каждый элемент предыдущего указателя
                {
                    //если этот элемент содержит провод на добавленый элемент
                    foreach(int key, element2->wire.keys())//номера выходов
                    {
                        if(element2->wire[key]->nextE->id == obj->id)
                        {
                            //из предыдущего указателя найден провод на текущий элемент
                            //нужен указатель на obj содержащий данный провод

                            Element* start = mapElement[element2->id];
                            int startNumOut = key;


                            Element* end = obj;
                            int endNumInput = element2->wire[key]->nextWire;
                            WireOfE *w = new WireOfE(start,end,startNumOut,endNumInput);
                            //был ли уже такой провод?
                            bool flag = false;
                            foreach(WireOfE *wire, wBuffer)
                            {
                                if(wire->startP == w->startP && wire->endP == w->endP)
                                {//номер выхода из элемента
                                    if(wire->startN == w->startN && wire->endN == w->endN)
                                    {//номер входа в следующий
                                        flag = true;
                                        break;
                                    }
                                }
                            }
                            if(!flag)
                                addWire(w);//провод установлен
                        }
                    }
                }
            }
            mapElement.insert(obj->id ,obj);//добавляются в коце чтобы вилки не затерли предшествинников раньше времени
                                                //чтобы предшественники могли сослаться на свои вилки
        }
        previous = current;
        current = Shem::next(current);
        rung++;
    }while(current->elements.size()>0);//работает
    //setAutoposityon();
    setGrid();//установка позиций для росстановки элементов
    paintFirst();
    //
    QList<Element *> curListE;
    foreach(ShemE* n_e, sh->start->elements)//вытягивать элементы и провода(выходы элементов)
    {//заполнение списком начальных элементов
        curListE.append(getELink(n_e->id) );
    }
    autoCorrected(curListE, QList<Line>() );//++++++++++++++++++++++
}

void PaintWidget::addElement(Element *obj)
{
    eBuffer.append(obj);
}
void PaintWidget::cleanElements()
{
    eBuffer.clear();
}
void PaintWidget::addWire(WireOfE *obj)
{
    wBuffer.append(obj);
}

void PaintWidget::setAutoVerticalLayout()//mapLevel должен быть проинициализирован
{
    QMap<int,int> hightAtRung;//вспомогательная, текущая высота столба в ранге
    foreach(Element* e, mapLevel.keys())
    {
        int posX=10;
        int curRung = MapRung[e];
        foreach(Element* e2, mapLevel.keys())
        {
            int tempRung = MapRung[e2];
            if(tempRung == curRung)
            {
                if(mapLevel[e2] < mapLevel[e])
                    posX = posX + dh + e2->getHeight();
            }
        }
        e->setPosY(posX);
    }


    /*
    QMap<int,int> hightAtRung;//вспомогательная, текущая высота столба в ранге
    for(int i=0;i<eBuffer.size();i++)/////////////////////////////////////////////
    {
        int curRung = MapRung[eBuffer[i]];
        if(hightAtRung[curRung] == 0)
            hightAtRung[curRung]=10;
        eBuffer[i]->setPosY(hightAtRung[curRung]);
        hightAtRung.insert(curRung, hightAtRung[curRung]+dh+eBuffer[i]->getHeight());
    }
      */
}

void PaintWidget::setGrid()
{
    QMap<int,int> kolAtRung;//вспомогательная, хранит номер ранга и число элемнтов которые уже были с данным рангом
    //QMap<int,int> hightAtRung;//вспомогательная, высота столба в ранге
    int rung=0;
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        kolAtRung.insert(rung,kolAtRung[rung]+1 );//увеличивает число элементов которые уже расставлены
        mapLevel.insert(eR,kolAtRung[rung]);//вставка элемента с его уровнем(в высоту)
    }
    QMap<int,int> map_width;//ключ-ранг, значение-ширина
    //запись ширин столбцов
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        if(map_width[rung] < eR->getWidth() )
            map_width[rung] = eR->getWidth();
    }
    QMap<int,int> map_posX;//ключ-ранг, значение-позиция столба по У
    map_posX.insert(1,dw);//установка позиции для элементов первого столба
    foreach(int curRung, map_width.keys())
    {
        map_posX.insert(curRung+1, map_posX[curRung]+map_width[curRung]+dw);
    }//map_posY хранит позицию позицию для элемента после последнего(неиспользуется)

    for(int i=0;i<eBuffer.size();i++)//установка позиции
    {
        int curRung = MapRung[eBuffer[i]];

        eBuffer[i]->setPosX(map_posX[curRung]);//установка позиции
    }
    setAutoVerticalLayout();

}
//--------------------------------------------------------

void PaintWidget::paintFirst()
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

    for(int i=0;i<wBuffer.size();i++)
    {
        bool flag = false;
        foreach(WireOfE* w, static_wire)
            if(w== wBuffer[i])
            {
                flag=true;
                break;
            }
        if(!flag)
        {
            wBuffer[i]->defaultW();//задать вызов только для не статичных(static_wire)
        }
        else
        {
            //поскольку провод уже статичен
            int kol=0;
            QList<Line> lines=wBuffer[i]->getLines();//линии статичного провода
            int j=0;
            while(kol < lines.size())
            {
                Line LS = static_Line[j];
                foreach(Line LW, lines)
                    if(LS.start.x()==LW.start.x() && LS.start.y()==LW.start.y())
                    {
                        static_Line.removeAt(j);
                        j--;
                        kol++;
                        break;
                    }
                j++;
            }
//
            wBuffer[i]->updateStart();
//
            QList<Line> linesw=wBuffer[i]->getLines();
            foreach(Line LW, linesw)
            {
                static_Line.append(LW);
            }

        }
        wBuffer[i]->draw(&painter);
    }


}

Element* PaintWidget::getELink(QString id)
{
    for(int i=0;i<eBuffer.size();i++)
    {
        if(eBuffer[i]->id==id)
            return eBuffer[i];
    }
}

QList<WireOfE *> PaintWidget::getWLink(ShemE* n_e)
{//вернуть массив ссылок на провода отрисовки
    QList<WireOfE *> wires;//провода на выходах элементов

        //ссылка на начальный и конечный элемент совпадает, а еще номера ывхода и входа тоже
        foreach(int numW, n_e->wire.keys() )//каждый выход на схеме
        {
            currentWire* wireS = n_e->wire[numW];
            ShemE* startS=n_e;
            ShemE* endS=wireS->nextE;
            //данные об искомом проводе
            Element* startP = getELink(startS->id);
            Element* endP = getELink(endS->id);
            int startNumOut = numW;
            int endNumInput = wireS->nextWire;
            //
            for(int i=0;i<wBuffer.size();i++)
            {
                if(wBuffer[i]->startP==startP && wBuffer[i]->endP==endP)
                    if(wBuffer[i]->startN == startNumOut && wBuffer[i]->endN == endNumInput)
                        wires.append(wBuffer[i]);
            }

        }
        return wires;

}


Element * PaintWidget::chenge_level(Element *wire, QList<int> old_numbers)
{
    //в shem номера выходов не меняются
        //номера входов в элементе к уоторому подключается провод
        //номера входов начинаются с 0
        //QList<int> listNum;
        Element* W2=0;


        int curNumEnd = mapLevel[wire];
        /*if(curNumEnd==1)
            return false;*/
        //количество
        int rungt=MapRung[wire];
        int kol=0;
        foreach(Element *w, MapRung.keys())
        {
            if(MapRung[w]==rungt)
                kol++;
        }

        //int kol = wire->endP->getNumInput();
        for(int i=1; i <= kol; i++)//входы элемента
        {
            bool flag = false;
            foreach(int num ,old_numbers)
            {
                if(num == i)
                {
                    flag = true;
                    break;
                }
            }
            if(!flag && i!=curNumEnd)
            {//усть вход для этого элемента, который раньше не использовался

                //
                foreach(Element *curW, eBuffer)
                {//провд который подключен на этот вход
                    if(mapLevel[curW]==i && MapRung[curW]==rungt )
                    {
                        mapLevel[curW] = curNumEnd;
                        W2 = curW;
                        break;
                    }

                }
                //
                mapLevel[wire]=i;

                return W2;
            }
        }
        return W2;
}

WireOfE * PaintWidget::chenge_out(WireOfE *wire, QList<int> old_numbers)//возвращает ссылку на второй провод
{//в shem номера выходов не меняются
    //номера входов в элементе к уоторому подключается провод
    //номера входов начинаются с 0
    //QList<int> listNum;
    WireOfE* W2=0;

    int curNumEnd = wire->endN;
    //if(curNumEnd==1)
      //  return false;

    int kol = wire->endP->getNumInput();
    for(int i=0; i < kol; i++)//входы элемента
    {
        bool flag = false;
        foreach(int num ,old_numbers)
        {
            if(num == i)
            {
                flag = true;
                break;
            }
        }
        if(!flag && i!=curNumEnd)
        {//усть вход для этого элемента, который раньше не использовался

            //
            foreach(WireOfE *curW, wBuffer)
            {//провд который подключен на этот вход
                if(curW->endP ==  wire->endP && curW->endN == i)
                {
                    curW->endN = curNumEnd;
                    W2 = curW;
                    break;
                }
            }
            //
            wire->endN=i;

            return W2;
        }
    }
    return W2;
}
//
int PaintWidget::chengeElementsLevel(int i,int search,QList<LevelsElement> &usingOuts )
{
    for(int n = 0; n < usingOuts.size(); n++)
    {
        qDebug()<<"| "<< usingOuts[n].cur_old_levels.size();
        //usingOuts[n].cur_old_levels.clear();
    }
    //i-номер текущего провода в списке
        int cur_n_for_chenge=search;//номер изменяемого провода
        if(search > i)
        {
            cur_n_for_chenge = chengeElementsLevel(i+1, search, usingOuts );
            //if(cur_n_for_chenge == i)

        }
        if(cur_n_for_chenge == i)
        {
            int oldOut =mapLevel[usingOuts[i].e];//номер выхода который заменится

            Element* W2=chenge_level(usingOuts[i].e, usingOuts[i].cur_old_levels);
            if(W2!=0 )//выход удалось помеять
            {//удалось поменять выход
                usingOuts[i].cur_old_levels.append(oldOut);
                //также записать старый выход для провода с которым произошла замена

                for(int n=0; n < usingOuts.size(); n++)
                {//если элемента нет то он и не записывается
                    LevelsElement outWire=usingOuts[n];
                    if(outWire.e == W2)
                    {
                        usingOuts[n].cur_old_levels.append(mapLevel[usingOuts[i].e]);
                        break;
                    }
                }
                return (i);
            }
            else
            {//уже перебраны все варианты подключений
             //нижестоящих(в списке) проводов  //рангов
                for(int n = i; n < usingOuts.size(); n++)
                {
                    usingOuts[n].cur_old_levels.clear();
                }
                //изменить положение вышестоящего элемента
                return (i-1);//search
            }
        }

/*
        if(cur_n_for_chenge == search)//это была самая вложеная
            return search;
        else//*/
            return (search);
}



int PaintWidget::chengeWiresConnected(int i,int search,QList<OutsWire>& usingOuts )
{//i-номер текущего провода в списке
    int cur_n_for_chenge=search;//номер изменяемого провода
    if(search > i)
    {
        cur_n_for_chenge = chengeWiresConnected(i+1, search, usingOuts );
        //if(cur_n_for_chenge == i)

    }
    if(cur_n_for_chenge == i)
    {
        int oldOut = usingOuts[i].wire->endN;//номер выхода который заменится

        WireOfE* W2=chenge_out(usingOuts[i].wire, usingOuts[i].cur_old_outs);
        if(W2!=0 )//выход удалось помеять
        {//удалось поменять выход
            usingOuts[i].cur_old_outs.append(oldOut);
            //также записать старый выход для провода с которым произошла замена

            for(int n=0; n < usingOuts.size(); n++)
            {
                OutsWire outWire=usingOuts[n];
                if(outWire.wire == W2)
                {
                    usingOuts[n].cur_old_outs.append(usingOuts[i].wire->endN);
                    break;
                }
            }
            return (i);
        }
        else
        {//уже перебраны все варианты подключений
         //нижестоящих(в списке) проводов
            for(int n = i; n < usingOuts.size(); n++)
            {
                usingOuts[n].cur_old_outs.clear();
            }
            //изменить положение вышестоящего элемента
            return (i-1);//search
        }
    }

/*
    if(cur_n_for_chenge == search)//это была самая вложеная
        return search;
    else//*/
        return (search);
}

//

QList<Element*> PaintWidget::next(QList<Element*> curListE)
{
    QList<Element*> nextListE;
    time++;
    int rungN=MapRung[curListE[0]]+1;
    foreach(Element*e, eBuffer)
    {
        if(MapRung[e]==rungN )
            nextListE.append(e);
    }
    return nextListE;
}
/*QList<Element*> PaintWidget::next(QList<Element*> curListE)
{
    QList<WireOfE*> wires_out;
    QList<Element*> nextListE;
    QMap<Element*, int> numInput;//хранит число проводов которые сейчас найдены
    foreach(WireOfE*wire, wBuffer)
    {
        foreach(Element* curElement, curListE)
            if(curElement == wire->startP)
            {
                wires_out.append(wire);
                numInput.insert(wire->endP,numInput[wire->endP]+1 );
            }
    }
    foreach(Element* curElement, numInput.keys())
    {
        if(numInput[curElement] == curElement->getNumInput() )
            nextListE.append(curElement);
        else
        {//найдены не все входы
            foreach(WireOfE*wire, wires_out)
            {
                if(wire->endP == curElement)
                {//провод который указывает на этот элемент
                    bool flag=false;
                    foreach(Element* e, nextListE)
                    {//если элемента что ссылается eще небыло
                        if(e == wire->startP)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if(!flag)
                        nextListE.append(wire->startP);//элемент в начале этого провода
                }
            }
        }
    }
    return nextListE;
}*/

//--
void PaintWidget::autoCorrected(QList<Element*> curListER, QList<Line>linesToPrevE)//рекурсия
{
    QList<Element*> curListE;
    //
    QList<Line>linesToCurE;
    foreach(Element* n_e, curListER)
    {
        foreach(WireOfE* w ,wBuffer)//
        {//элементы что указывают на текущий элемент
            if(w->endP == n_e)
            {
                bool isFind = false;
                foreach(Element* e, curListE)
                    if(e==w->startP)
                    {
                        isFind=true;
                        break;
                    }
                if(!isFind)
                    curListE.append(w->startP);
            }
        }
    }

    linesToPrevE=linesToCurE;//---
    QList<Element*> nextT = next(curListER);
    if(nextT.size() > 0)
        autoCorrected(nextT, linesToCurE);
    //
    if(curListE.size()==0)
        return;
    QList<OutsWire> usingOuts;//список проводов и выходов ими используемых
    QList<LevelsElement> usingLevels;//список элементов и уровней ими используемых
    //

    QList<Line> temp_Line;//список линий для элементов расположеных на текущем cur(элементы могут потом менять уровень)
    QList<WireOfE *> temp_Wire;//провода длякорректировки

    QMap<int,int> list_rungs;//номер ранга
    QMap<Element*,int>next_elements;//список элементов на которые можно попасть из текущих
    //он может быть не таким как next если имеются элементы с более одним выходом
    foreach(Element* n_e, curListE)//вытягивать элементы и провода(выходы элементов)
    {//заполнение
        QList<Line> e_Line=descriptElementLike3Line(n_e );//
        temp_Line.append(e_Line[0]);
        temp_Line.append(e_Line[1]);
        temp_Line.append(e_Line[2]);
        //
        list_rungs.insert(MapRung[n_e],0);
        //
        foreach(WireOfE * w, wBuffer)
        {
            if(w->startP == n_e)//вилки тоже отлавливаются
                next_elements.insert(w->endP,0);
        }


        QList<WireOfE *> tempw;
        foreach(WireOfE* wire, wBuffer)
        {//провода что выходят из элементов
            if(wire->startP == n_e)
                tempw.append(wire);
        }
        foreach(WireOfE * w, tempw)
        {//
            temp_Wire.append(w);
            OutsWire out;
            out.wire=w;
            //usingOuts.append(out);
        }
        //
        //LevelsElement out;
        //out.e=n_e;
        //usingLevels.append(out);

    }
    foreach(Element *e, eBuffer)
    {
        foreach(int rung, list_rungs.keys())
            if(MapRung[e] == rung)
            {
                LevelsElement elementWithLevels;
                elementWithLevels.e=e;
                usingLevels.append(elementWithLevels);
                //break;
            }
    }
    //

    foreach(Element *e, next_elements.keys())//что-то не так---------------------------------------------------
    {
        foreach(WireOfE * w, wBuffer)
        {
            if(w->endP==e)
            {
                OutsWire out;
                out.wire=w;
                usingOuts.append(out);
                //break;
            }
        }
    }

    //
    int cur_n_e=usingLevels.size()-1;
    int cur_n_w=usingOuts.size()-1;//провод который первый сменит положение
    //проверка если не удается проложить провода
    bool flag =true;
    //
    /*
    foreach(Element *e, mapLevel.keys())
    {
        qDebug()<<e->name;
        qDebug()<<mapLevel[e];
    }
*/
    qDebug()<<"ttrtrtrtrrttrtrrrtrtyyyyyyyyyyttrtrrrtrtyyyyyyyyyyyyttrtrrrtrtyyyyyyyyyyyyyyyyyy";
    /*
    foreach(LevelsElement ow, usingLevels)
    {
        ow.cur_old_levels.clear();
    }*/
    cur_n_e=usingLevels.size()-1;
    WireOfE * broken_wire=0;
    do
    {
        QList<Line> static_LineTemp;
        foreach(Line L4, linesToPrevE)
            static_LineTemp.append(L4);
        foreach(Line L5, static_Line)
            static_LineTemp.append(L5);
        foreach(Line L6, temp_Line)//при перестановки элементов менять список синий описыв текущ элементы
            static_LineTemp.append(L6);
        foreach(WireOfE * w, temp_Wire)
        {
            flag = w->generic(static_LineTemp);
            if(!flag)
            {
                broken_wire = w;
                break;
            }
            //
            QList<Line> wireL=w->getLines();
            foreach(Line L, wireL)//линии проводов
            {//не стыковаться с проводом после его размещения
                static_LineTemp.append(L);
            }
        }
        if(!flag)
        {
            QMessageBox pausa;
            pausa.setText( "pause" );
            //pausa.exec();

            QMessageBox msgBox;
            msgBox.setText( QString("wair") );
            //msgBox.exec();
            cur_n_e = chengeElementsLevel(0,cur_n_e,usingLevels);

            levelsG.append(QString(":%1 ").arg(cur_n_e) );

/*
            if(cur_n_e==-1)//перебор завершен
            {


                cur_n_e = usingLevels.size()-1;
                cur_n_w = chengeWiresConnected(0,cur_n_w,usingOuts );


                if(cur_n_w==-1)
                {

                    QMessageBox msgBox;
                    msgBox.setText( levelsG );
                    msgBox.exec();

                    flag = true;
                }

                foreach(Element *e, mapLevel.keys())
                {
                    qDebug()<<e->name;
                    qDebug()<<mapLevel[e];
                }

            }*/
            if(cur_n_e==-1)
                flag = true;
            setAutoVerticalLayout();//обновление позиции элементов из-за смены уровней
            paintFirst();//переподключение проводов по новым номерам
        }
        //flag =false //менять выходы местами//менять входы местами(Level элементов)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }while(!flag);


    //
    /*
    QMessageBox msgBox;
    msgBox.setText( QString("fail") );
    msgBox.exec();*/
    //

    //запись объектов для проверки следующих
    foreach(WireOfE * w, temp_Wire)
    {
        static_wire.append(w);

        QList<Line> wire=w->getLines();
        foreach(Line L, wire)//линии проводов
        {
            static_Line.append(L);
        }
    }
    foreach(Line L, temp_Line)//линии элементов
    {
        static_Line.append(L);
    }
    //запись окончена рекурсия переходит на уровень выше
}
QList<Line> PaintWidget::descriptElementLike3Line(Element* e)
{//добавить 3-ю линию

    QList <Line> lines;
    Line L1;
    L1.start = QPoint(e->getX(), e->getY() );
    L1.end = QPoint(e->getX() + e->getWidth()+10, e->getY());
    lines.append(L1);
    Line L2;
    L2.start = QPoint(e->getX() + e->getWidth()+3, e->getY() );
    L2.end = QPoint(e->getX() + e->getWidth()+3, e->getY() + e->getHeight() );
    lines.append(L2);
    Line L3;
    L3.start = QPoint(e->getX(), e->getY() + e->getHeight() );
    L3.end = QPoint(e->getX() + e->getWidth()+10, e->getY() + e->getHeight() );
    lines.append(L3);
    return lines;
}

//------------------------------------------------------------

void PaintWidget::setAutoposityon()//старо
{
    //нужна карта хранящая указатель и уроверь(в вниз спукаясь какой это элемент по счету в ранге данного элемента)
    //mapLevel;//Level тоже с 1 начинается как и Rung
    QMap<int,int> kolAtRung;//вспомогательная, хранит номер ранга и число элемнтов которые уже были с данным рангом
    int rung=0;
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        kolAtRung.insert(rung,kolAtRung[rung]+1 );//увелисивает число элементов которые уже расставлены
        mapLevel.insert(eR,kolAtRung[rung]);//вставка элемента с его уровнем(в высоту), который нужно еще изменить
    }
    for(int i=0;i<eBuffer.size();i++)//установка позиции
    {
        eBuffer[i]->setPos(MapRung[eBuffer[i]]*50, mapLevel[eBuffer[i]]*50);
        //eBuffer.insert();
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{

    setWhatsThis("dsf");
    //QWhatsThis::showText(event->pos(),"текст сообщения");
    // //
    bool flag = false;
    for(int i=0;i<eBuffer.size();i++)
    {
        if(eBuffer[i]->isOnclic(event->pos().x(), event->pos().y()))
        {
            activE=i;
            flag = true;
            break;
        }
    }

    if(!flag)
    {
        for(int i=0;i<wBuffer.size();i++)
        {
            if(wBuffer[i]->isOnclic(event->pos().x(), event->pos().y()))
            {
                activW=i;
                break;
            }
        }
    }
    //
    if (event->button() == Qt::LeftButton)
    {
        mDrawMode = true;
        event->accept();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        activE=-1;
        activW=-1;
        //
        mDrawMode = false;
        this->update();
        event->accept();
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!mDrawMode)
        return;
    if(activE>=0)
        eBuffer[activE]->move(event->pos().x(), event->pos().y());
    if(activW>=0)
        wBuffer[activW]->move(event->pos().x() );
    this->update();
    event->accept();
// //

    //setMouseTracking(true);
    //QEvent::MouseTrackingChange
//this->setToolTip( QString("nwwvb %1").arg(time++) );
    //this->setToolTip("qw\nr/ner\nww");
}


bool PaintWidget::eventFilter( QObject *obj, QEvent *evt )
{
  gBox.exec();*/
    //

    //Р·Р°РїРёСЃСЊ РѕР±СЉРµРєС‚РѕРІ РґР»СЏ РїСЂРѕРІРµСЂРєРё СЃР»РµРґСѓСЋС‰РёС…
    foreach(WireOfE * w, temp_Wire)
    {
        static_wire.append(w);

        QList<Line> wire=w->getLines();
        foreach(Line L, wire)//Р»РёРЅРёРё РїСЂРѕРІРѕРґРѕРІ
        {
            static_Line.append(L);
        }
    }
    foreach(Line L, temp_Line)//Р»РёРЅРёРё СЌР»РµРјРµРЅС‚РѕРІ
    {
        static_Line.append(L);
    }
    //Р·Р°РїРёСЃСЊ РѕРєРѕРЅС‡РµРЅР° СЂРµРєСѓСЂСЃРёСЏ РїРµСЂРµС…РѕРґРёС‚ РЅР° СѓСЂРѕРІРµРЅСЊ РІС‹С€Рµ
}
QList<Line> PaintWidget::descriptElementLike3Line(Element* e)
{//РґРѕР±Р°РІРёС‚СЊ 3-СЋ Р»РёРЅРёСЋ

    QList <Line> lines;
    Line L1;
    L1.start = QPoint(e->getX(), e->getY() );
    L1.end = QPoint(e->getX() + e->getWidth()+10, e->getY());
    lines.append(L1);
    Line L2;
    L2.start = QPoint(e->getX() + e->getWidth()+3, e->getY() );
    L2.end = QPoint(e->getX() + e->getWidth()+3, e->getY() + e->getHeight() );
    lines.append(L2);
    Line L3;
    L3.start = QPoint(e->getX(), e->getY() + e->getHeight() );
    L3.end = QPoint(e->getX() + e->getWidth()+10, e->getY() + e->getHeight() );
    lines.append(L3);
    return lines;
}

//------------------------------------------------------------

void PaintWidget::setAutoposityon()//СЃС‚Р°СЂРѕ
{
    //РЅСѓР¶РЅР° РєР°СЂС‚Р° С…СЂР°РЅСЏС‰Р°СЏ СѓРєР°Р·Р°С‚РµР»СЊ Рё СѓСЂРѕРІРµСЂСЊ(РІ РІРЅРёР· СЃРїСѓРєР°СЏСЃСЊ РєР°РєРѕР№ СЌС‚Рѕ СЌР»РµРјРµРЅС‚ РїРѕ СЃС‡РµС‚Сѓ РІ СЂР°РЅРіРµ РґР°РЅРЅРѕРіРѕ СЌР»РµРјРµРЅС‚Р°)
    //mapLevel;//Level С‚РѕР¶Рµ СЃ 1 РЅР°С‡РёРЅР°РµС‚СЃСЏ РєР°Рє Рё Rung
    QMap<int,int> kolAtRung;//РІСЃРїРѕРјРѕРіР°С‚РµР»СЊРЅР°СЏ, С…СЂР°РЅРёС‚ РЅРѕРјРµСЂ СЂР°РЅРіР° Рё С‡РёСЃР»Рѕ СЌР»РµРјРЅС‚РѕРІ РєРѕС‚РѕСЂС‹Рµ СѓР¶Рµ Р±С‹Р»Рё СЃ РґР°РЅРЅС‹Рј СЂР°РЅРіРѕРј
    int rung=0;
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        kolAtRung.insert(rung,kolAtRung[rung]+1 );//СѓРІРµР»РёСЃРёРІР°РµС‚ С‡РёСЃР»Рѕ СЌР»РµРјРµРЅС‚РѕРІ РєРѕС‚РѕСЂС‹Рµ СѓР¶Рµ СЂР°СЃСЃС‚Р°РІР»РµРЅС‹
        mapLevel.insert(eR,kolAtRung[rung]);//РІСЃС‚Р°РІРєР° СЌР»РµРјРµРЅС‚Р° СЃ РµРіРѕ СѓСЂРѕРІРЅРµРј(РІ РІС‹СЃРѕС‚Сѓ), РєРѕС‚РѕСЂС‹Р№ РЅСѓР¶РЅРѕ РµС‰Рµ РёР·РјРµРЅРёС‚СЊ
    }
    for(int i=0;i<eBuffer.size();i++)//СѓСЃС‚Р°РЅРѕРІРєР° РїРѕР·РёС†РёРё
    {
        eBuffer[i]->setPos(MapRung[eBuffer[i]]*50, mapLevel[eBuffer[i]]*50);
        //eBuffer.insert();
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{

    setWhatsThis("dsf");
    //QWhatsThis::showText(event->p