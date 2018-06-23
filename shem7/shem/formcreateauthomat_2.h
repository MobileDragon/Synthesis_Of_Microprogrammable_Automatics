#ifndef FORMCREATEAUTHOMAT_2_H
#define FORMCREATEAUTHOMAT_2_H

#include <QDialog>
#include <QtGui>
#include <QRegExp>
#include <QList>
#include "TransitionAuthomat.h"

class FormCreateAuthomat_2 : public QDialog
{
    Q_OBJECT
public:
    explicit FormCreateAuthomat_2(int num_input, int num_state, QWidget *parent = 0);
    bool isActivated();
    QList<TransitionAuthomat> getListTransit();
    QString getResetState();

    QLineEdit* inputsL;
    QLineEdit* statusL;
private:
    int input;
    int state;
    QString resetSignal;
    QLineEdit* resetAtSignal;
    QTableView* tablePV;
    QPushButton* nextB;
    bool isNext;
    QList<TransitionAuthomat> listTransit;//таблица значений
    bool isItemForNextState(QStandardItem* item);

private slots:
    void nextClk();
    void cancelClk();
    void closeEvent(QCloseEvent *__e);
    void resetChanged();

    void tabChanged(QStandardItem* item);



};

#endif // FORMCREATEAUTHOMAT_2_H
