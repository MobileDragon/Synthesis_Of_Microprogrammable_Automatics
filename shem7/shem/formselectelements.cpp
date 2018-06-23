#include "formselectelements.h"





FormSelectElements::FormSelectElements(QWidget *parent) :
    QDialog(parent)
{
    name="";
    isNext=false;
    this->setWindowTitle("Вибір елементів для побудування");
    this->setMinimumWidth(600);
    QPushButton* pcmdOk     = new QPushButton("&Build");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");
    connect(pcmdOk, SIGNAL(clicked()), SLOT(nextClk()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(cancelClk()));


    QPushButton* pcmdAdd    = new QPushButton("Додати>");
    QPushButton* pcmdDel = new QPushButton("<Видалити");
    connect(pcmdAdd, SIGNAL(released()), SLOT(slotAdd()) );
    connect(pcmdDel, SIGNAL(released()), SLOT(slotDel()) );

    //connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    //connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

    editL = new QLineEdit;

    tableSelected = new QTableView();
    tableSelected->verticalHeader()->hide();
    tableSelected->setSortingEnabled(true);
    tableSelected->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableSelected->verticalHeader()->hide();
    tableSelected->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStandardItemModel  *model = new QStandardItemModel;
    QStringList name_table;
    name_table << "Ім'я" << "Тривалість" << "Операція";
    model->setHorizontalHeaderLabels(name_table);
    tableSelected->setModel(model);
    tableSelected->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
    tableSelected->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    tableSelected->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);
    connect(tableSelected, SIGNAL(clicked(QModelIndex)), SLOT(slotClik(const QModelIndex)) );


    tableBase = new QTableView();
    //
    tableBase->setAlternatingRowColors(true);//tableView
    tableBase->setSortingEnabled(true);
    tableBase->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableBase->verticalHeader()->hide();
    tableBase->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //
    updateListBase();

    tableBase->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
    tableBase->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    tableBase->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);

    connect(tableBase, SIGNAL(clicked(QModelIndex)), SLOT(slotClik(const QModelIndex)) );

    //Layout setup
    QHBoxLayout* hBoxCanceOk = new QHBoxLayout;
    hBoxCanceOk->addStretch();
    hBoxCanceOk->addWidget(pcmdCancel);
    hBoxCanceOk->addWidget(pcmdOk);


    connect(editL, SIGNAL(textChanged(QString)), SLOT(editFields()) );
    QVBoxLayout* vBoxBase = new QVBoxLayout;
    QLabel* base    = new QLabel("Елементи з бази");
    vBoxBase->addWidget(base);
    vBoxBase->addWidget(editL);
    vBoxBase->addWidget(tableBase);


    QVBoxLayout* vBoxOutLook = new QVBoxLayout;
    QLabel* OutLook    = new QLabel("Зовншній вигляд");
    vBoxOutLook->addWidget(OutLook);
    w= new PaintWidget;
    w->setMinimumSize(70,110);

    vBoxOutLook->addWidget(w);
    vBoxOutLook->addWidget(pcmdAdd);
    vBoxOutLook->addWidget(pcmdDel);


    vBoxOutLook->addStretch();
    //vBoxOutLook->addWidget(tableBase);
    QVBoxLayout* vBoxSelected = new QVBoxLayout;
    QLabel* selected    = new QLabel("Додані до побудування");
    vBoxSelected->addWidget(selected);
    vBoxSelected->addWidget(tableSelected);



    QHBoxLayout* hChieldMain = new QHBoxLayout;
    hChieldMain->addLayout(vBoxBase);
    hChieldMain->addLayout(vBoxOutLook);
    hChieldMain->addLayout(vBoxSelected);

    QVBoxLayout* vBoxMain = new QVBoxLayout;
    vBoxMain->addLayout(hChieldMain);
    //vBoxMain->addLayout(vBoxOutLook);
    vBoxMain->addLayout(hBoxCanceOk);





    setLayout(vBoxMain);
}

QString FormSelectElements::convertType(int kolInput, QString operation)
{
    QString str = "";
    if(operation == "RS" || operation == "T")
        return operation;

    int i=0;
    if(operation[i] == '!')
    {
        i++;
        if(kolInput > 1)
            str += QString("%1 ! ").arg(kolInput);
        else
        {
            str = "not";
            return str;
        }
    }
    else
        str += QString("%1 ").arg(kolInput);
    if(operation[i] == '1')
        str += "or";
    else
        if(operation[i] == '&')
            str += "and";

    return str;
}


void FormSelectElements::updateListSelected()
{
    QStandardItemModel  *model = new QStandardItemModel;
    QStringList name_table;
    name_table << "Ім'я" << "Тривалість" << "Операція";
    model->setHorizontalHeaderLabels(name_table);
    field_tableOfBase(model,true);//заполнене таблицы

    tableSelected->setModel(model);
}


