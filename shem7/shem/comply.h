#ifndef COMPLY_H
#define COMPLY_H
#include <QString>
#include <Qvector>
#include <QStringList>
#include "element.h"
#include "bde.h"
#include <QMap>
#include <QStack>
#include <QtCore/QList>
#include "shem.h"

//#include <cctype>

#include "paintwidget.h"
#include "elementale.h"
//���� � ����� �������� ���� � ���� �����
struct formulSearch //��������� ��������� ��� �������� ���������� ����������� ���������
{//(��� decompose)
    int numInputs; //����� ������ � ���������� �����
    int numInclude; //���������� ��������(���+1)
    int numElements; //����� ��������� � �����
    QMap<QString,QString> formul; //��������� �������
    QMap<QString,int> balanceKey;//������ ��� ����������������� ������ � �������//!������ ��� optimimal==false
    QString main_key;//��� �������� �����, ������������� ���������//!������ ��� optimimal==true
};
/*
struct rungKey
{
    int rang;
    QString key;
};
*/
class Comply
{
public:
    QString broken;

    Comply();//�������� ������ ���������� ��������� ��� �������� �������
    Comply(Shem* shema, bool j, QString out="");
    QString decompositionOfLogicalOperations(QString dnf);


private:
    QString outy;
    bool expr();
    bool variable();
    bool factor();
    bool brackets();
    void decompos();//����� ��������� ��������� ��� ����������� � ���� ������� //�������� convert()
    QString makeKey(int i);
    QChar whotAction(QString key);
    int whotCountOperations(QString key);//���������� ��������
    void inicializeInputs(QString dnf);//�������������� ������ ����������(variables )

    QVector<QString> composInput(QString key);//���������� ������ id ���������� �� ���� ��������
    //���������� ��������
    //
    void decomposNegative();//����� ��������� � ��������� ��������
    void clearInaction();//�������� ����������� �� actions
    int getMaxInput(int max,QSqlQuery* query);
    //
    void searchElements(int j, QSqlQuery *query, formulSearch* temp,
                        QVector<QString> balanceSymplList, int cur_num);//������� tempFormul
    //balance ��� ������ ���������� ������� �� �������������, ���������� ��� elements
    bool isBest(formulSearch* tempFormul);//���������� � bestFormul tempFormul ���� ����� ��������
    //��������� ��������? - ��

    void fillShema();//�������� shema
    int getKolPovtor(QString search_key);//����� ������ � ������ ���������
    bool isKnownInputs(QString keyAction, QVector<QString> knownInputs);//�������� ���������
    QVector<QString> delBrekets(QVector<QString> keys);//���������� ������ ��� ������
    ShemE* getFastestElement(QChar typeAction, int kolInput, QString id);//����� ��� ��������� ������ �������� ��������

    QChar curAction;
    QVector<QString> elements;//������ ���� ���������� �������//�������� ���������� �� ���� ������������ �����������
    formulSearch* bestFormul;//������� ���������� ��� ��������� ���� ���������
    formulSearch* tyi;
    QStack<QChar> formul;
    QStack<QChar> formul2;
    QString formulT;
    QString ftemp;
    bool optimal;
    QMap<QString,bool> variables;//������ ����������
    QString currentVariable;
    QMap<QString,QString> actions;//������ ��������� ��������, ������ ("@0","X1*@1")
    int counter;//������� ����� ��������
    int counterForBest;//counter ������� �������� ��� �������� ������ �������� ����������
    Shem* shema;//��������� �� ����������� �����





    //now ������ ���� ���������� �� actions �� ���� ��������� ��������� (��������� ����)



};

#endif // COMPLY_H
