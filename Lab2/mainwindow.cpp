#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    savepath = "";
    QGraphicsScene *s = new QGraphicsScene(0, -10, ui->graphicsView_tree->width(), ui->graphicsView_tree->height(), ui->graphicsView_tree);
    ui->graphicsView_tree->setScene(s);
    s->addEllipse(-37.5, 0, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(42))->setPos(25,25);
//    s->addEllipse(10 + 37.5 + 10, 100, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(24))->setPos(25,25);
//    s->addEllipse(-75 - 10 - 37.5 - 10, 100, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(46))->setPos(25,25);
//    s->addEllipse(10 + 75 + 20, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(42))->setPos(25,25);
//    s->addEllipse(10, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(24))->setPos(25,25);
//    s->addEllipse(-75 - 10, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(46))->setPos(25,25);
//    s->addEllipse(-75 - 10 - 75 - 20, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
//    s->addText(QString::number(46))->setPos(25,25);
    s->addEllipse(10 + 37.5 + 10, 100, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(24))->setPos(25,25);
    s->addEllipse(-75 - 10 - 37.5 - 10, 100, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(46))->setPos(25,25);
    s->addEllipse(10 + 75 + 20, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(42))->setPos(25,25);
    s->addEllipse(10, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(24))->setPos(25,25);
    s->addEllipse(-75 - 10, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(46))->setPos(25,25);
    s->addEllipse(-75 - 10 - 75 - 20, 200, 75, 75, QPen(Qt::black), QBrush(Qt::white));
    s->addText(QString::number(46))->setPos(25,25);
    ui->graphicsView_tree->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_exit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_pushButton_create_tree_clicked()
{
    typenode = ui->comboBox_typenode->currentText();
    treetype = ui->comboBox_treetype->currentText();
    FacadeInt = nullptr;
    FacadeDouble = nullptr;
    FacadeString = nullptr;
    if(typenode == "Integer")
    {
        FacadeInt = new TreeFacade<int>(typenode.toStdString(), treetype.toStdString(), this, ui->graphicsView_tree);
        ui->lineEdit_key->setText("");
        ui->lineEdit_key->setValidator(new QIntValidator(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()));
    }
    if(typenode == "Double")
    {
        FacadeDouble = new TreeFacade<double>(typenode.toStdString(), treetype.toStdString(), this, ui->graphicsView_tree);
        ui->lineEdit_key->setText("");
        ui->lineEdit_key->setValidator(new QDoubleValidator(std::numeric_limits<double>::min(), std::numeric_limits<double>::max(), 5));
    }
    if(typenode == "String")
    {
        FacadeString = new TreeFacade<std::string>(typenode.toStdString(), treetype.toStdString(), this, ui->graphicsView_tree);
        ui->lineEdit_key->setText("");
        ui->lineEdit_key->setValidator(nullptr);
    }
    ui->groupBox_tree_manag->setEnabled(true);
    ui->action_save->setEnabled(true);
    ui->action_save_as->setEnabled(true);
    ui->action_undo->setEnabled(true);
}



void MainWindow::on_pushButton_Add_clicked()
{
    if(typenode == "Integer")
    {
        FacadeInt->AddKey(ui->lineEdit_key->text().toStdString());
    }
    if(typenode == "Double")
    {
        FacadeDouble->AddKey(ui->lineEdit_key->text().toStdString());
    }
    if(typenode == "String")
    {
        FacadeString->AddKey(ui->lineEdit_key->text().toStdString());
    }
    ui->lineEdit_key->setText("");
}


void MainWindow::on_pushButton_Remove_clicked()
{
    if(typenode == "Integer")
    {
        FacadeInt->RemoveKey(ui->lineEdit_key->text().toStdString());
    }
    if(typenode == "Double")
    {
        FacadeDouble->RemoveKey(ui->lineEdit_key->text().toStdString());
    }
    if(typenode == "String")
    {
        FacadeString->RemoveKey(ui->lineEdit_key->text().toStdString());
    }
    ui->lineEdit_key->setText("");
}


void MainWindow::on_action_save_triggered()
{
    if(savepath == ""){
        this->on_action_save_as_triggered();
    }
    else{
        if(typenode == "Integer")
        {
            FacadeInt->Save();
        }
        if(typenode == "Double")
        {
            FacadeDouble->Save();
        }
        if(typenode == "String")
        {
            FacadeString->Save();
        }
    }
}


void MainWindow::on_action_save_as_triggered()
{
    QString newfilename = QFileDialog::getSaveFileName(this, "Save as", "", "Text file (*.txt);;All files (*.*)");
    if(newfilename != ""){
        savepath = newfilename;
        if(typenode == "Integer")
        {
            FacadeInt->changeSavePath(savepath.toStdString());
        }
        if(typenode == "Double")
        {
            FacadeDouble->changeSavePath(savepath.toStdString());
        }
        if(typenode == "String")
        {
            FacadeString->changeSavePath(savepath.toStdString());
        }
        this->on_action_save_triggered();
    }
}


void MainWindow::on_action_undo_triggered()
{
    if(typenode == "Integer")
    {
        FacadeInt->Undo();
    }
    if(typenode == "Double")
    {
        FacadeDouble->Undo();
    }
    if(typenode == "String")
    {
        FacadeString->Undo();
    }
}


void MainWindow::on_pushButton_Analyze_clicked()
{
    std::string result;
    if(typenode == "Integer")
    {
        result = FacadeInt->Analyze();
    }
    if(typenode == "Double")
    {
        result = FacadeDouble->Analyze();
    }
    if(typenode == "String")
    {
        result = FacadeString->Analyze();
    }
    QMessageBox::information(this, "Analysis", QString::fromStdString(result));
}


void MainWindow::on_pushButton_Traverse_clicked()
{
    std::string type = ui->comboBox_travtype->currentText().toStdString(); 
    std::string result;
    if(typenode == "Integer")
    {
        result = FacadeInt->Iterate(type);
    }
    if(typenode == "Double")
    {
        result = FacadeDouble->Iterate(type);
    }
    if(typenode == "String")
    {
        result = FacadeString->Iterate(type);
    }
    QMessageBox::information(this, "Traversal", "Keys in " +QString::fromStdString(type + ":\n" +result));
}

