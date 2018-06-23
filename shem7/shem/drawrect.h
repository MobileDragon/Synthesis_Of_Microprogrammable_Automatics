#ifndef DRAWRECT_H
#define DRAWRECT_H

#include <QString>
#include <math.h>
#include <Qvector>
#include <QStringList>
#include <QDebug>
#include <QPainter>

class DrawRect
{
public:
    DrawRect();
    DrawRect(int width, int height,int posx,int posy);
    void draw(QPainter*);

private:
    int width;
    int height;
    int posx;
    int posy;
};

#endif // DRAWRECT_H
