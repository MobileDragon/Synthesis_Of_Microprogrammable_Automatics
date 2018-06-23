#include "comply.h"
#include <QVariant>

//#include <math.h>

Comply::Comply()
{
    this->broken="";
    optimal=false;
    formulT="";
    ftemp="";
    counter=0;
    this->bestFormul = new formulSearch();
    this->bestFormul->main_key="";
    this->bestFormul->numElements=0;
    this->bestFormul->numInclude=20;
    this->bestFormul->numInputs=1;



}

Comply::Comply(Shem* shema, bool j, QString out)
{
    this->outy=out;
    this->broken="";
    optimal=j;
    formulT="";
    ftemp="";
    counter=0;
    this->bestFormul = new formulSearch();
    this->bestFormul->main_key="";
    this->bestFormul->numElements=0;
    this->bestFormul->numInclude=0;
    this->bestFormul->numInputs=0;

    this->shema=shema;
}

int Comply::getMaxInput(int max,QSqlQuery *query)//находит элемент с числом входов == max,меньше его
{//,либо больше в случае когда нет меньше
    query->first();
    //query->next();
    int maxOfB = query->value(0).toInt();
    do
    {
        int num = query->value(0).toInt();
        //int razn=
        if( abs(num-max) < abs(maxOfB-max) )
        {
            maxOfB=num;
        }
        else
            if( abs(num-max) == abs(maxOfB-max) )
            {
                if(num < maxOfB)
                    maxOfB=num;

            }
        if(maxOfB == max)
            return max;
    }while (query->next());
    return maxOfB;
}

