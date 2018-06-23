#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elementale.h"
#include "elementalt.h"
#include "comply.h"

#include <QDebug>

#include "elementalwilka.h"
#include "forminputf.h"
#include "formcreateauthomat_1.h"
#include "formcreateauthomat_2.h"
#include "paintauthomat.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenus();//������ ����


    //QGridLayout *l = new QGridLayout();
    //l->addWidget(new QPushButton("Ok"));
    //setLayout(l);

    w= new PaintWidget;
    //QWidget *w=new QWidget;
    w->setMinimumHeight(100);
    w->setMinimumWidth(100);

    //QPushButton *n = new QPushButton("Ok");
    //QPushButton *nn = new QPushButton("Ok6");

    //ui->tabWidget->


    ui->horizontalLayout->setMargin(20);

    ui->horizontalLayout->addWidget(w);

    authomatIndex=1;
    shemIndex=1;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{

    //PaintWidget *area=new PaintWidget();
    this->setWindowTitle("Shem builder");

    QMenu * mnFile = new QMenu("����"); // ������ ���� ����
    // ----------- ����� ��������� ����� ���� � ���������� ��� � �����----
    QAction *msgAction = new QAction("������ �������",mnFile);
    connect(msgAction, SIGNAL(triggered()), this, SLOT(createFromFunction()));
    mnFile->addAction(msgAction);


    QAction *createAction = new QAction("������ ������� ��������",mnFile);
    connect(createAction, SIGNAL(triggered()), this, SLOT(createFromTT()));
    mnFile->addAction(createAction);



    QMenu * mnEdit = new QMenu("������������"); // ������ ���� Edit
    QAction *aboutShem = new QAction("³������ ��� �����",mnFile);
    aboutShem->setCheckable(true);
    QAction *visualeName = new QAction("����� ��������",mnFile);
    visualeName->setCheckable(true);
    QAction *visualFunctson = new QAction("������� ����������",mnFile);
    visualFunctson->setCheckable(true);
    mnEdit->addAction(aboutShem);
    mnEdit->addAction(visualeName);
    mnEdit->addAction(visualFunctson);

    QMenu * mnHelp = new QMenu("������"); // ���� ������
    QAction *aboutP = new QAction("��� �������",mnFile);
    QAction *aboutU = new QAction("������ �����������",mnFile);
    mnHelp->addAction(aboutP);
    mnHelp->addAction(aboutU);





    ui->menuBar->addMenu(mnFile); // ��������� ������ ���� � menuBar, �.�. ��, ������� ����� ������������ � ��. ����
    ui->menuBar->addMenu(mnEdit);
    ui->menuBar->addMenu(mnHelp);

}

void MainWindow::createFromFunction()
{
    FormInputF* formSetFormul = new FormInputF();
    formSetFormul->exec(); //������ ����� ����� �������
    if(!formSetFormul->isActivated())
        return;
    QString funk = formSetFormul->getFomul();
    bool ekonomBuild = formSetFormul->getEkonomChek();

    //
    BDE::getInstance()->clearSelectedE();
    FormSelectElements* form = new FormSelectElements();
    form->exec(); //������ ����� ������ ���������
    if(!form->isActivated())
        return;
    BDE::getInstance()->setSelectedE(form->getListSelected() );

    //
    Shem* sh=new Shem();
    Comply* func=new Comply(sh, ekonomBuild);
    //�������� ��� ������� � ���� ���������� �������
    QString temp=func->decompositionOfLogicalOperations(funk);

    //
    PaintWidget* s = new PaintWidget;//����� �������
    s->setShem(sh);
    ui->tabWidget->addTab(s,QString("Shem %1").arg(shemIndex++) );
    ui->tabWidget->setCurrentWidget(s);

}

void MainWindow::createFromTT()
{
    QString puth = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.csv *.txt");
    QVector<QString> dnf = Karno::buildDNF(puth);

    //Comply::convert(dnf,w);

    //QVector<QString> dnf = Karno::buildDNF(puth);

    QString result="";
    foreach(QString str,dnf)
        result+=Karno::convert(str)+"+";
    QString h100=result.left(result.length()-1);

    //
    BDE::getInstance()->clearSelectedE();
    FormSelectElements* form = new FormSelectElements();
    form->exec(); //������ ����� ������ ���������
    if(!form->isActivated())
        return;
    BDE::getInstance()->setSelectedE(form->getListSelected() );
    //
    Shem* sh=new Shem();
    Comply* func=new Comply(sh, false);
    //�������� ��� ������� � ���� ���������� �������
    QString temp=func->decompositionOfLogicalOperations(h100);

    //
    PaintWidget* s = new PaintWidget;//����� �������
    s->setShem(sh);
    ui->tabWidget->addTab(s,QString("Shem %1").arg(shemIndex++) );
    ui->tabWidget->setCurrentWidget(s);

}

