#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>

#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QWhatsThis>
#include <QRegExp>
#include <QMap>

#include "drawrect.h"
#include "elementale.h"
#include "element.h"
#include "wireofe.h"
#include "elementalwilka.h"
#include "elementalinput.h"
#include "elementaloutput.h"

//#include "QWhatsThis"
#include <QCursor>
#include <QMap>
#include "shem.h"
#include "Line.h"

class PaintWidget : public QWidget
{
    Q_OBJECT

    struct OutsWire
    {
        WireOfE* wire;
        QList<int> cur_old_outs;
    };
    struct LevelsElement
    {
        Element* e;
        QList<int> cur_old_levels;
    };
public:
    PaintWidget(QWidget *parent = 0);
    ~PaintWidget();
    void addElement(Element *obj);
    void cleanElements();
    //QMap<Element*, int> time;
    int time;

    void addWire(WireOfE *obj);
    void setShem(Shem* sh);//установка Shem, сразу создаются неоюходимые элементы и выс схема отображается
    //virtual void whatsThis();
protected:
    QList<QPoint> bridges;//точки мостов




    bool isEntryBridge(QPoint newBre);//совпадения

    int whotElement(ShemE* element);//число в качестве имени может содержать только вилка
    void setAutoposityon();//запускается перед первой отрисовкой и расставлянт автоматически элементы(устарело)
    void setGrid();//начальная сетка расстановки элементов
    void setAutoVerticalLayout();
    //


    QString levelsG;


    void paintFirst();//нанесение начальных позиций проводов
    void autoCorrected(QList<Element *> curListE, QList<Line> linesToPrevE);//смена местами уровней, входов простых элкмкнтов//изменение колен проводов
    QList<Line> descriptElementLike3Line(Element* e);//координаты для проверки пересечений элементов
    //
    Element* getELink(QString id);
    QList<WireOfE *> getWLink(ShemE *n_e);
    WireOfE *chenge_out(WireOfE *wire, QList<int> old_numbers);//смена номера выхода провода для провода(номер входа элемента)
    Element *chenge_level(Element *wire, QList<int> old_numbers);

    int chengeWiresConnected(int i,int search,QList<OutsWire> &usingOuts );/** перебор всех возможных вариантов подключений проводов к элементам
                                 по одному сособу за вызов
                                получает: номер вхождения(передавать 0),
                                номер провода который необходимо изменить(при первом вызове максимальный(отсчет от 0))
                                дальше передавать возвращаемое функцией значение.
                                Когда функция возвращает -1 перебор всех подключений завершен
                                usingOuts-список элементов с их номерами уже использованых выходов

                                **/
    int chengeElementsLevel(int i,int search,QList<LevelsElement> &usingOuts );
    //
    QList<Element*> next(QList<Element*> curListE);//список элементов входы которых можно заполнить из текущих

    //метод определяющий пересекается ли переданая линия с любой другой или элементом
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    //virtual void whatsThis(QMouseEvent *event);
    //virtual void changeEvent(QEvent * e);
virtual bool eventFilter( QObject *obj, QEvent *evt );

    virtual void paintEvent(QPaintEvent *event);

    QList<QPoint> getCollisionPoints(WireOfE* wire);
//private:
    //QList<QPoint> mDrawBuffer;
    QList<Element*> eBuffer;
    QList<WireOfE*> wBuffer;
    QMap<Element*,int> MapRung;//список рангов в которых находятся элементы
    //Level тоже с 1 начинается как и Rung
    QMap<Element*,int> mapLevel;//на каком уроане(по высоте)находится элемент в своем ранге
    int activE;
    int activW;
    Shem* shem;
    bool mDrawMode;
    int dh;//промежуток между элементами в высоту
    int dw;//промежуток между рангами элементов в ширину
    QList<Line> static_Line;//линии которые уже расствлены(не перемещаются)
    QList<WireOfE*> static_wire;//список проводов у которыйх позиция уже расставлена(не перемещать)
};

#endif // PAINTWIDGET_H