//j-число входов для элемента который следует найти
//query-список доступных элементов
//temp-формкла которая генерируется(содержание formul немного изменить)//содержит QMap<QString,QString> formul;
//balanceSympl-список оставшихся переменных
//cur_num-номер для генераци нового ключа
void Comply::searchElements(int j, QSqlQuery *query, formulSearch* temp,
                            QVector<QString> balanceSymplList, int cur_num)
{
    //query->first();
    QStack<QString> balanceSympl;//копирование в стек для удобства
    foreach(QString e, balanceSymplList)
    {
        balanceSympl.push(e);
    }
    //
    formulSearch* cur_temp=new formulSearch;//заполнить
    int input = getMaxInput(j, query);
    QString str = "";
    QString lastInut = 0;//последний добавленный элемент
    if(temp->numInputs == 0)
    {
        for(int i=0; i<input; i++)
        {
            if(!balanceSympl.isEmpty())
            {
                str += balanceSympl.top();
                lastInut = balanceSympl.top();
                balanceSympl.pop();
            }
            else
                str += lastInut;
            str += curAction;//+*
        }
        //удалить последний символ
        str = str.left(str.length()-1);

        cur_temp->formul.insert(makeKey(++cur_num), str);//запись
        cur_temp->main_key = makeKey(cur_num);
        cur_temp->balanceKey.insert(makeKey(cur_num), 1);
        cur_temp->numElements = 1;
        cur_temp->numInclude = 1;
        cur_temp->numInputs = input;
    }
    else
    {
        if(optimal)
        {
            str = "(" + temp->main_key + ")";
            for(int i=1; i<input; i++)
            {
                str += curAction;//+*
                if(!balanceSympl.isEmpty())
                {
                    str += balanceSympl.top();
                    lastInut = balanceSympl.top();
                    balanceSympl.pop();
                }
                else
                    str += lastInut;
            }
            foreach(QString key, temp->formul.keys())
            {
                cur_temp->formul.insert(key, temp->formul[key]);
            }
            cur_temp->formul.insert(makeKey(++cur_num), str);
            cur_temp->main_key = makeKey(cur_num);
            cur_temp->numElements = temp->numElements+1;
            cur_temp->numInclude = temp->numInclude+1;
            cur_temp->numInputs = temp->numInputs + input-1;
        }
        else
        {
            int num_key=0;//количество ключей подаваемых на входы текущего элемента
            int cur_rung=0;//ранг от 1(это уорвень) элемент только с иксами является первым уровнем
            foreach(QString key, temp->balanceKey.keys())//временное сохранение(постоянное)
            {
                cur_temp->balanceKey.insert(key, temp->balanceKey[key]);
            }
            for(int i=0; i<input; i++)
            {
                if(!balanceSympl.empty())
                {
                    str += balanceSympl.top();
                    lastInut = balanceSympl.top();
                    balanceSympl.pop();
                    cur_rung = 1;
                }
                else //нужен список уровней для ключей, и использовать их начиная с нижних
                {
                    if(!cur_temp->balanceKey.isEmpty())
                    {
                        int minR = cur_temp->balanceKey.begin().value();//самый высокий ранг на текущий момент
                        QString minK = cur_temp->balanceKey.begin().key();
                        foreach(QString key, cur_temp->balanceKey.keys())
                        {
                            if(cur_temp->balanceKey[key] < minR )
                            {
                                minR = cur_temp->balanceKey[key];
                                minK = key;

                            }
                        }
                        cur_temp->balanceKey.remove(minK);
                        num_key++;

                        str += "(" +  minK + ")";
                        lastInut = "(" +  minK + ")";
                        cur_rung = minR+1;
                    }
                    else
                        str += lastInut;

                }
                str += curAction;//+*
            }
            //удалить последний символ
            str = str.left(str.length()-1);

            cur_temp->formul.insert(makeKey(++cur_num), str);//запись
            foreach(QString key, temp->formul.keys())
            {
                cur_temp->formul.insert(key, temp->formul[key]);
            }
            cur_temp->balanceKey.insert(makeKey(cur_num), cur_rung);//main_key неиспользуется
            //cur_temp->main_key = makeKey(cur_num);//
            cur_temp->numElements = temp->numElements+1;
            cur_temp->numInclude = cur_rung;//это число вложений для только-что добавленного элемента
            //проверка в самом начале выйдет если там несколько выходов
            cur_temp->numInputs = temp->numInputs + input - num_key;





        }
    }





/*
    //formulSearch cur_temp;//заполнить
    if(temp->numInputs != 0)
        cur_temp.formul = "("+temp->formul+curAction+ "("+str+")" +")";
    else
        cur_temp.formul = "("+str+")";
    cur_temp.numElements = temp->numElements + input-1;
    */
    //какая-то глобальня переменная хранит текущее действие(+ *) ты лентяй
    //int i = j - input;//число входов Х и balanceKey//строка не нужна



    QVector<QString> balanceSymplCur;//дл следующего вызова searchElements
    while(!balanceSympl.isEmpty())
    {
        balanceSymplCur.append(balanceSympl.top());
        balanceSympl.pop();
    }


    int i = cur_temp->balanceKey.size() + elements.size() - cur_temp->numInputs;
    //число входов Х и balanceKey
    if(optimal)
        i+=1;
    while(i > 1)
    {
        //formulSearch tempFormul = searchElements(i, query,);
        searchElements(i, query, cur_temp, balanceSymplCur, cur_num);
        i--;
    }
    if(isBest(cur_temp))
        counterForBest=cur_num;
}

bool Comply::isBest(formulSearch* tempFormul)
{
    if(tempFormul->numInputs < elements.size() || tempFormul->balanceKey.size()>1)//добавить случай когда выходов больше 1(сделано)
        return false;
    if((bestFormul->numInputs) == 0)
    {

        bestFormul = tempFormul;

        return true;
    }
    if(tempFormul->numInputs > bestFormul->numInputs)
        return false;
    if(tempFormul->numInputs < bestFormul->numInputs)
    {
        bestFormul = tempFormul;
        return true;
    }
    if(optimal)
    {
        if(tempFormul->numElements < bestFormul->numElements)
        {
            bestFormul = tempFormul;
            return true;
        }
        if(tempFormul->numElements > bestFormul->numElements)
            return false;
        if(tempFormul->numInclude < bestFormul->numInclude)
        {
            bestFormul = tempFormul;
            return true;
        }
    }
    else
    {
        if(tempFormul->numInclude < bestFormul->numInclude)
        {
            bestFormul = tempFormul;
            return true;
        }
        if(tempFormul->numInclude > bestFormul->numInclude)
            return false;
        if(tempFormul->numElements < bestFormul->numElements)
        {
            bestFormul=tempFormul;
            return true;
        }
    }
}

