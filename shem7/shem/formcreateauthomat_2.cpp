#include "formcreateauthomat_2.h"

FormCreateAuthomat_2::FormCreateAuthomat_2(int num_input, int num_state, QWidget *parent) :
    QDialog(parent)
{
    input = num_input;
    state = num_state;
    this->setWindowTitle("Table Switching & Outs");
    this->setMinimumWidth(500);
    this->setMinimumHeight(320);
    nextB = new QPushButton("Next>");
    nextB->setMaximumWidth(80 );
    QPushButton* cancelB     = new QPushButton("Cancel");
    cancelB->setMaximumWidth(80 );

    QVBoxLayout* vBox = new QVBoxLayout;
    vBox->setSpacing(10);

    QHBoxLayout* hBox = new QHBoxLayout;
    hBox->addStretch();//заполнение оставшегося пространства
    hBox->addWidget(cancelB,1);//выход
    hBox->addWidget(nextB,2);//продолжить создание ватомата(выбор набора элементов)
    connect(nextB, SIGNAL(clicked()), SLOT(nextClk()));
    connect(cancelB, SIGNAL(clicked()), SLOT(cancelClk()));

//генерируй по input и state таблицу заголовки выдели остальной позволь заполнять QTableView

    tablePV =new QTableView;
    tablePV->setFont(QFont("Arial",10,5000));
    tablePV->verticalHeader()->hide();
    tablePV->horizontalHeader()->hide();
//генерация модели таблицы(шапка)
    QStandardItemModel  *model = new QStandardItemModel;
    QStandardItem *item;
    //горизонтальная
    item = new QStandardItem("Current" );
    item->setEditable(false);
    item->setBackground(QBrush(QColor(250,240,180)) );
    item->setFont(QFont("Arial",11,8000));
    model->setItem(0, 0, item);//первая ячкйка
    int numState=1;
    for(int i=1; i<state*2; i+=2)//список всех состояний
    {
        item = new QStandardItem(QString("State A%1").arg(numState));
        item->setTextAlignment(Qt::AlignCenter);
        item->setEditable(false);
        item->setBackground(QBrush(QColor(250,240,180)) );
        item->setFont(QFont("Arial",11,8000));
        model->setItem(0, i, item);
        numState++;
    }
    item = new QStandardItem("Next");
    item->setEditable(false);
    item->setBackground(QBrush(QColor(250,230,170)) );
    item->setFont(QFont("Arial",10,6500));
    model->setItem(1, 0, item);//первая ячкйка
    for(int i=1; i <= state*2; i++)//шапка переключений и выходов
    {
        QString typeHeader;
        if(i%2==0)
            typeHeader="Output ";
        else
            typeHeader="Switch at";
        item = new QStandardItem(typeHeader);
        item->setEditable(false);
        item->setBackground(QBrush(QColor(250,230,170)) );
        item->setFont(QFont("Arial",10,6500));
        model->setItem(1, i, item);
    }
    numState=1;
    for(int i=2; i<input+2; i++)//список всех входов
    {
        item = new QStandardItem(QString("Input Z%1 ").arg(numState) );
        item->setEditable(false);
        item->setBackground(QBrush(QColor(200,220,250)) );
        item->setFont(QFont("Arial",11,8000));
        model->setItem(i, 0, item);
        numState++;
    }
    connect(model, SIGNAL(itemChanged(QStandardItem*)) ,this,SLOT(tabChanged(QStandardItem*) ));//изменение
    tablePV->setModel(model);
    for(int i=1; i<=state*2; i+=2)//объединение шапки состояний
    {
        tablePV->setSpan(0,i,1,2);
    }
    for(int i=0; i < state*2+1; i++)//ширина столбцов
    {
        tablePV->horizontalHeader()->setResizeMode(i,QHeaderView::ResizeToContents);
    }



    QHBoxLayout* hBox0 = new QHBoxLayout;
    QLabel* labelR = new QLabel("Input State, whot must be instaled, when is Reset");
    labelR->setFont(QFont("Arial",9));
    hBox0->addWidget(labelR,0);//
    resetAtSignal = new QLineEdit;
    resetAtSignal->setText("A1");
    resetAtSignal->setMaximumWidth(40);
    resetAtSignal->setFont(QFont("Arial",9));
    hBox0->addWidget(resetAtSignal,1);
    hBox0->addStretch();//заполнение оставшегося пространства
    connect(resetAtSignal, SIGNAL(editingFinished()) ,this,SLOT(resetChanged() ));//изменение
    //connect(resetAtSignal, SIGNAL(selectionChanged()) ,this,SLOT(resetSelect() ));//выбрано
    //connect(resetAtSignal, SIGNAL(editingFinished()) ,this,SLOT(resetUnSelect() ));//не выбрано

    vBox->addLayout(hBox0,1);
    vBox->addWidget(tablePV,1);
    vBox->addLayout(hBox,2);//добавление кнопок

    this->setLayout(vBox);
}



bool FormCreateAuthomat_2::isItemForNextState(QStandardItem* item)
{
    if(item->column()%2 != 0)
        return true;
    else return false;
}

void FormCreateAuthomat_2::nextClk()//slot
{
    isNext=true;
    resetSignal=resetAtSignal->text();
    //сформировать лист структуры

    for(int row=2; row < input+2; row++)
    {
        int prevState=1;
        for(int column=1; column < state*2+1; column+=2)
        {
            QModelIndex index = tablePV->model()->index(row,column+1);
            TransitionAuthomat transit;
            transit.idInput = QString("Z%1").arg(row-1);
            transit.idOut = tablePV->model()->data(index).toString();
            index = tablePV->model()->index(row,column);
            transit.nextState = tablePV->model()->data(index).toString();

            transit.prevState = QString("A%1").arg(prevState);
            listTransit.append(transit);
            prevState++;
        }
    }





    //emit numInput(inputsL->text().toInt(), statusL->text().toInt());
    accept();
}

bool FormCreateAuthomat_2::isActivated()
{
    return isNext;
}

QList<TransitionAuthomat> FormCreateAuthomat_2::getListTransit()
{
    if(isNext)
        return listTransit;
}
QString FormCreateAuthomat_2::getResetState()
{
    if(isNext)
        return resetSignal;
}

void FormCreateAuthomat_2::cancelClk()//slot
{
    accept();
}

void FormCreateAuthomat_2::tabChanged(QStandardItem* item)
{
    //если это состояние автомата
    int numState=0;
    if(isItemForNextState(item) )
    {
        if(item->text().length()==0)
            return;
        QRegExp rx("(\\d+)");
        int pos = rx.indexIn( item->text() );
        if (pos > -1)
            numState = rx.cap(1).toInt();
        if(numState < 1)
            numState=1;
        if(numState > state)
            numState = state;
        item->setText( QString("A%1").arg(numState) );

    }
}

void FormCreateAuthomat_2::resetChanged()
{
    int numState=0;
    QRegExp rx("(\\d+)");
    int pos = rx.indexIn( resetAtSignal->text() );
    if (pos > -1)
        numState = rx.cap(1).toInt();
    if(numState < 1)
        numState=1;
    if(numState > state)
        numState = state;
    resetAtSignal->setText( QString("A%1").arg(numState) );
}

void FormCreateAuthomat_2::closeEvent(QCloseEvent *__e){
    //name->setText("");
    __e->accept();
}
