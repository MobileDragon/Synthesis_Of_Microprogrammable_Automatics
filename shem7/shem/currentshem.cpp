#include "currentshem.h"

CurrentShem::CurrentShem()
{

}

CurrentShem::CurrentShem(CurrentShem* currentShem)
{
    foreach(ShemE* element, currentShem->elements)
    {
        elements.append(element);
    }
}

CurrentShem::~CurrentShem()
{
    elements.clear();
}

bool CurrentShem::operator== (CurrentShem* A)
{
    int kol_find=0;
    foreach(ShemE* element, A->elements)//каждый элемент полученого
    {
        foreach(ShemE* e, this->elements)//каждый элемент вызывающего
        {
            if(e == element)
            {
                kol_find++;
                break;
            }
        }
    }
    if(kol_find == A->elements.size() && kol_find == this->elements.size())
        return true;
    return false;
}

bool CurrentShem::operator!= (CurrentShem* A)
{
    if(this == A)
        return false;
    return true;
}
