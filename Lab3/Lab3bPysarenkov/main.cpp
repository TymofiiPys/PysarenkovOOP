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
    std::vector<double> c = {1,2};
    std::vector<int> s = {1, -1, -1};
    int m = 3, n = 2;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MAX);
    
    std::vector<std::vector<double>> ar = {{2,3,-1,0,0},{-2,3,0,1,0},{2,-1,0,0,1}};
    std::vector<double> br = {1,5,4};
    std::vector<double> cr = {-1,-2,0,0,0};
    std::vector<int> sr = {0, 0, 0};
    int mr = 3, nr = 5;
    Constr* cres = new Constr(ar, sr, br, mr, nr);
    LinFunc* lfres = new LinFunc(cr, nr, MIN);
    
    cc->Standartify();
    lf->set_n(cc->get_n());
    lf->Standartify();
    
    CHECK(*cc == *cres);
    CHECK(*lf == *lfres);
    
    a = {{8,-5},{2,7},{2,2}};
    b = {17,5,1};
    c = {3,1};
    s = {-1, -1, 1};
    m = 3, n = 2;
    cc = new Constr(a, s, b, m, n);
    lf = new LinFunc(c, n, MAX);
    
    ar = {{8,-5, 1,0,0},{2,7,0,1,0},{2,2,0,0,-1}};
    br = {17,5,1};
    cr = {-3,-1,0,0,0};
    sr = {0, 0, 0};
    mr = 3, nr = 5;
    cres = new Constr(ar, sr, br, mr, nr);
    lfres = new LinFunc(cr, nr, MIN);
    
    cc->Standartify();
    lf->set_n(cc->get_n());
    lf->Standartify();
    
    CHECK(*cc == *cres);
    CHECK(*lf == *lfres);
    
    delete cc;
    delete cres;
    delete lf;
    delete lfres;
}

TEST_CASE("Canonify test"){
    std::vector<std::vector<double>> a = {{2,3,-1,0,0},{-2,3,0,1,0},{2,-1,0,0,1}};
    std::vector<double> b = {1,5,4};
    std::vector<int> s = {0, 0, 0};
    int m = 3, n = 5;
    Constr* cc = new Constr(a, s, b, m, n);
    
    std::vector<std::vector<double>> ar = {{2,3,-1,0,0,1},{-2,3,0,1,0,0},{2,-1,0,0,1,0}};
    std::vector<double> br = {1,5,4};
    std::vector<int> sr = {0, 0, 0};
    int mr = 3, nr = 6;
    Constr* cres = new Constr(ar, sr, br, mr, nr);

    cc->Canonify();
    CHECK(*cc == *cres);
    
    a = {{8,-5, 1,0,0},{2,7,0,1,0},{2,2,0,0,-1}};
    b = {17,5,1};
    s = {0, 0, 0};
    m = 3, n = 5;
    cc = new Constr(a, s, b, m, n);
    
    ar = {{8,-5, 1,0,0,0},{2,7,0,1,0,0},{2,2,0,0,-1,1}};
    br = {17,5,1};
    sr = {0, 0, 0};
    mr = 3, nr = 6;
    cres = new Constr(ar, sr, br, mr, nr);
    
    cc->Canonify();
    CHECK(*cc == *cres);
    
    delete cc;
    delete cres;
}
