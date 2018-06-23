#include "sheme.h"

currentWire::currentWire()
{
    nextWire=-1;
    nextE=0;
}

currentWire::currentWire(ShemE *nextElement,int numInput)
{
    makeLink(nextElement, numInput);
}

void currentWire::makeLink(ShemE* nextElement,int numInput)
{
    nextWire=numInput;
    nextE=nextElement;
}

currentWire::~currentWire()
{
    nextE->wire.clear();
    delete nextE;
}

currentWire::currentWire(currentWire* wire)
{
    currentWire();
    //curWire=wire.curWire; в мапе указывается по ключу к какомы выходу принадлежит провод
    nextWire=wire->nextWire;
    nextE=wire->nextE;
}
