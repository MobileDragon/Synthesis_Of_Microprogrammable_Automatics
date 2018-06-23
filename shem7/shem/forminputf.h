#ifndef FORMINPUTF_H
#define FORMINPUTF_H

#include <QDialog>

#include <QtGui>

#include <QDebug>

class FormInputF : public QDialog
{
    Q_OBJECT
public:
    FormInputF(QWidget* pwgt = 0);
    bool isActivated();
    bool getEkonomChek() const;
    QString getFomul() const;


private:
        QPushButton* nextB;
        bool isNext;
        QLineEdit* m_ptxtFormul;
        QCheckBox* chek;


private slots:
    void nextClk();
    void cancelClk();
};

#endif // FORMINPUTF_H
