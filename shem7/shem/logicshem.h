#ifndef LOGICSHEM_H
#define LOGICSHEM_H
#include "currentshem.h"

class LogicShem//схема логической функции/автомата с тригерами
{
public:
    LogicShem();

    CurrentShem end;
    CurrentShem Start;
};

#endif // LOGICSHEM_H
