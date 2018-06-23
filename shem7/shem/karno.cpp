#include "karno.h"

Karno::Karno()
{
}


QVector<uint32_t> Karno::readCSV(QString puth)
{
    QFile file(puth);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Ошибка открытия для чтения";
        //return;
    }

    QVector<uint32_t> masOutput(0);
    while(!file.atEnd())
    {
        QString s=file.readLine();
        QChar last=s.split(";").last().at(0);
        QString y=""; y+=last;
        try
        {
            if(last=='1'||last=='0')
                masOutput.append(y.toInt());
            else
                masOutput.append(2);
        }
        catch(...)
        {
            masOutput.append(2);
        }
    }
    return masOutput;
}

uint32_t Karno::makeDecOfBin(QString s1)//почти не нужна(неправда)
{
    uint32_t at1 = 0;
    int32_t kol = s1.length()-1;
    int32_t i = 0;
    while (i <= kol)
    {
        if(s1[kol-i]=='1')
        {
            at1 += pow(2, i);
        }
        i++;
    }
    return at1;
}

QString Karno::makeBinOfDec(uint32_t a,uint32_t capacity)
{
    QString t="";
    do
    {
        if(a % 2)
            t='1'+t;
        else
            t='0'+t;
        a /= 2;
    } while (a != 0);
    QString s1="";
    s1+=t.rightJustified(capacity,'0');
    return s1;
}


QString Karno::convert(QString s1)
{
    QString str="";
    for(uint32_t i=0;i<s1.length();i++)
    {
        char index[3]="";

        if(s1[i]=='1')
        {
            str+="X";
            str+= itoa(i+1,index,10);
            str+="*";
        }
        else if(s1[i]=='0')
        {
            str+="~X";
            str+=itoa(i+1,index,10);
            str+="*";
        }

    }
    if(str.length()==0)
        return "1";
    return str.left(str.length()-1);

}


QString Karno::getGlobal(QString s1,QString s2)
{
    QString str="";
    for(int i=0;i<s1.length();i++)
    {
        if(s1[i]==s2[i])
            str+=s1[i];
        else
        {
            str+='2';
        }

    }
    return str;

}
QVector<uint32_t> Karno::searchGlobal(QString global,QVector<uint32_t> masOutput)
{
    QVector<uint32_t> masGlobal(0);
    uint32_t capacity= pow(masOutput.size(),0.5);
    for(int i=0;i<masOutput.size();i++)
    {
        QString temp=Karno::makeBinOfDec(i,capacity);
        bool flag =true;
        for(uint32_t j=0;j<capacity;j++)
        {
            if(global[j]=='0'||global[j]=='1')
            {
                if(temp[j]!=global[j])
                {
                    flag=false;

                }
            }
        }
        if(flag)
            masGlobal.append(i);
    }
    return masGlobal;
}
bool Karno::chekNum(QVector<uint32_t> masOutput,QVector<uint32_t> masGlobal,int chek)
{
    foreach(uint32_t i,masGlobal)
    {
        if(masOutput[i]==chek)
            return true;
    }
    return false;
}
QVector<QString> Karno::separator(QVector<QString> temp_select)
{
    QVector<QString> select(0);
    foreach(QString join,temp_select)
    {
        bool flag=true;

        for(int j=0;j<temp_select.size() && flag;j++)
        {
            QString other_join =temp_select[j];
            int kol=0;
            for(int i=0;i<join.length();i++)
            {
                if(other_join[i]==join[i])
                    kol++;
                else if(other_join[i]=='2')
                {
                    kol++;
                    flag=false;
                }
            }
            if(kol!=join.length())
                flag=true;

        }
        if(flag && (!select.contains(join)) )
            select.append(join);
    }
    return select;
}

bool Karno::isContain(QString str,QString str2)
{
    int kol=0;
    for(int i=0;i<str2.length();i++)
    {
        if(str[i]==str2[i])
            kol++;
        else if(str[i]=='2')
            kol++;
    }
    bool flag=false;
    if(kol==str2.length())
        flag=true;
    return flag;
}

QVector<QString> Karno::delExcessJoin(QVector<QString> ansver,QVector<uint32_t> masOutput)
{
    QVector<QString> select(0);
    foreach(QString str, ansver)
    {
        bool flag=true;
        //входит ли в данное объединение "2"
        QVector<uint32_t> masGlobal=Karno::searchGlobal(str,masOutput);
        if(Karno::chekNum(masOutput,masGlobal,2))
        {
            //получить номера всех единиц которые входят в объединение
            int i=0;
            while(i<masGlobal.size()&&flag)
            {
                int a=masGlobal[i];
                if(masOutput[a]=='1')
                {
                    //если данный список входит в другие объединения
                    int j=0;
                    while(j<ansver.size()&&flag)
                    {
                        QString str2=ansver[j];
                        if(QString::compare(str,str2)!=0)
                            if(isContain(str2,makeBinOfDec(a,str.length() ))!=0)
                            {
                                select.append(str);
                                flag=false;
                            }
                        j++;
                    }
                }
                i++;
            }
        }
        else
            select.append(str);
    }
    return select;
}

QVector<QString> Karno::buildDNF(QString puth)
{
    QString result="";
    QVector<uint32_t> masOutput=Karno::readCSV(puth);//набор выходов
    uint32_t capacity= pow(masOutput.size(),0.5);//число входных сигналов
    QVector<QString> temp_global(0);
    for(int i=0; i<masOutput.size();i++)
    {
        if(masOutput[i]==1)//ищим единицы
        {
            QString temp1=Karno::makeBinOfDec(i,capacity);
            QString global=temp1;
            //с кем можно объединить?
            for(int j=i+1; j<masOutput.size();j++)
            {
                if(masOutput[j]>0)//с единицами или 2 ghb придумать
                {
                    QString temp2=Karno::makeBinOfDec(j,capacity);
                    //проверить на возможность объединения
                    //(существуют ли с данным условием не единицы)
                    global=Karno::getGlobal(temp1,temp2);

                    //кто еще подходит под єто описание?
                    QVector<uint32_t> masGlobal=Karno::searchGlobal(global,masOutput);
                    if(masGlobal.size()>0)
                    {
                        //есть нули?(global=0)
                        if(Karno::chekNum(masOutput,masGlobal,0))
                        {
                            global=temp1;
                        }
                    }
                }
                temp_global.append(global);//массив который нужно отсеять
            }
            //result+='+'+temp1;
        }
    }
    //отсеиваем обьединения входящие в другие
    QVector<QString> ansver=Karno::separator(temp_global);
    //удаление обьединений с "2" для тех, кто уже обьединен
    ansver=Karno::delExcessJoin(ansver,masOutput);//наверное стоит поставить перед separator
    foreach(QString str,ansver)
        result+=Karno::convert(str)+"+";
    QString h100=result.left(result.length()-1);
    return ansver;
}