void Comply::decompos()//подготовить actions для заполнения схемы
{
    clearInaction();//удаление переходов(ничего не содержащих)

    QMap<QString,QString> actions_temp;

    //optimal=true
    foreach(QString key, actions.keys())
    {
        elements=composInput(key);//элементы подаваемые на вход разбиваемого объединения
        int kolNead = elements.size();
        if(kolNead > 1)
        {


            QString sqlQuer=(QString)"select wires.numInput as numInput, elements.name from elements, wires, operations"
                        +" where (elements.idwires=wires.id and elements.idoperations=operations.id)"
                        +" and operations.operation like";


            int colOperations = whotCountOperations(key);
            if(whotAction(key)=='+'  )
            {
                sqlQuer+="'1'";
            }
            if(whotAction(key)=='*'  )
            {
                sqlQuer+="'&'";
            }
            if(whotAction(key)=='~'  )
            {
                sqlQuer+="'!1'";
            }
            QSqlQuery *query = BDE::getInstance()->make(sqlQuer);

            bool flag=true;
            QVector<int> max;
            max.append(kolNead);

            int i=kolNead;
            int filled=0;
            int cur_num;
            counterForBest=counter;
            while(i > 1)//ошибка(исправлено)//добавить еще использ элементов с отрицанием на выходе
            {//и где-то еще вынос общих действий за скобки(отдельный метод)для optimal=true
                formulSearch* tempS = new formulSearch;//заполнить
                QString y="";
                tempS->main_key="";
                tempS->numElements=0;
                tempS->numInclude=0;
                tempS->numInputs=0;

                cur_num = counter;
                searchElements(kolNead, query, tempS, elements, cur_num);
                i--;
            }
            counter=counterForBest;

            // bestFormul хранит уже лучшую формулу
            //дальше изменение имени формулы для того, чтобы ссылаться по старому ключу
            if(optimal)
            {
                QString temp_value = bestFormul->formul.value(bestFormul->main_key);
                bestFormul->formul.erase( bestFormul->formul.find( bestFormul->main_key ) );
                bestFormul->formul.insert(key, temp_value);
            }
            else
            {
                QString temp_key = bestFormul->balanceKey.keys()[0];

                QString temp_value = bestFormul->formul.value(temp_key);
                bestFormul->formul.erase( bestFormul->formul.find( temp_key ) );
                bestFormul->formul.insert(key, temp_value);
            }
            foreach(QString key2, bestFormul->formul.keys())//сохранение списка действий
            {
                actions_temp.insert(key2, bestFormul->formul.value(key2));


            }
            this->bestFormul->main_key="";//очищение bestFormul
            this->bestFormul->numElements=0;
            this->bestFormul->numInclude=0;
            this->bestFormul->numInputs=0;
            this->bestFormul->balanceKey.clear();
            this->bestFormul->formul.clear();


        }
        else
        {
            actions_temp.insert(key, actions.value(key));
        }

        //

    }
    actions.clear();
    actions=actions_temp;

    fillShema();//заполнить схему
    //дальше протестировать схему и поставить более медленные элементы, где это можно
}

QChar Comply::whotAction(QString key)
{
    int length=actions[key].length();
    QString str=actions[key];
    int i=0;
    while(i<length)
    {
        if( str[i] =='+')
        {
            curAction = '+';
            return'+';
        }
        if(str[i]=='*')
        {
            curAction = '*';
            return'*';
        }
        if(str[i]=='~')
        {
            curAction = '~';
            return'~';
        }
        i++;
    }
    curAction = 'E';
    return 'E';
}

int Comply::whotCountOperations(QString key)
{
    int countOperations=0;
    QChar operation=whotAction(key);
    int length=actions[key].length();

    QString str=actions[key];
    int i=0;
    while(i<length)
    {
        if(str[i] == operation)
            countOperations++;
        i++;
    }
    return countOperations;
}