void MainWindow::createAuthomat()//�������� ������ ��������
{


    //ElementalE *rtt=new ElementalE("or304");//RS15S
    //ElementalT *rtt=new ElementalT("RS15S");//��������� ������ ���������� ���������
    //w->addElement(rtt);


    QList<TransitionAuthomat> listTransit;
    QString resetState;


    FormCreateAuthomat_1* form = new FormCreateAuthomat_1();
    //connect(form, SIGNAL(numInput(int,int)), SLOT(createAuthomatNext(int,int)) );
    form->exec(); //������ ����� ������ ���������

    if(form->isActivated())
    {
        FormCreateAuthomat_2* form2 = new FormCreateAuthomat_2(form->getNumInput(), form->getNumState());
        form2->exec(); //������ ����� ������ ���������
        if(form2->isActivated())
        {
            listTransit=form2->getListTransit();
            resetState=form2->getResetState();
            BuildAuthomat* authomat = new BuildAuthomat(listTransit, resetState);//
            QMap<int, ShemAtBush> mapBush = authomat->getBush();//������ ���� �� ����� ��� �������� int(key)
///////////////////
            paintAuthomat* a = new paintAuthomat(listTransit);
            ui->tabWidget->addTab(a,QString("Authomat %1").arg(authomatIndex++) );
            ui->tabWidget->setCurrentWidget(a);


        }
    }

}


void MainWindow::showMessage()
{
/*
    FormSelectElements* form = new FormSelectElements();
    form->exec(); //������ ����� ������ ���������

    FormInputF* form1 = new FormInputF();
    form1->exec(); //������ ����� ������ ���������
    QString jot0 = form1->firstName();
    bool jot = form1->lastName();
*/
    QString jot0 ="x1+x2";
    bool jot = false;

    //QMessageBox msgBox;
    //msgBox.setText("First Name:  "+form->firstName());
    //msgBox.exec();

    //���������� ���� ������
    QSqlQuery *query = BDE::getInstance()->make("SELECT elements.name, idwires, idoperations, time FROM elements");

    //������� �������� �� �������
    while (query->next())
    {
        //ui->textEdit->insertPlainText("23frd4");
        QString name = query->value(0).toString();
        QString idwires = query->value(1).toString();
        QString idoperations = query->value(2).toString();
        QString time = query->value(3).toString();
        //ui->textEdit->insertPlainText(name+" "+idwires+" "+idoperations+" "+time+"\n");
    }


    ElementalE *rtt=new ElementalE("or304");//��������� ������ ���������� ���������
    //w->addElement(rtt);
    //
    ElementalWilka *rt=new ElementalWilka("X1", "x1");
    //w->addElement(rt);
/*
    QString puth = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.csv *.txt");
    QVector<QString> dnf = Karno::buildDNF(puth);

    //Comply::convert(dnf,w);

    //QVector<QString> dnf = Karno::buildDNF(puth);

    QString result="";
    foreach(QString str,dnf)
        result+=Karno::convert(str)+"+";
    QString h100=result.left(result.length()-1);
*/

    Shem* sh=new Shem();
    Comply* func=new Comply(sh, jot);

    //QString temp=func->decompositionOfLogicalOperations(h100);
    //�������� ��� ������� � ���� ���������� �������
    QString temp=func->decompositionOfLogicalOperations(jot0);
    //���������� PaintWidget w
    w->setShem(sh);

    //X1*(X1+X2)+X2*X2
    //~X9+((~(X1+~X2+~X3)+X5)*~(X4))
    //~(X1+~X2+~X3)*~(X4+~X5)
    //"X1*~(X2+X3+~X4+X5)"
    //"X1*X2+X3"
    //"X1*(X2+X3+X4+X5)"
    //"X1+X2+X3+X4+X5+(X6)"
    //"X1*X2*X3*X4+X5+X6+X7*X8*X9"
    //"X1*(X2+X3+X4+X5)*(X6+X7+X8)"


    ui->textEdit->insertPlainText(temp);

}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
