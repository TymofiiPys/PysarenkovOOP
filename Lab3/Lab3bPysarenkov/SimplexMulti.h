#ifndef SIMPLEXMULTI_H
#define SIMPLEXMULTI_H
//#include "Simplex.h"
#include <thread>
#include <algorithm>
#include <string>

//Мультипотокова версія

//Використовує ті ж класи, що містять лінійні обмеження, лінійну функцію,
//але інший шлях розв'язання - перебір розв'язків при різних базисах, яких може бути
//C(n,m) - де n - кількість змінних у СЗЛП, m - кількість лін. обмежень

//int fact(int n){
//    int q = 1;
//    for(int i = 2; i <= n; ++i){
//        q *= i;
//    }
//    return q;
//}

//int Comb(int n, int k){
//    int q = 1;
//    int start = n - k > k ? n - k : k;
//    int divby = fact(n-start); 
//    for(int i = start + 1; i <= n; ++i){
//        q *= i;
//    }
//    return q / divby;
//}

std::vector<std::vector<int>> Comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    
    std::vector<std::vector<int>> q;
    
    // print integers and permute bitmask
    do {
        std::vector<int> r;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) r.push_back(i);
        }
        q.push_back(r);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return q;
}

//Приведення розширеної матриці (con->a() | con->b) до заданого базису
//(тобто, стовпчики з індексами у basis утворюватимуть одиничну матрицю)
Constr adapt_to_basis(Constr con, std::vector<int> basis){
    std::vector<std::vector<double>> a = con.get_a();
    std::vector<double> b = con.get_b();
    int m = b.size(), n = a[0].size();
    
    for(int k = 0; k < m; k++){
        double divideby = a[k][basis[k]];
        for(int j = 0; j < n; j++){
            a[k][j] /= divideby;
        }
        b[k] /= divideby;
        
        for(int i = 0; i < m; i++){
            if(i == k)
                continue;
            double factor = a[i][basis[k]];
            for(int j = 0; j < m; j++){
                a[i][j] -= factor * a[k][j];
            }
            b[i] -= factor * b[k];
        }
    }
    
    return Constr(a, std::vector<int>(m, 0), b, m, n);
}

//Пошук розв'язків із різними базисами
void find_sol_with_basis(int num_threads, int thread_id, LinFunc lf, Constr con, std::vector<std::vector<int>> arrbasis, Solution *solthr){
    std::vector<std::vector<double>> a = con.get_a();
    std::vector<double> b = con.get_b();
    std::vector<double> c = lf.get_c();
    int m = b.size(), n = a[0].size();
    std::vector<double> sol(n);
    double minlfvalue = INT_MAX;
    size_t basind = thread_id;
    
    while(basind < arrbasis.size()){
        Constr conb = adapt_to_basis(con, arrbasis[basind]);
        std::vector<double> bb = conb.get_b();
        
        std::vector<double> x(n);
        double lfvalue = 0;
        
        bool xiGreaterThanZero = true;
        for(int i = 0; i < m; ++i){
            if(bb[i] < 0){
                xiGreaterThanZero = false;
                break;
            }
        }
        
        if(xiGreaterThanZero)
        {
            for(int i = 0; i < m; ++i){
                lfvalue += c[arrbasis[basind][i]] * bb[i];
                x[arrbasis[basind][i]] = bb[i];
            }
            
            if(lfvalue < minlfvalue ){
                minlfvalue = lfvalue;
                sol = x;
            }
        }
        basind += num_threads;
    }
    
    *solthr = Solution{0, sol, minlfvalue};
}

Solution Solver::multithreadedLPP(int num_thr){
    std::vector<std::thread> threads;
    std::vector<double> sol;
    
    start_con->Standartify();
    start_lf->Standartify(start_con->n);
    
    std::vector<std::vector<int>> arrbasis = Comb(start_con->n, start_con->m);
    std::vector<Solution> solthr(num_thr);
    
//    find_sol_with_basis(num_thr, 0, *start_lf, *start_con, arrbasis, solthr[0]);
//    std::thread t1 (find_sol_with_basis, num_thr, 0, *start_lf, *start_con, arrbasis, &solthr[0]);
    
//    t1.join();
    for(int i = 0; i < num_thr; i++){
        threads.push_back(std::thread(find_sol_with_basis, num_thr, i, *start_lf, *start_con, arrbasis, &solthr[i]));
    }
    
    for(int i = 0; i < num_thr;i++){
        threads[i].join();
    }
    
    double minlfvalue = INT_MAX;
    for(int i = 0; i < num_thr; i++){
        if(solthr[i].lfvalue < minlfvalue){
            minlfvalue = solthr[i].lfvalue;
            sol = solthr[i].sol;
        }
    }
//    int type = step->get_steptype();
//    if(type == LIN_FUNC_UNLIMITED)
//    {
//        sol = std::vector<double>(start_con->n, -1);
//        lfvalue = start_lf->opt ? INT_MAX : INT_MAX * -1;
//    }
//    else
//    {
//        sol = step->get_opt();
//        lfvalue = mult(start_lf->c, sol);
//    }
//    delete step;
    
    return {0, sol, minlfvalue};
}

#endif // SIMPLEXMULTI_H