void Comply::inicializeInputs(QString dnf)//инициализация списка переменных пока только true
{
    for(int i=0;i<dnf.length();i++)//инициализация списка переменных
                                    //(вызывается для подстановки всех вариантов значений)
                                    //и заполнения таблици истинности
    {
        QString temp="";
        if( (dnf[i]).isLetter() )
            do
            {
                temp+=dnf[i];
                i++;
            }while( dnf[i].isLetterOrNumber() );
        if(temp.length()>0)
            variables.insert(temp,true);
    }
}

QVector<QString> Comply::composInput(QString key)//возвращает список id подаваемых на вход операции
{//должен считать повторения а данном элементе от ключа
    QVector<QString> masInput(1);
    int length=actions[key].length();
    QString str=actions[key];
    int i=0;
    int j=0;
    while(i<length)
    {
        if(str[i]=='+' || str[i]=='*')
        {
            masInput.append("");
            j++;
        }
        else
        {
            if(str[i].isLetterOrNumber() || str[i] == '@' || str[i] == '(' || str[i] == ')')//()
            masInput[j]+=str[i];
        }
        i++;
    }
    return masInput;
}






void Comply::decomposNegative()
{
    QMap<QString,QString> tempActions;
    counter=actions.size()-1;//должно быть иаксимально записаный ранее ключь
    foreach(QString key, actions.keys())
    {
        QString action = actions[key];
        int length=action.length();
        int i=0;
        while(i<length)
        {
            if(action[i]=='~')//если есть отрацание
            {
                //строка с переменной
                QString copy="";
                while(i<length)
                {
                    QChar c=action[i];

                    if(c=='+' || c=='*')
                    {
                        break;
                    }
                    copy+=c;
                    i++;
                }
                tempActions[makeKey(++counter) ] = copy;  //то вынести это как отдельное дейтвие
                tempActions[key] += makeKey(counter); //и подставим указатель на это действие
            }
            else
            {
                tempActions[key]+=action[i];
                i++;
            }
        }
    }
    actions.clear();
    actions=tempActions;
}

void Comply::clearInaction()
{
    QMap<QString,QString> tempActions;
    QMap<QString,QString> links;//бездейтсвующие элементы
    counter=actions.size()-1;//дальше эту строку использовать нельзя
    foreach(QString key, actions.keys())
    {
        if(whotCountOperations(key)==0)
        {
            links[key]+=actions[key];
        }
        else
        {
            //проверка ключа на наличие в links
            bool flag=false;
            foreach(QString keyLink, links.keys())
            {
                if(key==links[keyLink])//не выдерживает скобок
                {
                    tempActions[keyLink]+=actions[key];
                    flag=true;
                    break;
                }
            }
            if(!flag)
                tempActions[key]+=actions[key];
        }
    }
    actions.clear();
    actions=tempActions;
}

int Comply::getKolPovtor(QString search_key)//число входов с данным элементом
{
    int kolPovtor=0;
    foreach(QString key, actions.keys())
    {
        QVector<QString> keys2 = composInput(key);
        foreach(QString key2 ,keys2)
        {
            if(key2 == search_key)
                kolPovtor++;
        }
    }
    return kolPovtor;
}

bool Comply::isKnownInputs(QString keyAction, QVector<QString> knownInputs)//проверка выражения
{
    QVector<QString> cur_input = delBrekets(composInput(keyAction));
    foreach(QString key, cur_input)
    {
        bool flag = true;//неизвестный вход
        foreach(QString key2,knownInputs)
        {
            if(key2 == key)
            {
                flag=false;
                break;
            }
        }
        if(flag)
        {
            return false;
        }
    }
    return true;
}

QVector<QString> Comply::delBrekets(QVector<QString> keys)//возвращает список без скобок
{
    QVector<QString> temp;
    foreach(QString value, keys)
    {
        QString str="";
        int i=0;
        while(i<value.length())
        {
            if(value[i] == '(' || value[i] == ')')
                i++;
            else
            {
                str += value[i];
                i++;
            }
        }
        temp.append(str);
    }
    return temp;
}

