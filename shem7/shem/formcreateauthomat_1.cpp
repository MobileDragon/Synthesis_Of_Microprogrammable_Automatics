#include "formcreateauthomat_1.h"
#include <QString>

FormCreateAuthomat_1::FormCreateAuthomat_1(QWidget *parent) :
    QDialog(parent)
{
    isNext = false;
    inputsL = new QLineEdit;

    statusL  = new QLineEdit;

    QLabel* inputs    = new QLabel("Set num inputs:");
    QLabel* status     = new QLabel("Set num state:");
    this->setWindowTitle("Create New Authomat");
    status->setBuddy(inputsL);
    status->setBuddy(statusL);

    nextB = new QPushButton("Next>");
    QPushButton* cancelB     = new QPushButton("Cancel");
    nextB->setMinimumWidth(60);
    nextB->setMaximumWidth(60);
    inputsL->setMaximumWidth(nextB->width());
    statusL->setMaximumWidth(nextB->width());

    connect(nextB, SIGNAL(clicked()), SLOT(nextClk()));
    connect(cancelB, SIGNAL(clicked()), SLOT(cancelClk()));
    connect(inputsL, SIGNAL(textChanged(QString)), SLOT(editFields()));
    connect(statusL, SIGNAL(textChanged(QString)), SLOT(editFields()));


    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->setVerticalSpacing(10);
    ptopLayout->addWidget(inputs, 0, 0);
    ptopLayout->addWidget(status, 1, 0);
    ptopLayout->addWidget(inputsL, 0, 1);
    ptopLayout->addWidget(statusL, 1, 1);
    //ptopLayout->addWidget(m_ptxtLastName, 1, 1);

    ptopLayout->addWidget(cancelB, 2, 0);
    ptopLayout->addWidget(nextB, 2,1);

    //loginB->setCheckable(true);
    this->setLayout(ptopLayout);
    nextB->setEnabled(false);

    this->adjustSize();


}

void FormCreateAuthomat_1::nextClk()//slot
{
    isNext=true;
    //emit numInput(inputsL->text().toInt(), statusL->text().toInt());
    accept();
}

int FormCreateAuthomat_1::getNumInput()
{
    return inputsL->text().toInt();
}
int FormCreateAuthomat_1::getNumState()
{
    return statusL->text().toInt();
}
bool FormCreateAuthomat_1::isActivated()
{
    return isNext;
}

void FormCreateAuthomat_1::cancelClk()//slot
{
    accept();
}

void FormCreateAuthomat_1::editFields()//slot
{
    QRegExp reg("[0-9]+");
    if( reg.exactMatch(inputsL->text()) && reg.exactMatch(statusL->text()) )
    {
        if( inputsL->text().toInt()>0  &&  statusL->text().toInt()>0 )
            nextB->setEnabled(true);
        else
            nextB->setEnabled(false);
    }
    else
        nextB->setEnabled(false);
}

void FormCreateAuthomat_1::closeEvent(QCloseEvent *__e){
    //name->setText("");
    __e->accept();
}
