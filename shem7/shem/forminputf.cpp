#include "forminputf.h"

FormInputF::FormInputF(QWidget *pwgt) :
    QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    m_ptxtFormul = new QLineEdit;
    chek = new QCheckBox;

    isNext=false;
    this->setWindowTitle("Нова схема");
    QLabel* plblFormul    = new QLabel("Введіть функцію");
    QLabel* plblEkonomChek     = new QLabel("Економічне побудування");

    plblFormul->setBuddy(m_ptxtFormul);

    nextB = new QPushButton("Далі");
    QPushButton* pcmdCancel = new QPushButton("Вихід");

    connect(nextB, SIGNAL(clicked()), SLOT(nextClk()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(cancelClk()));

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(plblFormul, 0, 0);
    ptopLayout->addWidget(plblEkonomChek, 1, 0);
    ptopLayout->addWidget(m_ptxtFormul, 0, 1);
    ptopLayout->addWidget(chek, 1, 1);
    ptopLayout->addWidget(pcmdCancel, 2, 0);
    ptopLayout->addWidget(nextB, 2,1);
    setLayout(ptopLayout);
}

bool FormInputF::isActivated()
{
    return isNext;
}

QString FormInputF::getFomul() const
{
    if(!isNext)
        return(QString("") );
    return m_ptxtFormul->text();
}

bool FormInputF::getEkonomChek() const
{
    if(chek->isChecked())
        return true;
    else
        return false;
}

void FormInputF::nextClk()
{
    isNext=true;
    accept();
}

void FormInputF::cancelClk()
{
    accept();
}
