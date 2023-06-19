#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <vector>
#include <iostream>

#define M 200000
enum Opt{
    MIN, MAX
};

enum SolutionStepTypes{
    NOT_A_FINAL_STEP, FOUND_OPT, LIN_FUNC_UNLIMITED
};

//!
//! \brief The Solution struct
//!
struct Solution{
    int type;
    std::vector<double> sol;
    double lfvalue;
};

//!
//! \brief The Linear Function class
//!
class LinFunc{
    //L = c[0]x[0] + c[1]x[1] + ... + c[n]x[n] -> opt
    std::vector<double> c;
    int n;      //кількість змінних
    bool opt;   //true - max, false - min
public:
    LinFunc(std::vector<double> c, int n, bool opt){
        this->c = c;
        this->n = n;
        this->opt = opt;
    }
    
    LinFunc(int n, bool opt){
        this->n = n;
        this->opt = opt;
    }
    
    void set_n(int n){
        this->n = n;
    }
    
    std::vector<double> get_c(){
        return this->c;
    }
    
    void Standartify(int nNew);//Переведення лін. функції у таку, що відповідає СЗЛП, тобто має прямувати в мінімум
    
    void Canonify(int nNew);//Переведення лін. функції у таку, що відповідає КЗЛП, тобто містить в доданках M(y1+...+yk),
    //де yi - введені штучні змінні
    
    bool operator==(const LinFunc& r){
        if(c != r.c || n != r.n || opt != r.opt)
            return false;
        return true;
    }
    
    friend class Sol_Step;
    friend class Sol_Step_Mult;    
    friend class Solver;
};

//!
//! \brief The Lnear Constraints class
//!
class Constr{
    //a[i][0]x[0] + a[i][1]x[1] + ... + a[i][n]x[n] <= (=, >=) b[i], i = [1, m]
    std::vector<std::vector<double>> a;
    
    std::vector<int> sign; //<= -> -1, = -> 0, >= -> 1
    
    std::vector<double> b;
    
    int m;  //кількість лінійних обмежень
    
    int n;  //кількість змінних
public:
    Constr(int m, int n){
        this->m = m;
        this->n = n;
    }
    
    Constr(std::vector<std::vector<double>> a, std::vector<int> sign, std::vector<double> b, int m, int n){
        this->a = a;
        this->sign = sign;
        this->b = b;
        this->m = m;
        this->n = n;
    }
    
    void Standartify();//Переведення л.о. у такі, що відповідають СЗЛП, тобто є рівностями із
    //введеними за необхідності балансними змінними
    
    void Canonify();//Переведення л.о у такі, що відповідають СЗЛП, тобто в матриці a має існувати одинична підматриця,
    //а всі елементи b мають бути невід'ємними
    
    int get_n(){
        return this->n;
    }
    
    bool operator==(const Constr& r){
        if(m != r.m || n != r.n || a != r.a || b != r.b || sign != r.sign)
            return false;
        return true;
    }
    
    std::vector<std::vector<double>> get_a(){
        return a;
    }
    
    std::vector<double> get_b(){
        return b;
    }
    
    friend class Sol_Step;
    friend class Sol_Step_Mult;
    friend class Solver;
};

//!
//! \brief The Solution Step class
//!
class Sol_Step{
    LinFunc* lf; //Лінійна функція
    
    Constr* con; //Лінійні обмеження
    
    std::vector<int> basis; //Базисні змінні
    
    std::vector<double> delta;//симплекс-різниці
    
    std::vector<double> theta;
    
    int min_delta;//індекс стовпця, для якого отримано мінімальну симплекс-різницю, або
    //змінної, яку ми занесемо до базису
    
    int min_theta;//індекс рядка, для якого отримано мінімальне тета, або
    //змінної, яку ми витягнемо із базису
    
    int steptype;//чи розв'язок закінчується на цьому кроці і висновок з розв'язку
public:
    Sol_Step(LinFunc* lf, Constr* con){
        this->lf = lf;
        this->con = con;
        this->steptype = NOT_A_FINAL_STEP;
        this->basis = compute_basis();
        this->delta = compute_delta();
        this->theta = compute_theta(); 
    }
    
    std::vector<int> compute_basis();
    
    std::vector<double> compute_delta();
    
    std::vector<double> compute_theta();
    
    Constr* get_next_step_con();
    
    int get_steptype(){
        return steptype;
    }
    
    std::vector<double> get_opt();
};

//!
//! \brief The Multithreaded solution step
//!
class Sol_Step_Mult{
    //!
    //! \brief Linear Function
    //!
    LinFunc* lf;
    //!
    //! \brief Constraint
    //!
    Constr* con;
    //!
    //! \brief basis
    //!
    std::vector<int> basis;
    //!
    //! \brief delta
    //!
    std::vector<double> delta;
    
    std::vector<double> theta;
    
    int min_delta;//індекс стовпця, для якого отримано мінімальну симплекс-різницю, або
    //змінної, яку ми занесемо до базису
    
    int min_theta;//індекс рядка, для якого отримано мінімальне тета, або
    //змінної, яку ми витягнемо із базису
    
    int steptype;//чи розв'язок закінчується на цьому кроці і висновок з розв'язку
public:
    Sol_Step_Mult(LinFunc* lf, Constr* con, int num_thr){
        this->lf = lf;
        this->con = con;
        this->steptype = NOT_A_FINAL_STEP;
        this->basis = compute_basis();
        this->delta = compute_delta(num_thr);
        this->theta = compute_theta(); 
    }
    
    std::vector<int> compute_basis();
    
    std::vector<double> compute_delta(int num_thr);
    
    std::vector<double> compute_theta();
    
//    void subtract_row(int num_thr, int thread_id, Constr* retcon);
    
    Constr* get_next_step_con(int num_thr);
    
    int get_steptype(){
        return steptype;
    }
    
    std::vector<double> get_opt();
};

//!
//! \brief The Solver class
//!
class Solver{
    //Початкові умови:
    LinFunc* start_lf;
    Constr* start_con;
    //Кроки симплекс-методу (можна зробити list)
    bool solved;
    std::vector<Sol_Step> steps;
public:
    Solver(LinFunc* lf, Constr* con){
        this->start_lf = lf;
        this->start_con = con;
        this->solved = false;
    }
    Solution solve();
//    void find_sol_with_basis(int num_threads, int thread_id, LinFunc lf, Constr con, std::vector<std::vector<int>> arrbasis, Solution& solthr);
    Solution multithreadedLPP_basis(int num_thr);
    
    Solution multithreadedLPP_simplex(int num_thr);
};


#endif // SIMPLEX_H
