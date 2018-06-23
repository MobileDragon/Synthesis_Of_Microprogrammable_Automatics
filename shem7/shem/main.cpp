#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QClipboard>

#include "paintwidget.h"



using namespace std;
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1251"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

    QApplication a(argc, argv);
    MainWindow w;
    //PaintWidget w;
    //PaintWidget c;
    w.show();
    //c.show();
    
    return a.exec();
}
