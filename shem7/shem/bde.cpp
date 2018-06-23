#include "bde.h"
#include <QMessageBox>

BDE::BDE()
{

}
void BDE::inicialize()
{
    if(db.isOpen())
        return;
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString pathToDB = QDir::currentPath()+QString("/elementsd2");
    //QString pathToDB = "./elementsd2";
    db.setDatabaseName(pathToDB);
    QFileInfo checkFile(pathToDB);

    if (checkFile.isFile())
    {
        if (db.open() )
        {
            qDebug() << "[+] Connected to Database File " +QDir::currentPath();
        }
        else
        {
            qDebug() << "[!] Database File was not opened";
            QMessageBox msgBox;
            msgBox.setText("[!] Database File was not opened  "+db.lastError().text());
            msgBox.exec();
        }
    }
    else
    {
        qDebug() << "[!] Database File does not exist";
        QMessageBox msgBox1;
        msgBox1.setText("[!] Database File does not exist  "+pathToDB);
        msgBox1.exec();
    }
    query = new QSqlQuery(db);
}

BDE* BDE::getInstance()
{
    static BDE instance;
    return &instance;
}

QSqlQuery* BDE::make(QString request)//выборка из базы//учитывает selectedE
{
    qDebug() << "[>] reqvest";
    qDebug() <<request;
    if(selectedE.length()>0)
    {
        QString listN;
        foreach(QString name, selectedE)
        {
            if(listN.length() > 0)
                listN.append(" or ");
            listN.append(QString("name like '%1' ").arg(name) );
        }

        //name like''

        QString newRequest = QString("select * from(%1) where %2 ").arg(request,listN);
        //qDebug()<<QString(">> %1").arg(newRequest);
        //QMessageBox msgBox;
        //msgBox.setText( QString(">> %1").arg(newRequest) );
        //msgBox.exec();
        request = newRequest;
    }
    inicialize();
    delete(query);
    query = new QSqlQuery(db);
    if(!query->exec(request))
    {
        qDebug() << "[!] bad reqvest";
        qDebug() << query->lastError().text();
    }
    return query;
}

void BDE::insert(QString request)
{
    inicialize();
    delete(query);
    query = new QSqlQuery(db);
    if(!query->prepare(request))
    {
        qDebug() << "[!] bad reqvest";
        qDebug() << query->lastError().text();
    }
    query->exec(request);
    qDebug() << "[!] bad reqvest ";
    qDebug() << query->lastError().text();
}

void BDE::setSelectedE(QList<QString> elements)
{
    clearSelectedE();
    selectedE = elements;
}

void BDE::clearSelectedE()
{
    selectedE.clear();
}
