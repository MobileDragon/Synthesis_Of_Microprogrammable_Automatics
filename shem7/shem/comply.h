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
//РАНГ И ЧИСЛО ВЛОЖЕНИЙ ОДНА И ТАЖЕ ЦИФРА
struct formulSearch //Объявлена структура для хранения результата подстановки элементов
{//(для decompose)
    int numInputs; //число входов у построеной схемы
    int numInclude; //количество вложений(раг+1)
    int numElements; //число элементов в схеме
    QMap<QString,QString> formul; //посроеная формула
    QMap<QString,int> balanceKey;//список еще незадействованных ключей и уровень//!только для optimimal==false
    QString main_key;//имя главного ключа, обьеденяющего остальные//!только для optimimal==true
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

    Comply();//передать список допустимых элементов при создании объекта
    Comply(Shem* shema, bool j, QString out="");
    QString decompositionOfLogicalOperations(QString dnf);


private:
    QString outy;
    bool expr();
    bool variable();
    bool factor();
    bool brackets();
    void decompos();//более детальное разбиение для размерности в один элемент //заменяет convert()
    QString makeKey(int i);
    QChar whotAction(QString key);
    int whotCountOperations(QString key);//количество действий
    void inicializeInputs(QString dnf);//инициализирует список переменных(variables )

    QVector<QString> composInput(QString key);//возвращает список id подаваемых на вход операции
    //доработать проверки
    //
    void decomposNegative();//вынос отрицания в отдельное действие
    void clearInaction();//удаление бездействий из actions
    int getMaxInput(int max,QSqlQuery* query);
    //
    void searchElements(int j, QSqlQuery *query, formulSearch* temp,
                        QVector<QString> balanceSymplList, int cur_num);//создает tempFormul
    //balance это список переменных которые не задействованы, изначально это elements
    bool isBest(formulSearch* tempFormul);//записывает в bestFormul tempFormul если лучше подходит
    //доработал алгоритм? - да

    void fillShema();//записать shema
    int getKolPovtor(QString search_key);//число входов с данным элементом
    bool isKnownInputs(QString keyAction, QVector<QString> knownInputs);//проверка выражения
    QVector<QString> delBrekets(QVector<QString> keys);//возвращает список без скобок
    ShemE* getFastestElement(QChar typeAction, int kolInput, QString id);//метод для получения самого быстрого элемента

    QChar curAction;
    QVector<QString> elements;//список имен переменных формулы//элементы подаваемые на вход разбиваемого объединения
    formulSearch* bestFormul;//формкла подходящая под выбранную базу элементов
    formulSearch* tyi;
    QStack<QChar> formul;
    QStack<QChar> formul2;
    QString formulT;
    QString ftemp;
    bool optimal;
    QMap<QString,bool> variables;//список переменных
    QString currentVariable;
    QMap<QString,QString> actions;//список поэтапных действий, хранит ("@0","X1*@1")
    int counter;//текущий номер действия
    int counterForBest;//counter который сохранен при подворке лчшего варианта построения
    Shem* shema;//указатель на заполняемую схему





    //now делать пока построение по actions из всех возможных элементов (расширить базу)



};

#endif // COMPLY_H
