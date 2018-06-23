#ifndef TRANSITIONAUTHOMAT_H
#define TRANSITIONAUTHOMAT_H
#include <QString>
struct TransitionAuthomat
{
    QString idInput;//какой сигнал активен
    QString idOut;//сигнал который необходимо активировать
    QString prevState;//id триггера что хранил 1
    QString nextState;//триггер который должен активироваться
};

#endif // TRANSITIONAUTHOMAT_H
