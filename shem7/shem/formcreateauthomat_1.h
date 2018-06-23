#ifndef FORMCREATEAUTHOMAT_1_H
#define FORMCREATEAUTHOMAT_1_H

#include <QDialog>

#include <QtGui>

#include <QRegExp>

class FormCreateAuthomat_1 : public QDialog
{
    Q_OBJECT
public:
    explicit FormCreateAuthomat_1(QWidget *parent = 0);
    bool isActivated();
    int getNumInput();
    int getNumState();

    QLineEdit* inputsL;
    QLineEdit* statusL;
private:
    QPushButton* nextB;
    bool isNext;

private slots:
    void nextClk();
    void cancelClk();
    void closeEvent(QCloseEvent *__e);

    void editFields();

signals:
    void numInput(int input, int state);
    
};

#endif // FORMCREATEAUTHOMAT_1_H