void FormSelectElements::updateListBase()//////////////////
{
    QStandardItemModel  *model = new QStandardItemModel;
    QStringList name_table;
    name_table << "Ім'я" << "Тривалість" << "Операція";
    model->setHorizontalHeaderLabels(name_table);
    field_tableOfBase(model,false);//заполнене таблицы

    tableBase->setModel(model);

}

void FormSelectElements::field_tableOfBase(QStandardItemModel *table, bool flag)
{
    QVector<column> list = getListElements(listSelect, flag);
    //table->setRowCount(list.size() );

    for(int row = 0; row < list.size(); row++)
    {
        QStandardItem *item = new QStandardItem;

        int col = 0;
        item->setText(QString("%1").arg(list[row].name ));
        table->setItem(row, col++, item); // вставка ячейки
        item = new QStandardItem();
        item->setText(QString("%1").arg(list[row].time ));
        table->setItem(row, col++, item); // вставка ячейки
        item = new QStandardItem();
        item->setText(QString("%1").arg(list[row].operation ));
        //item->setFlags(Qt::ItemIsSelectable);
        table->setItem(row, col, item); // вставка ячейки
    }

}

QVector<column> FormSelectElements::getListElements(QList<QString> dontSelect, bool flag)
{
    QVector<column> list(0);

    QString sqlQuer=QString("select elements.name, elements.time, wires.numInput, operations.operation from elements, wires, operations where (elements.idwires=wires.id and elements.idoperations=operations.id) and (elements.name LIKE '%1%') ORDER BY elements.name").arg(name);

    QSqlQuery *query = BDE::getInstance()->make(sqlQuer);

    while (query->next())
    {
        column str;
        str.name = query->value(0).toString();
        str.time = query->value(1).toDouble();
        str.operation = convertType(query->value(2).toInt(), query->value(3).toString() );
        int t=query->value(2).toInt();
        qDebug() << QString("[!] %1").arg(t);
        if( (!flag) == (dontSelect.indexOf(str.name)<0) )
        {
            list.append(str);
        }
    }
    return list;
}

bool FormSelectElements::isActivated()
{
    return isNext;
}

QList<QString> FormSelectElements::getListSelected()
{
    QList<QString> selectedE;
    if(!isNext)
        return selectedE;
    return listSelect;
}

void FormSelectElements::baseClik(QString nameOfBase)
{
    if(nameOfBase.length()<0)
        return;
    selectElements=nameOfBase;
    QSqlQuery *query = BDE::getInstance()->make(QString("select elements.name, operations.operation from elements, wires, operations where (elements.idwires=wires.id and elements.idoperations=operations.id) and name LIKE '%1'").arg(nameOfBase));
    if(query->next())
    {
        QString type = query->value(1).toString();

        qDebug()<<type;
        Element *rtt;
        if(type=="RS"||type=="T")
            rtt=new ElementalT(nameOfBase);
        else
            rtt=new ElementalE(nameOfBase);
        w->cleanElements();
        w->addElement(rtt);
        w->update();
    }
    else
    {
        w->cleanElements();
        w->update();
    }
}


//
//
void FormSelectElements::slotClik(const QModelIndex &index)
{
    int row = index.row();

    QTableView* thisTable = ((QTableView*)sender());//родитель
    QString element_name = (thisTable->model()->data(thisTable->model()->index(row,0))).toString();

    baseClik(element_name);

}

void FormSelectElements::slotAdd()
{
    if(selectElements.length()==0)
        return;
    if(listSelect.indexOf(selectElements)>=0 )
        return;
    listSelect.append(selectElements);
    updateListBase();
    updateListSelected();

    QModelIndex index = tableBase->model()->index(0,0);
    //slotClik(index);
    baseClik(tableBase->model()->data(index).toString() );

}

void FormSelectElements::slotDel()
{
    if(selectElements.length()==0)
        return;
    int num = listSelect.indexOf(selectElements);
    if(num>=0)
    {
        listSelect.removeAt(num);
        updateListBase();
        updateListSelected();

        QModelIndex index = tableSelected->model()->index(0,0);
        //slotClik(index);
        baseClik(tableSelected->model()->data(index).toString() );
    }
}

void FormSelectElements::editFields()
{
    name = editL->text();
    qDebug()<<"rrrrrrrrrrrrRRRRRRRR";
    updateListBase();
}

void FormSelectElements::nextClk()
{
    isNext=true;
    accept();
}

void FormSelectElements::cancelClk()
{
    accept();
}

void FormSelectElements::closeEvent(QCloseEvent *__e){
    //name->setText("");
    __e->accept();
}
