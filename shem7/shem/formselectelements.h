#ifndef FORMSELECTELEMENTS_H
#define FORMSELECTELEMENTS_H

#include <QDialog>
#include "bde.h"
#include <QtGui>
#include "element.h"
#include "elementalt.h"
#include "paintwidget.h"
#include <QDialog>

struct column
{
    QString name;
    double time;
    QString operation;
};

class FormSelectElements : public QDialog
{
    Q_OBJECT
public:
    explicit FormSelectElements(QWidget *parent = 0);
    QList<QString> getListSelected();
    bool isActivated();
private:
    QList<QString> listSelect;
    QString selectElements;
    bool isNext;
    void baseClik(QString nameOfBase);

    QLineEdit* editL;
    QTableView* tableBase;
    QTableView* tableSelected;

    QString convertType(int kolInput, QString operation);
    void field_tableOfBase(QStandardItemModel* table, bool flag);
    void updateListBase();
    void updateListSelected();
    QVector<column> getListElements(QList<QString> dontSelect, bool flag);//flag=true значит только из списка
    QString name;
    PaintWidget* w;
signals:
    
public slots:
    void editFields();
    void closeEvent(QCloseEvent *__e);
    void slotClik(const QModelIndex &index);
    //void slotClikSelected(const QModelIndex &index);

    void slotAdd();
    void slotDel();

    void nextClk();
    void cancelClk();
    
};

#endif // FORMSELECTELEMENTS_H
