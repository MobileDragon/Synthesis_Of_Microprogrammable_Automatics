#include "buildauthomat.h"

BuildAuthomat::BuildAuthomat(QList<TransitionAuthomat> list_transit, QString reset_state)
{
    listTransit=list_transit;
    resetState=reset_state;
    idWire=0;
//
    fielMapBush();
    fielMapBush2RS();
    fielMapBush3RS();
    fielMapBush4RS();
}

QString BuildAuthomat::makeKey(int num)
{
    QString key = QString("y%1").arg(num);
    return key;
}

void BuildAuthomat::fielMapBush()
{
    //mapBush;
    ShemAtBush bus;
    //mapBush.insert()
    foreach(TransitionAuthomat transit, listTransit)
    {
        idWire++;
        //формировать уравнения для каждой шины
        //первая шина
        QString formul = QString("%1*%2").arg(transit.idInput,transit.prevState);
        qDebug()<<formul;
        Shem* sh=new Shem();
        Comply* func=new Comply(sh, false,makeKey(idWire) );//добавить указание названия последнего элемента
        //makeKey(idWire)-выход для отображения в шине
        busIn.insert(makeKey(idWire),transit.nextState );// какой сигнал, куда должен попасть(триггер)
        busInR.insert(makeKey(idWire),transit.prevState );
        if(transit.prevState!=resetState)
            busInR.insert("R",transit.prevState );

        func->decompositionOfLogicalOperations(formul);
        //
        bus.shems.append(sh);
        //вторую определить отдельно от всех(когда все остальные заполнены)
        //третяя шина получает на входы выходы из второй шины
        //четвертая оттображает выходы, но подводит элементы из второй

        //добавить T и RS тригер
    }
    busIn.insert("R",resetState );
    mapBush.insert(1, bus);
}

void BuildAuthomat::fielMapBush2RS()
{//выходы этой(второй) шины необходимо сохранить для указав к какому выходу они принадлежат
 //также сохранить их указав на какое состояние(id тригера) они подаются, и на какой вход
    QMap<QString, QString> formulSetT;//триггер, формулу что должен получить на SET
    foreach(QString key ,busIn.keys())//key-номер сигнала
    {//key-номер провода
        QString treeger=busIn[key];//триггер, что получает этот провод

        if(formulSetT[treeger].length() == 0)
            formulSetT.insert(treeger, key  );//записать первый провод для входа на триггер
        else
            formulSetT.insert(treeger, QString("%1+%2").arg(formulSetT[treeger],key   ) );
        //formulSetT[treeger]-формула для формирования схемы для установки сигнала на SET
    }

    foreach(QString key ,busIn.keys())//key-номер сигнала
    {//key-номер провода
        QString treeger=busIn[key];//триггер, что получает этот провод
        qDebug()<<formulSetT[treeger];//одна из формул
    }

    ShemAtBush bus;
    foreach(QString treeger ,formulSetT.keys())
    {
        trigerIn inAtTreeger;
        //
        QString formul=formulSetT[treeger];
        bool flag =false;
        for(int i=0; i<formul.length(); i++)
        {
            if(formul[i]=='+')
            {
                flag=true;
                break;
            }
        }

        if(flag)//,больше одного элемента
        {
            qDebug()<<formul;
            qDebug()<<"true";
            idWire++;
            Shem* sh=new Shem();
            Comply* func=new Comply(sh, false,makeKey(idWire) );//makeKey(idWire)- указание названия последнего элемента
            //задание выходного сигнала
            inAtTreeger.S=makeKey(idWire);
            func->decompositionOfLogicalOperations(formul);
            //построение структуры по формуле
            bus.shems.append(sh);
        }
        else
        {//сделать sh с единственным входом, которым и есть formul
            qDebug()<<formul;
            qDebug()<<"false";
            idWire++;
            Shem* sh=new Shem();

            ShemE* eIN = new ShemE(formul);
            ShemE* eOUT = new ShemE(1,makeKey(idWire));
            inAtTreeger.S=makeKey(idWire);//сигнал на триггер что должен установиться
            //получить пердыдущий триггер невозможно
            //

            currentWire *wire = new currentWire(eOUT,0);//провод на следующий элемент

            eIN->wire.insert(0,wire);

            CurrentShem* start = new CurrentShem;
            start->elements.append(eIN);
            CurrentShem* end = new CurrentShem;
            end->elements.append(eOUT);

            sh->start = start;//

            sh->end = end;
            bus.shems.append(sh);
        }
        mapTrigerIn.insert(treeger,inAtTreeger);//сет сигналы

    }//ok

    //previous

    QMap<QString, QString> formulSetTR;//триггер, формулу что должен получить на SET
    foreach(QString key ,busInR.keys())//key-номер сигнала
    {//key-номер провода
        QString treeger=busInR[key];//триггер, что получает этот провод

        if(formulSetTR[treeger].length() == 0)
            formulSetTR.insert(treeger, key  );//записать первый провод для входа на триггер
        else
            formulSetTR.insert(treeger, QString("%1+%2").arg(formulSetTR[treeger],key   ) );
        //formulSetT[treeger]-формула для формирования схемы для установки сигнала на SET
    }

    foreach(QString key ,busInR.keys())//key-номер сигнала
    {//key-номер провода
        QString treeger=busInR[key];//триггер, что получает этот провод
        qDebug()<<formulSetTR[treeger];//одна из формул
    }

    //ShemAtBush bus;
    foreach(QString treeger ,formulSetTR.keys())
    {
        //trigerIn inAtTreeger;
        //
        QString formul=formulSetTR[treeger];
        bool flag =false;
        for(int i=0; i<formul.length(); i++)
        {
            if(formul[i]=='+')
            {
                flag=true;
                break;
            }
        }

        if(flag)//,больше одного элемента
        {
            qDebug()<<formul;
            qDebug()<<"true";
            idWire++;
            Shem* sh=new Shem();
            Comply* func=new Comply(sh, false,makeKey(idWire) );//makeKey(idWire)- указание названия последнего элемента
            //задание выходного сигнала
            mapTrigerIn[treeger].R=makeKey(idWire);
            func->decompositionOfLogicalOperations(formul);
            //построение структуры по формуле
            bus.shems.append(sh);
        }
        else
        {//сделать sh с единственным входом, которым и есть formul
            qDebug()<<formul;
            qDebug()<<"false";
            idWire++;
            Shem* sh=new Shem();

            ShemE* eIN = new ShemE(formul);
            ShemE* eOUT = new ShemE(1,makeKey(idWire));
            mapTrigerIn[treeger].R=makeKey(idWire);//сигнал на триггер что должен установиться
            //получить пердыдущий триггер невозможно
            //

            currentWire *wire = new currentWire(eOUT,0);//провод на следующий элемент

            eIN->wire.insert(0,wire);

            CurrentShem* start = new CurrentShem;
            start->elements.append(eIN);
            CurrentShem* end = new CurrentShem;
            end->elements.append(eOUT);

            sh->start = start;//

            sh->end = end;
            bus.shems.append(sh);
        }
        //mapTrigerIn.insert(treeger,inAtTreeger);//сет сигналы
    }//







    //
    mapBush.insert(2, bus);









}


