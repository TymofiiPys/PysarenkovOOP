#include "mainwindow.h"
#include "includes.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QMessageBox>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_m->setValidator(new QIntValidator(0, INT_MAX));
    ui->lineEdit_n->setValidator(new QIntValidator(0, INT_MAX));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_action_exit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_pushButton_clicked()
{
//    QLineEdit *le = new QLineEdit(this);
    m = ui->lineEdit_m->text().toInt();
    n = ui->lineEdit_n->text().toInt();
    lfle = std::vector<QLineEdit*>(n);
    conle = std::vector<std::vector<QLineEdit*>>(m);
    signs = std::vector<QComboBox*>(m);
    for(int i = 0; i < m; i++){
        conle[i] = std::vector<QLineEdit*>(n+1);
    }
    
    QWidget *wi = new QWidget(this);
    QWidget *lfwi = new QWidget(this);
    QWidget *conwi = new QWidget(this);
    QVBoxLayout *vbmain = new QVBoxLayout(this);
    QGridLayout *lfgrid = new QGridLayout(this);
    QGridLayout *congrid = new QGridLayout(this);
    vbmain->setAlignment(Qt::AlignTop);
    opt = new QComboBox(this);
    opt->addItem("Max");
    opt->addItem("Min");
    
    QLabel* llinf = new QLabel("Лінійна функція");
    QLabel* la = new QLabel("L = ");
    vbmain->addWidget(llinf);
    lfgrid->addWidget(la, 1, 0);
    
    for(int i = 0; i < n; i++){
        lfle[i] = new QLineEdit(this);
        QLabel *la;
        if(i != n - 1)
            la = new QLabel("x" + QString::number((i+1)) + " + ", this);
        else
            la = new QLabel("x" + QString::number((i+1)) + " -> ", this);
        lfle[i]->setValidator(new QDoubleValidator(-INT_MAX, INT_MAX, 10, this));
        lfle[i]->setMinimumWidth(100);
        lfle[i]->setMaximumWidth(100);
        lfgrid->addWidget(lfle[i],1,2*i+1);
        lfgrid->addWidget(la,1,2*i+2);
    }
    lfgrid->addWidget(opt, 1, 2*n+1);
    lfwi->setLayout(lfgrid);
    lfwi->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    vbmain->addWidget(lfwi);
    QLabel* lcon = new QLabel("Система лінійних обмежень");
    vbmain->addWidget(lcon);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            conle[i][j] = new QLineEdit(this);
            QLabel *la;
            if(j != n - 1)
                la = new QLabel("x" + QString::number((j+1)) + " + ", this);
            else
                la = new QLabel("x" + QString::number((j+1)), this);
            conle[i][j]->setValidator(new QDoubleValidator(-INT_MAX, INT_MAX, 10, this));
            conle[i][j]->setMaximumWidth(100);
            congrid->addWidget(conle[i][j],i+3,2*j);
            congrid->addWidget(la,i+3,2*j+1);
        }
        signs[i] = new QComboBox(this);
        signs[i]->addItem("<=");
        signs[i]->addItem("=");
        signs[i]->addItem(">=");
        congrid->addWidget(signs[i], i+3, 2*n);
        conle[i][n] = new QLineEdit(this);
        conle[i][n]->setValidator(new QDoubleValidator(-INT_MAX, INT_MAX, 10, this));
        conle[i][n]->setMaximumWidth(100);
        congrid->addWidget(conle[i][n], i+3, 2*n+1);
    }
    conwi->setLayout(congrid);
    conwi->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    vbmain->addWidget(conwi);
    QString qs = "";
    for(int j = 0; j < n; j++){
        qs += "x" + QString::number((j+1)) + " >= 0";
        if(j != n-1)
            qs += ", ";
    }
    la = new QLabel(qs, this);
    vbmain->addWidget(la);
//    vbmain->setHorizontalSpacing(0);
    wi->setLayout(vbmain);
    ui->scrollArea->setWidget(wi);
    ui->pushButton_solve->setEnabled(true);
}


void MainWindow::on_pushButton_solve_clicked()
{
    bool allfilled = true;
    for(int j = 0; j < n; j++){
        if(lfle[j]->text() == "")
        {
            allfilled = false;
            QMessageBox::warning(this, "Розв'язку не буде...", "Не повністю заповнено лінійну функцію!");
            break;
        }
    }
    for(int i = 0; i < m && allfilled; i++)
    {
        for(int j = 0; j < n + 1; j++){
            if(conle[i][j]->text() == "")
            {
                allfilled = false;
                QMessageBox::warning(this, "Розв'язку не буде...", "Не повністю заповнено лінійні обмеження!");
                break;
            }
        }
    }
    if(!allfilled)
        return;
    std::vector<std::vector<double>> a(m);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            a[i].push_back(conle[i][j]->text().toDouble());
        }
    }
    std::vector<double> b(m);
    std::vector<int> s(m);
    for(int i = 0; i < m; i++){
        b[i] = conle[i][n]->text().toDouble();
        switch(signs[i]->currentIndex()){
        case 0:
            s[i] = -1;
            break;
        case 1:
            s[i] = 0;
            break;
        case 2:
            s[i] = 1;
            break;
        }
    }
    std::vector<double> c(n);
    for(int j = 0; j < n; j++){
        c[j] = lfle[j]->text().toDouble();
    }
    int optval = opt->currentIndex() == 0 ? MAX : MIN;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, optval);
    Solver* solver = new Solver(lf, cc);
    Solution solut = solver->solve();
    QString strsol = "";
    if(solut.type == LIN_FUNC_UNLIMITED)
        strsol = "Лінійна функція необмежена, розв'язку немає";
    else
    {
        strsol = "Значення лінійної функції: " + QString::number((solut.lfvalue * 100)/100) + ". Вектор змінних: (";
        for(size_t i = 0; i < solut.sol.size(); i++){
            strsol += QString::number((solut.sol[i] * 100)/100);
            if(i == solut.sol.size() - 1)
                strsol += ").";
            else
                strsol+= "; ";
        }    
    }
    QMessageBox::information(this, "Розв'язок", strsol);
    delete solver;
    delete cc;
    delete lf;
}

