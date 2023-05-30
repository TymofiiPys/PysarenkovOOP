#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>

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
    
    void on_pushButton_clicked();
    
    void on_pushButton_solve_clicked();
    
private:
    Ui::MainWindow *ui;
    
    int m;
    int n;
    
    std::vector<QLineEdit*> lfle;
    std::vector<std::vector<QLineEdit*>> conle;
    std::vector<QComboBox*> signs;
    QComboBox* opt;
};

#endif // MAINWINDOW_H
