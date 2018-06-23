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
    void setShem(Shem* sh);//��������� Shem, ����� ��������� ����������� �������� � ��� ����� ������������
    //virtual void whatsThis();
protected:
    QList<QPoint> bridges;//����� ������




    bool isEntryBridge(QPoint newBre);//����������

    int whotElement(ShemE* element);//����� � �������� ����� ����� ��������� ������ �����
    void setAutoposityon();//����������� ����� ������ ���������� � ����������� ������������� ��������(��������)
    void setGrid();//��������� ����� ����������� ���������
    void setAutoVerticalLayout();
    //


    QString levelsG;


    void paintFirst();//��������� ��������� ������� ��������
    void autoCorrected(QList<Element *> curListE, QList<Line> linesToPrevE);//����� ������� �������, ������ ������� ���������//��������� ����� ��������
    QList<Line> descriptElementLike3Line(Element* e);//���������� ��� �������� ����������� ���������
    //
    Element* getELink(QString id);
    QList<WireOfE *> getWLink(ShemE *n_e);
    WireOfE *chenge_out(WireOfE *wire, QList<int> old_numbers);//����� ������ ������ ������� ��� �������(����� ����� ��������)
    Element *chenge_level(Element *wire, QList<int> old_numbers);

    int chengeWiresConnected(int i,int search,QList<OutsWire> &usingOuts );/** ������� ���� ��������� ��������� ����������� �������� � ���������
                                 �� ������ ������ �� �����
                                ��������: ����� ���������(���������� 0),
                                ����� ������� ������� ���������� ��������(��� ������ ������ ������������(������ �� 0))
                                ������ ���������� ������������ �������� ��������.
                                ����� ������� ���������� -1 ������� ���� ����������� ��������
                                usingOuts-������ ��������� � �� �������� ��� ������������� �������

                                **/
    int chengeElementsLevel(int i,int search,QList<LevelsElement> &usingOuts );
    //
    QList<Element*> next(QList<Element*> curListE);//������ ��������� ����� ������� ����� ��������� �� �������

    //����� ������������ ������������ �� ��������� ����� � ����� ������ ��� ���������
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
    QMap<Element*,int> MapRung;//������ ������ � ������� ��������� ��������
    //Level ���� � 1 ���������� ��� � Rung
    QMap<Element*,int> mapLevel;//�� ����� ������(�� ������)��������� ������� � ����� �����
    int activE;
    int activW;
    Shem* shem;
    bool mDrawMode;
    int dh;//���������� ����� ���������� � ������
    int dw;//���������� ����� ������� ��������� � ������
    QList<Line> static_Line;//����� ������� ��� ����������(�� ������������)
    QList<WireOfE*> static_wire;//������ �������� � �������� ������� ��� �����������(�� ����������)
};

#endif // PAINTWIDGET_H
