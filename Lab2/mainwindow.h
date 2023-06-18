#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "TreeFacade.h"

//template<typename T>
//class Node;

//template<typename T>
//class TreeFacade;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_action_exit_triggered();
    
    void on_pushButton_create_tree_clicked();
    
private:
    Ui::MainWindow *ui;
//    TreeFacade<int> a;
};

#endif // MAINWINDOW_H
