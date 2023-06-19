#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Trees.h"

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
    
    void on_pushButton_Add_clicked();
    
    void on_pushButton_Remove_clicked();
    
    void on_action_save_triggered();
    
    void on_action_save_as_triggered();
    
    void on_action_undo_triggered();
    
    void on_pushButton_Analyze_clicked();
    
    void on_pushButton_Traverse_clicked();
    
private:
    Ui::MainWindow *ui;
    TreeFacade<int>* FacadeInt;
    TreeFacade<double>* FacadeDouble;
    TreeFacade<std::string>* FacadeString;
    QString typenode;
    QString treetype;
    QString savepath;
};

#endif // MAINWINDOW_H
