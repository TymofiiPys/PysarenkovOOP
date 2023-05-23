#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "mainwindow.h"
#include "Simplex.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    context.run();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

TEST_CASE("Standartify test"){
    std::vector<std::vector<double>> a = {{2,3},{-2,3},{2,-1}};
    std::vector<double> b = {1,5,4};
    std::vector<int> s = {1, -1, -1};
    int m = 3, n = 2;

//    std::vector<std::vector<double>> ac = {{2,3},{-2,3},{2,-1}};
//    double bc[3] = {1,5,4};
//    int sc[3] = {1, -1, -1};
//    for(int i = 0; i < m; i++)
//    {
//        b[i] = bc[i];
//        sign[i] = sc[i];
//        for(int j = 0; j < n; j++){
//            a[i][j] = ac[i][j];
//        }
//    }
    Constr* cc = new Constr(a, s, b, m, n);
//    double** ar, *br;
//    int* signr;
//    int mr = 3, nr = 5;
//    ar = new double*[mr];
//    for(int i = 0; i < mr; i++)
//        ar[i] = new double[nr];
//    br = new double[mr];
//    signr = new int[mr];
    std::vector<std::vector<double>> ar = {{2,3,-1,0,0},{-2,3,0,1,0},{2,-1,0,0,1}};
    std::vector<double> br = {1,5,4};
    std::vector<int> sr = {0, 0, 0};
    int mr = 3, nr = 5;
//    for(int i = 0; i < mr; i++)
//    {
//        br[i] = bcr[i];
//        signr[i] = scr[i];
//        for(int j = 0; j < nr; j++){
//            ar[i][j] = acr[i][j];
//        }
//    }
    Constr* cres = new Constr(ar, sr, br, mr, nr);
    cc->Standartify();
    CHECK(*cc == *cres);
    delete cc;
    delete cres;
}
