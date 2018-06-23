#include "drawrect.h"


DrawRect::DrawRect()
{
}

DrawRect::DrawRect(int width, int height, int posx, int posy)
{
    this->width=width;
    this->height=height;
    this->posx=posx;
    this->posy=posy;

}

void DrawRect:: draw(QPainter *painter)
{
    painter->drawRect(width, height, posx, posy);
}
