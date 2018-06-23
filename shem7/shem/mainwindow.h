#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenu>
#include <QAction>  //подпункт меню
#include <QDebug>
#include <QFileDialog>
#include <karno.h>
#include <QGridLayout.h>
#include "paintwidget.h"
#include "qpushbutton.h"
#include <QWidget>

//#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include "formselectelements.h"
#include "TransitionAuthomat.h"

#include "buildauthomat.h"
#include "ShemAtBush.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    PaintWidget *w;
private:
    Ui::MainWindow *ui;
    void createMenus(); //создание меню
    int authomatIndex;
    int shemIndex;

private slots:
    void createFromFunction();
    void createFromTT();
    void showMessage();//слот для меню
    void createAuthomat();
    //void createAuthomatNext(int input, int state);
    void on_tabWidget_tabCloseRequested(int index);
};

#endif // MAINWINDOW_H