ShemE* Comply::getFastestElement(QChar typeAction, int kolInput, QString id)
{
    QString action;
    switch(typeAction.toAscii())
    {
        case'+':action="'1'";
        break;
        case'*':action="'&'";
        break;
        case'~':action="'!1'";
    }
    QString sqlQuer=(QString)"select elements.name, elements.time, wires.numInput from elements, wires, operations"
                +" where (elements.idwires=wires.id and elements.idoperations=operations.id)"
            +" and operations.operation like"+action
            +"and wires.numInput="+QString("%1 ").arg(kolInput);

    QSqlQuery *query = BDE::getInstance()->make(sqlQuer);

    query->next();
    QString bestName = query->value(0).toString();
    double bestTime = query->value(1).toDouble();
    double time;
    while(query->next())
    {
        time= query->value(1).toInt();
        if(time < bestTime)
        {
            bestTime = time;
            bestName = query->value(0).toString();
        }
    }
    //инициализировать список выходов
    //QVector<int> output;
    //output.append(query->value(2).toInt());//номер выхода
    ShemE* e = new ShemE(bestName, id);
    return e;
}

void Comply::fillShema()//это делать
{
    QVector<QString> knownKeys;//список известных ключей
    //может также хранить єлементы до которых недостучаться
    //запись входящих Х
    foreach(QString key, variables.keys() )
    {
        ShemE* e =new ShemE(key);//Х
        shema->addBack(e);
        knownKeys.append(key);
        //вилки добавлять!!!
        int kol = getKolPovtor(key);
        if(kol > 1)
        {
            shema->addWilka(key, kol);
        }
    }
    //запись элементов actions не содержащих неизвестных ключей
    QVector<QString> oldKeys;//список уже задействованных ключей
    int i=actions.size();
    QString last_key="";
    while(i>0)//пока не задействованы все actions
    {
        foreach(QString key, actions.keys())
        {
            if(isKnownInputs(key, knownKeys))
            {
                bool flag = false;
                foreach(QString key2, oldKeys)
                {
                    if(key2 == key)
                    {
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                {
                    i--;
                    oldKeys.append(key);
                    QVector<QString> cur_inputs = delBrekets(composInput(key));
                    //метод для получения самого быстрого элемента
                    ShemE* e = getFastestElement(whotAction(key), cur_inputs.size(), key);//elemental
                    shema->addBack(e, cur_inputs);//запись  элемента(подключение)
                    knownKeys.append(key);
                    int kol = getKolPovtor(key);
                    if(kol > 1)
                    {
                        shema->addWilka(key, kol);
                    }
                    last_key = key;
                }
            }
        }
    }
    //подключение выходаY
    QVector<QString> input;
    input.append(last_key);
    ShemE* e =new ShemE(1,outy);//y1(нумерация для пользователя с 1)

    //дочитать схему до конца
    while(shema->end->elements.size()>1)
        shema->end=Shem::next(shema->end);
    shema->addBack(e, input);

}

QString Comply::decompositionOfLogicalOperations(QString dnf)//преобразование операций для использования элементов
{
    bool answer;

    inicializeInputs(dnf);//инициализация списка переменных пока только true(пока не важно чем)
    for(int i=dnf.length()-1; i>=0; i--)//заполнение стека
    {
        formul.push(dnf[i]);
    }

    counter=0;
    currentVariable="";
    QVector<int> output(1);
    output[0]=3;
    answer = expr();
    decomposNegative();
    //----
/*ПРИМЕР
    ShemE* e =new ShemE("not12","id1",output);
    shema->addBack(e);
    ShemE* ee =new ShemE("not12","id2",output);
    QVector<QString> input;
    input.append("id1");
    shema->addBack(ee,input);
*/


    //actions разложить на элементарные операции учитывая bool optimal
    decompos();//более детальное разбиение для размерности в один элемент

    //заполнение shema


    if(answer)
        return"1";
    else
        return"0";
}

//-------------
QString Comply::makeKey(int i)
{
    QString key="@";
    key+=(QString::number(i) );
    return key;
}

bool Comply::expr()//сумма
{
    //создание новой собаки и запись действий в нее
    int tempCounter=counter;//

    //
    bool result = factor();
    QChar c;

    bool flagAction=false;
    while (!formul.empty())
    {

        c=formul.top();
        formul.pop();

        while (c == ' ')//пробелы после х
        {
            c=formul.top();
            formul.pop();
            //c = std::cin.get();
        }
        if(c=='+')
        {

            if(currentVariable.length()>0)
            {
                actions[makeKey(tempCounter)]+=currentVariable;//
            }
            else
            {
                actions[makeKey(tempCounter)]+=makeKey(counter);//
            }

            actions[makeKey(tempCounter)]+=c;//
            bool tempR=result;
            result=(factor() || tempR);
            flagAction=true;
        }
        else
        {

            formul.push(c);

            actions[makeKey(tempCounter)]+=currentVariable;//

            currentVariable="";
            return result;
        }
    }

    actions[makeKey(tempCounter)]+=currentVariable;//

    return result;
}

bool Comply::variable()//подстановка переменной
{
    bool result=false;
    bool sign = true;
    QChar c;

    c=formul.top();
    formul.pop();

    while (c == ' ')
    {
        c=formul.top();
        formul.pop();
    }

    if (c == '~')
    {
        sign = false;
    }
    else
        formul.push(c);//в начале цикла сниму

    if(c.isNumber())//нет уборки пробелов
    {
        formul.pop();
        if(c=='0')
            result=false;
        else
            result=true;
        if(!sign)
            result=!result;
        return result;
    }
    QString temp="";
    while (!formul.empty())
    {
        c=formul.top();
        formul.pop();

        while (c == ' ')
        {
            c=formul.top();
            formul.pop();
        }

        if (c.isLetterOrNumber())
        {
            temp+=c;
        }
        else
        {

            formul.push(c);
            break;
        }

    }
    if(temp.length()>0)
    {
        result=variables.value(temp);
        currentVariable=temp;
    }
    if(!sign)
        result=!result;
    return result;
}

bool Comply::factor()//произведение
{
    bool result = brackets();
    QChar c;

    int tempCounter=counter;//
    bool flag=false;//

    while (!formul.empty())
    {
        c=formul.top();
        formul.pop();

        while (c == ' ')
        {
            c=formul.top();
            formul.pop();
        }
        if(c=='*')
        {
            if(!flag)//
            {
                flag = true;
                tempCounter = ++counter;//
            }
            actions[makeKey(tempCounter)]+=currentVariable;//
            actions[makeKey(tempCounter)]+=c;//

            bool tempR=result;
            result=(brackets() && tempR);
            //actions[makeKey(tempCounter)]+=currentVariable;//
        }
        else
        {
            if(flag)
            {
                actions[makeKey(tempCounter)]+=currentVariable;//
                currentVariable=makeKey(tempCounter);//
            }
            formul.push(c);
            return result;
        }
    }
    if(flag)
    {
        actions[makeKey(tempCounter)]+=currentVariable;//
        currentVariable=makeKey(tempCounter);//
    }




    return result;
}

bool Comply::brackets()//скобки
{
    bool result;
    bool sign = true;
    QChar c;

    c=formul.top();
    formul.pop();

    while (c == ' ')
    {
        c=formul.top();
        formul.pop();
    }

    if (c == '~')
    {
        sign = false;
        c=formul.top();
        formul.pop();
    }

    while (c == ' ')
    {
        c=formul.top();
        formul.pop();
    }

    if (c == '(')
    {
        counter++;//
        int tempCounter=counter;

        result = expr();

        currentVariable="("+makeKey(tempCounter)+")";//
        if(!sign)
        {
            currentVariable="~"+currentVariable;
            result=!result;
        }

        c=formul.top();
        formul.pop();

        if (c != ')')
        {
            broken="The wrong placement of brackets \")\"! ";
            return false;
        }
        //создать дальше currentVariable


        return result;
    }
    else
    {
        formul.push(c);

        result=variable();
        if(!sign)
        {
            currentVariable="~"+currentVariable;
            result=!result;
        }

        return result;
    }

}










//--------


