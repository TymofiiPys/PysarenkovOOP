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
    lf->Standartify(cc->get_n());
    
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
    lf->Standartify(cc->get_n());
    
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
    std::vector<double> c = {-1,-2,0,0,0};
    std::vector<int> s = {0, 0, 0};
    int m = 3, n = 5;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MIN);
    
    std::vector<std::vector<double>> ar = {{2,3,-1,0,0,1},{-2,3,0,1,0,0},{2,-1,0,0,1,0}};
    std::vector<double> br = {1,5,4};
    std::vector<double> cr = {-1,-2,0,0,0,M};
    std::vector<int> sr = {0, 0, 0};
    int mr = 3, nr = 6;
    Constr* cres = new Constr(ar, sr, br, mr, nr);
    LinFunc* lfres = new LinFunc(cr, nr, MIN);
    
    cc->Canonify();
    lf->Canonify(cc->get_n());
    CHECK(*cc == *cres);
    CHECK(*lf == *lfres);
    
    a = {{8,-5, 1,0,0},{2,7,0,1,0},{2,2,0,0,-1}};
    b = {17,15,1};
    c = {-3,-1,0,0,0};
    s = {0, 0, 0};
    m = 3, n = 5;
    cc = new Constr(a, s, b, m, n);
    lf = new LinFunc(c, n, MIN);
    
    ar = {{8,-5, 1,0,0,0},{2,7,0,1,0,0},{2,2,0,0,-1,1}};
    br = {17,15,1};
    cr = {-3,-1,0,0,0,M};
    sr = {0, 0, 0};
    mr = 3, nr = 6;
    cres = new Constr(ar, sr, br, mr, nr);
    lfres = new LinFunc(cr, nr, MIN);
    
    cc->Canonify();
    lf->Canonify(cc->get_n());
    CHECK(*cc == *cres);
    CHECK(*lf == *lfres);
    
    delete cc;
    delete cres;
    delete lf;
    delete lfres;
}

TEST_CASE("Delta computation test"){
    std::vector<std::vector<double>> a = {{8,-5, 1,0,0,0},{2,7,0,1,0,0},{2,2,0,0,-1,1}};
    std::vector<double> b = {17,15,1};
    std::vector<double> c = {-3,-1,0,0,0,M};
    std::vector<int> s = {0, 0, 0};
    int m = 3, n = 6;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MIN);
    
    Sol_Step *step = new Sol_Step(lf, cc);
    std::vector<double> dr = {-3-2*M, -1-2*M, 0, 0, M, 0};
    std::vector<double> d = step->compute_delta();
    CHECK(dr == d);
    delete cc;
    delete lf;
    delete step;
}

TEST_CASE("Theta computation test"){
    std::vector<std::vector<double>> a = {{8,-5, 1,0,0,0},{2,7,0,1,0,0},{2,2,0,0,-1,1}};
    std::vector<double> b = {17,15,1};
    std::vector<double> c = {-3,-1,0,0,0,M};
    std::vector<int> s = {0, 0, 0};
    int m = 3, n = 6;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MIN);
    
    Sol_Step *step = new Sol_Step(lf, cc);
    std::vector<double> tr = {(double)17/8, (double)15/2, (double)1/2};
    std::vector<double> t = step->compute_theta();
    CHECK(tr == t);
    delete cc;
    delete lf;
    delete step;
}

TEST_CASE("Gauss-Jordan elimination test"){
    std::vector<std::vector<double>> a = {{8,-5, 1,0,0,0},{2,7,0,1,0,0},{2,2,0,0,-1,1}};
    std::vector<double> b = {17,15,1};
    std::vector<double> c = {-3,-1,0,0,0,M};
    std::vector<int> s = {0, 0, 0};
    int m = 3, n = 6;
    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MIN);
    
    Sol_Step *step = new Sol_Step(lf, cc);
    Constr* next = step->get_next_step_con();
    std::vector<std::vector<double>> res_a = {{0, -13, 1, 0, 4, -4}, {0,5,0,1,1,-1},{1,1,0,0,-0.5,0.5}};
    CHECK(next->get_a() == res_a);
    delete next;
    delete cc;
    delete lf;
    delete step;
}

TEST_CASE("Solution test"){
    std::vector<std::vector<double>> a = {{8,-5},{2,7},{2,2}};
    std::vector<double> b = {17,15,1};
    std::vector<double> c = {3,1};
    std::vector<int>s = {-1, -1, 1};
    int m = 3, n = 2;

    Constr* cc = new Constr(a, s, b, m, n);
    LinFunc* lf = new LinFunc(c, n, MAX);
    Solver* sol = new Solver(lf, cc);
    Solution opt = sol->solve();
    std::vector<double> optres = {(double)776/264, (double)43/33, 0, 0, (double)988/132, 0};
    
    CHECK(opt.type == FOUND_OPT);
    CHECK(opt.sol == optres);
    
    a = {{2,3},{-2,3},{2,-1}};
    b = {1,5,4};
    c = {1,2};
    s = {1, -1, -1};
    m = 3;
    n = 2;
    
    cc = new Constr(a, s, b, m, n);
    lf = new LinFunc(c, n, MAX);
    sol = new Solver(lf, cc);
    opt = sol->solve();
    optres = {4.25, 4.5, 21, 0, 0, 0};
    
    CHECK(opt.type == FOUND_OPT);
    CHECK(opt.sol == optres);
    
    delete sol;
    delete lf;
    delete cc;
}
