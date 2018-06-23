#include "paintwidget.h"

#include <QDebug>


#include <QMessageBox>
PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    mDrawMode = false;
    activE=-1;
    activW=-1;
    dh=40;//���������� ����� ���������� � ������ 20
    dw=70;//���������� ����� ������� ��������� � ������ 40 80
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
        return 2;//�����

    QRegExp reg("[0-9]+");
    if(reg.exactMatch(element->name) )//��� �������� ������ �����
        return 3;//�����

    if(element->name == element->id)
        return 0;//����

    if(element->wire.size() == 1)
        return 1;//������� ������� � ����� �������
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

void PaintWidget::setShem(Shem* sh)//��������� Shem
{
    shem=sh;
    CurrentShem* current = sh->start;
    CurrentShem* previous = 0;

    QMap<QString, Element*> mapElement;//��� �������� ���������
    int rung=1;//����� �� �������//���� ���������
    do
    {

        foreach(ShemE* element, current->elements)//������ ������� �������� ���������
        {//����������� ���������
            Element* obj=0;
            switch(whotElement(element))//���������� ����� ���� ��������
            {
                case 0:obj = new ElementalInput(element->name,element->id);
                break;
                case 1:obj = new ElementalE(element->name,element->id);
                break;
                case 2:obj = new ElementalOutput(element->name,element->id);
                break;
                case 3:obj = new ElementalWilka(element->name,element->id);
            }
            //��� �� ��� �������� � ������ id � name
            //���� ���� �� obj=���_���_������ � addElement ����������
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
            MapRung.insert(obj,rung);//������� ������



            //�������� ��������

            if(previous != 0)
            {//����������� ������� � �������� ��������

                foreach(ShemE* element2, previous->elements)//������ ������� ����������� ���������
                {
                    //���� ���� ������� �������� ������ �� ���������� �������
                    foreach(int key, element2->wire.keys())//������ �������
                    {
                        if(element2->wire[key]->nextE->id == obj->id)
                        {
                            //�� ����������� ��������� ������ ������ �� ������� �������
                            //����� ��������� �� obj ���������� ������ ������

                            Element* start = mapElement[element2->id];
                            int startNumOut = key;


                            Element* end = obj;
                            int endNumInput = element2->wire[key]->nextWire;
                            WireOfE *w = new WireOfE(start,end,startNumOut,endNumInput);
                            //��� �� ��� ����� ������?
                            bool flag = false;
                            foreach(WireOfE *wire, wBuffer)
                            {
                                if(wire->startP == w->startP && wire->endP == w->endP)
                                {//����� ������ �� ��������
                                    if(wire->startN == w->startN && wire->endN == w->endN)
                                    {//����� ����� � ���������
                                        flag = true;
                                        break;
                                    }
                                }
                            }
                            if(!flag)
                                addWire(w);//������ ����������
                        }
                    }
                }
            }
            mapElement.insert(obj->id ,obj);//����������� � ���� ����� ����� �� ������� ���������������� ������ �������
                                                //����� ��������������� ����� ��������� �� ���� �����
        }
        previous = current;
        current = Shem::next(current);
        rung++;
    }while(current->elements.size()>0);//��������
    //setAutoposityon();
    setGrid();//��������� ������� ��� ����������� ���������
    paintFirst();
    //
    QList<Element *> curListE;
    foreach(ShemE* n_e, sh->start->elements)//���������� �������� � �������(������ ���������)
    {//���������� ������� ��������� ���������
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

void PaintWidget::setAutoVerticalLayout()//mapLevel ������ ���� ������������������
{
    QMap<int,int> hightAtRung;//���������������, ������� ������ ������ � �����
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
    QMap<int,int> hightAtRung;//���������������, ������� ������ ������ � �����
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
    QMap<int,int> kolAtRung;//���������������, ������ ����� ����� � ����� �������� ������� ��� ���� � ������ ������
    //QMap<int,int> hightAtRung;//���������������, ������ ������ � �����
    int rung=0;
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        kolAtRung.insert(rung,kolAtRung[rung]+1 );//����������� ����� ��������� ������� ��� �����������
        mapLevel.insert(eR,kolAtRung[rung]);//������� �������� � ��� �������(� ������)
    }
    QMap<int,int> map_width;//����-����, ��������-������
    //������ ����� ��������
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        if(map_width[rung] < eR->getWidth() )
            map_width[rung] = eR->getWidth();
    }
    QMap<int,int> map_posX;//����-����, ��������-������� ������ �� �
    map_posX.insert(1,dw);//��������� ������� ��� ��������� ������� ������
    foreach(int curRung, map_width.keys())
    {
        map_posX.insert(curRung+1, map_posX[curRung]+map_width[curRung]+dw);
    }//map_posY ������ ������� ������� ��� �������� ����� ����������(��������������)

    for(int i=0;i<eBuffer.size();i++)//��������� �������
    {
        int curRung = MapRung[eBuffer[i]];

        eBuffer[i]->setPosX(map_posX[curRung]);//��������� �������
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
            wBuffer[i]->defaultW();//������ ����� ������ ��� �� ���������(static_wire)
        }
        else
        {
            //��������� ������ ��� ��������
            int kol=0;
            QList<Line> lines=wBuffer[i]->getLines();//����� ���������� �������
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
{//������� ������ ������ �� ������� ���������
    QList<WireOfE *> wires;//������� �� ������� ���������

        //������ �� ��������� � �������� ������� ���������, � ��� ������ ������ � ����� ����
        foreach(int numW, n_e->wire.keys() )//������ ����� �� �����
        {
            currentWire* wireS = n_e->wire[numW];
            ShemE* startS=n_e;
            ShemE* endS=wireS->nextE;
            //������ �� ������� �������
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
    //� shem ������ ������� �� ��������
        //������ ������ � �������� � �������� ������������ ������
        //������ ������ ���������� � 0
        //QList<int> listNum;
        Element* W2=0;


        int curNumEnd = mapLevel[wire];
        /*if(curNumEnd==1)
            return false;*/
        //����������
        int rungt=MapRung[wire];
        int kol=0;
        foreach(Element *w, MapRung.keys())
        {
            if(MapRung[w]==rungt)
                kol++;
        }

        //int kol = wire->endP->getNumInput();
        for(int i=1; i <= kol; i++)//����� ��������
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
            {//���� ���� ��� ����� ��������, ������� ������ �� �������������

                //
                foreach(Element *curW, eBuffer)
                {//����� ������� ��������� �� ���� ����
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

WireOfE * PaintWidget::chenge_out(WireOfE *wire, QList<int> old_numbers)//���������� ������ �� ������ ������
{//� shem ������ ������� �� ��������
    //������ ������ � �������� � �������� ������������ ������
    //������ ������ ���������� � 0
    //QList<int> listNum;
    WireOfE* W2=0;

    int curNumEnd = wire->endN;
    //if(curNumEnd==1)
      //  return false;

    int kol = wire->endP->getNumInput();
    for(int i=0; i < kol; i++)//����� ��������
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
        {//���� ���� ��� ����� ��������, ������� ������ �� �������������

            //
            foreach(WireOfE *curW, wBuffer)
            {//����� ������� ��������� �� ���� ����
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
    //i-����� �������� ������� � ������
        int cur_n_for_chenge=search;//����� ����������� �������
        if(search > i)
        {
            cur_n_for_chenge = chengeElementsLevel(i+1, search, usingOuts );
            //if(cur_n_for_chenge == i)

        }
        if(cur_n_for_chenge == i)
        {
            int oldOut =mapLevel[usingOuts[i].e];//����� ������ ������� ���������

            Element* W2=chenge_level(usingOuts[i].e, usingOuts[i].cur_old_levels);
            if(W2!=0 )//����� ������� �������
            {//������� �������� �����
                usingOuts[i].cur_old_levels.append(oldOut);
                //����� �������� ������ ����� ��� ������� � ������� ��������� ������

                for(int n=0; n < usingOuts.size(); n++)
                {//���� �������� ��� �� �� � �� ������������
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
            {//��� ��������� ��� �������� �����������
             //�����������(� ������) ��������  //������
                for(int n = i; n < usingOuts.size(); n++)
                {
                    usingOuts[n].cur_old_levels.clear();
                }
                //�������� ��������� ������������ ��������
                return (i-1);//search
            }
        }

/*
        if(cur_n_for_chenge == search)//��� ���� ����� ��������
            return search;
        else//*/
            return (search);
}



int PaintWidget::chengeWiresConnected(int i,int search,QList<OutsWire>& usingOuts )
{//i-����� �������� ������� � ������
    int cur_n_for_chenge=search;//����� ����������� �������
    if(search > i)
    {
        cur_n_for_chenge = chengeWiresConnected(i+1, search, usingOuts );
        //if(cur_n_for_chenge == i)

    }
    if(cur_n_for_chenge == i)
    {
        int oldOut = usingOuts[i].wire->endN;//����� ������ ������� ���������

        WireOfE* W2=chenge_out(usingOuts[i].wire, usingOuts[i].cur_old_outs);
        if(W2!=0 )//����� ������� �������
        {//������� �������� �����
            usingOuts[i].cur_old_outs.append(oldOut);
            //����� �������� ������ ����� ��� ������� � ������� ��������� ������

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
        {//��� ��������� ��� �������� �����������
         //�����������(� ������) ��������
            for(int n = i; n < usingOuts.size(); n++)
            {
                usingOuts[n].cur_old_outs.clear();
            }
            //�������� ��������� ������������ ��������
            return (i-1);//search
        }
    }

/*
    if(cur_n_for_chenge == search)//��� ���� ����� ��������
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
    QMap<Element*, int> numInput;//������ ����� �������� ������� ������ �������
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
        {//������� �� ��� �����
            foreach(WireOfE*wire, wires_out)
            {
                if(wire->endP == curElement)
                {//������ ������� ��������� �� ���� �������
                    bool flag=false;
                    foreach(Element* e, nextListE)
                    {//���� �������� ��� ��������� e�� ������
                        if(e == wire->startP)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if(!flag)
                        nextListE.append(wire->startP);//������� � ������ ����� �������
                }
            }
        }
    }
    return nextListE;
}*/

//--
void PaintWidget::autoCorrected(QList<Element*> curListER, QList<Line>linesToPrevE)//��������
{
    QList<Element*> curListE;
    //
    QList<Line>linesToCurE;
    foreach(Element* n_e, curListER)
    {
        foreach(WireOfE* w ,wBuffer)//
        {//�������� ��� ��������� �� ������� �������
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
    QList<OutsWire> usingOuts;//������ �������� � ������� ��� ������������
    QList<LevelsElement> usingLevels;//������ ��������� � ������� ��� ������������
    //

    QList<Line> temp_Line;//������ ����� ��� ��������� ������������ �� ������� cur(�������� ����� ����� ������ �������)
    QList<WireOfE *> temp_Wire;//������� ����������������

    QMap<int,int> list_rungs;//����� �����
    QMap<Element*,int>next_elements;//������ ��������� �� ������� ����� ������� �� �������
    //�� ����� ���� �� ����� ��� next ���� ������� �������� � ����� ����� �������
    foreach(Element* n_e, curListE)//���������� �������� � �������(������ ���������)
    {//����������
        QList<Line> e_Line=descriptElementLike3Line(n_e );//
        temp_Line.append(e_Line[0]);
        temp_Line.append(e_Line[1]);
        temp_Line.append(e_Line[2]);
        //
        list_rungs.insert(MapRung[n_e],0);
        //
        foreach(WireOfE * w, wBuffer)
        {
            if(w->startP == n_e)//����� ���� �������������
                next_elements.insert(w->endP,0);
        }


        QList<WireOfE *> tempw;
        foreach(WireOfE* wire, wBuffer)
        {//������� ��� ������� �� ���������
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

    foreach(Element *e, next_elements.keys())//���-�� �� ���---------------------------------------------------
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
    int cur_n_w=usingOuts.size()-1;//������ ������� ������ ������ ���������
    //�������� ���� �� ������� ��������� �������
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
        foreach(Line L6, temp_Line)//��� ������������ ��������� ������ ������ ����� ������ ����� ��������
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
            foreach(Line L, wireL)//����� ��������
            {//�� ����������� � �������� ����� ��� ����������
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
            if(cur_n_e==-1)//������� ��������
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
            setAutoVerticalLayout();//���������� ������� ��������� ��-�� ����� �������
            paintFirst();//��������������� �������� �� ����� �������
        }
        //flag =false //������ ������ �������//������ ����� �������(Level ���������)++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }while(!flag);


    //
    /*
    QMessageBox msgBox;
    msgBox.setText( QString("fail") );
    msgBox.exec();*/
    //

    //������ �������� ��� �������� ���������
    foreach(WireOfE * w, temp_Wire)
    {
        static_wire.append(w);

        QList<Line> wire=w->getLines();
        foreach(Line L, wire)//����� ��������
        {
            static_Line.append(L);
        }
    }
    foreach(Line L, temp_Line)//����� ���������
    {
        static_Line.append(L);
    }
    //������ �������� �������� ��������� �� ������� ����
}
QList<Line> PaintWidget::descriptElementLike3Line(Element* e)
{//�������� 3-� �����

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

void PaintWidget::setAutoposityon()//�����
{
    //����� ����� �������� ��������� � �������(� ���� �������� ����� ��� ������� �� ����� � ����� ������� ��������)
    //mapLevel;//Level ���� � 1 ���������� ��� � Rung
    QMap<int,int> kolAtRung;//���������������, ������ ����� ����� � ����� �������� ������� ��� ���� � ������ ������
    int rung=0;
    foreach(Element* eR,MapRung.keys())
    {
        rung = MapRung[eR];
        kolAtRung.insert(rung,kolAtRung[rung]+1 );//����������� ����� ��������� ������� ��� �����������
        mapLevel.insert(eR,kolAtRung[rung]);//������� �������� � ��� �������(� ������), ������� ����� ��� ��������
    }
    for(int i=0;i<eBuffer.size();i++)//��������� �������
    {
        eBuffer[i]->setPos(MapRung[eBuffer[i]]*50, mapLevel[eBuffer[i]]*50);
        //eBuffer.insert();
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{

    setWhatsThis("dsf");
    //QWhatsThis::showText(event->pos(),"����� ���������");
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
    //this->setMouseTracking(true);
    if(evt->type() == QEvent::MouseTrackingChange)
    {
        this->setToolTip( QString("nww %1").arg(time++) );
    }
    //this->setMouseTracking(true);
    return false;
    //return PaintWidget::eventFilter( obj, evt );
}
/*
void PaintWidget::changeEvent(QEvent * e)
{
    this->setToolTip( QString("nwwvb %1").arg(time++) );
    if(e->type() == QEvent::MouseTrackingChange)
    {
        this->setToolTip("n");
        QMouseEvent *event = (QMouseEvent*)e;

        for(int i=0;i<eBuffer.size();i++)
        {
            //this->setToolTip( QString("nww %1").arg(QCursor::pos().x()) );
            if(eBuffer[i]->isOnclic(QCursor::pos().x(), QCursor::pos().y()))
            {
                this->setToolTip("qw\nr/ner\nww");
                break;
            }
        }
        this->setToolTip( QString("nww %1").arg(time++) );
        //this->setToolTip("");

    }
    this->setToolTip( QString("nww %1").arg(time++) );
}*/

void PaintWidget::paintEvent(QPaintEvent *event)
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
        wBuffer[i]->update();
        //wBuffer[i]->updateStart();
        //����
        //���
        wBuffer[i]->draw(&painter, getCollisionPoints(wBuffer[i]) );
        //���������� ����� �������� ������ �����
    }
    setFont(QFont("Arial", 12, 100, 0));

}


QList<QPoint> PaintWidget::getCollisionPoints(WireOfE* wire)
{
    QList<QPoint> tempL;
    foreach(WireOfE* w ,wBuffer)
    {
        if(w == wire) continue;
        Line ml = wire->getLines()[1];
        Line ll = w->getLines()[0];
        if(ml.start.x() > ll.start.x() == ml.end.x() < ll.end.x() &&
           ml.start.y() > ll.start.y() == ml.end.y() < ll.end.y())
        {
            tempL.append(QPoint(ml.start.x(),ll.start.y()) );
            continue;
        }
        Line rl = w->getLines()[2];
        if(ml.start.x() > rl.start.x() == ml.end.x() < rl.end.x() &&
           ml.start.y() > rl.start.y() == ml.end.y() < rl.end.y())
        {
            tempL.append(QPoint(ml.start.x(),rl.start.y()) );
        }
    }
    qDebug()<<"UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU"<<tempL.size();
    return tempL;
}