void BuildAuthomat::fielMapBush3RS()
{//третяя шина
    ShemAtBush bus;
    foreach(QString treeger, mapTrigerIn.keys())
    {
        //QString formul = QString("%1+%2").arg(mapTrigerIn[treeger].R ,key  );



        ShemE* eIN = new ShemE(mapTrigerIn[treeger].S);
        ShemE* eIN2 = new ShemE(mapTrigerIn[treeger].R);

        ShemE* eTriger = new ShemE("RS15S",treeger,false);//триггер
        //QString name,QString id, bool

        idWire++;
        ShemE* eOUT = new ShemE(1,makeKey(idWire));//Q
        idWire++;
        ShemE* eOUT2 = new ShemE(1,makeKey(idWire));//~Q
        //

        currentWire *wire = new currentWire(eTriger,0);//провод на set
        currentWire *wire2 = new currentWire(eTriger,1);//провод на reset

        eIN->wire.insert(0,wire);
        eIN2->wire.insert(0,wire2);

        currentWire *wireOut = new currentWire(eOUT,0);//провод на set
        currentWire *wire2Out = new currentWire(eOUT2,0);//провод на reset

        CurrentShem* start = new CurrentShem;
        start->elements.append(eIN);
        start->elements.append(eIN2);
        CurrentShem* end = new CurrentShem;
        end->elements.append(eOUT);
        end->elements.append(eOUT2);

        Shem* sh=new Shem();
        sh->start = start;//

        sh->end = end;

        bus.shems.append(sh);
    }
    mapBush.insert(3, bus);
}

void BuildAuthomat::fielMapBush4RS()
{//четвертая шина
    ShemAtBush bus;

    QList<QString> formuls;
    QMap<QString, QString> formulsOuts;
    foreach(TransitionAuthomat value, listTransit)
    {// заполнение mapOut
        //value.idOut;

        //value.nextState;
        //value.prevState;
        //QString("%1&%2").arg(value.nextState ,value.prevState );
        //value.
        //mapOut[value.idOut].in.append();


        QString formul = QString("%1*%2").arg(value.nextState ,value.prevState  );
        QString oldFormul = formulsOuts[value.idOut];
        if(oldFormul.length()==0)
        {
            formulsOuts[value.idOut]=formul;
        }
        else
            formulsOuts[value.idOut]=QString("%1+%2").arg(oldFormul ,formul  );


    }


    foreach(QString outS, formulsOuts.keys())
    {
        Shem* sh=new Shem();
        Comply* func=new Comply(sh, false,outS );
        //задание выходного сигнала
        func->decompositionOfLogicalOperations(formulsOuts[outS]);
        //построение структуры по формуле
        bus.shems.append(sh);
    }
    mapBush.insert(4, bus);
}

QMap<int, ShemAtBush> BuildAuthomat::getBush()
{
    return mapBush;
}

QList<QString> BuildAuthomat::getListOutA()
{
    QList<QString> outs;
    foreach(TransitionAuthomat transit, listTransit)
    {
        bool flag = false;
        foreach(QString out, outs)
        {
            if(out == transit.idOut)
            {
                flag=true;
                break;
            }
        }
        if(!flag)
            outs.append(transit.idOut);
    }
}

QList<QString> BuildAuthomat::getListInputA()
{
    QList<QString> input;
    foreach(TransitionAuthomat transit, listTransit)
    {
        bool flag = false;
        foreach(QString in, input)
        {
            if(in == transit.idInput)
            {
                flag=true;
                break;
            }
        }
        if(!flag)
            input.append(transit.idInput);
    }
}
