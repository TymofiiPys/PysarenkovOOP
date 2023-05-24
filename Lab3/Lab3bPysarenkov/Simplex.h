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

struct Solution{
    int type;
    std::vector<double> sol;
};

//лінійна функція
class LinFunc{
    //L = c[0]x[0] + c[1]x[1] + ... + c[n]x[n] -> opt
    std::vector<double> c;
    int n;      //кількість змінних
    bool opt;   //true - max, false - opt
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
    void Standartify(int nNew);//Переведення лін. функції у таку, що відповідає СЗЛП, тобто має прямувати в мінімум
    void Canonify(int nNew);//Переведення лін. функції у таку, що відповідає КЗЛП, тобто містить в доданках M(y1+...+yk),
    //де yi - введені штучні змінні
    bool operator==(const LinFunc& r){
        if(c != r.c || n != r.n || opt != r.opt)
            return false;
        return true;
    }
    friend class Sol_Step;
    friend class Solver;
};

//Лінійні обмеження (непрямі)
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
    friend class Sol_Step;
    friend class Solver;
};

//Крок симплекс-методу
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
        this->basis = compute_basis();
        this->delta = compute_delta();
        this->steptype = NOT_A_FINAL_STEP;
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

//Клас-розв'язувач задачі лінійного програмування
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
};

void LinFunc::Standartify(int nNew){
    c.resize(nNew);
    if(this->opt)//якщо opt = MAX
    {
        for(int i = 0; i < nNew; i++){
            c[i] = c[i] * -1;
        }
        opt = MIN;
    }
    n = nNew;
}

void LinFunc::Canonify(int nNew){
    c.resize(nNew);
    for(int i = n; i < nNew; i++){
        c[i] = M;
    }
    n = nNew;
}

void Constr::Standartify(){
    int nequal = 0; //Кількість нерівностей
    
    for(int i = 0; i < m; i++){
        if(sign[i])
            nequal++;
    }
    if(nequal) //Якщо усі m лін. обмежень є рівностями, то ми вже й так маємо СЗЛП
    {
        n = n + nequal; //додаємо nequal балансних змінних

        for(int i = 0; i < m; ++i){
            a[i].resize(n);
        }
        int k = 0; //індекс нерівності
        for(int i = 0; i < m; i++){
            if(sign[i])//якщо i-те л. о. є нерівністю
            {
                for(int j = n - nequal; j < n; j++){
                    //коефіцієнти для балансних змінних
                    if(j == n - nequal + k)
                        a[i][j] = sign[i] * (-1); //л. о. є нерівністю виду <= - маємо ДОДАТИ балансну змінну з коеф. 1,
                    //а якщо >= - ВІДНЯТИ із тим же коеф.
                }
                sign[i] = 0;
                k++;
            }
        }
    }
}

//Повертає вектор, в якому записано, у яких із m рядків потрібно ввести штучну змінну,
//тобто, якщо не вдалось виділити у масиві a перший стовпець одиничної матриці,
//то у векторі буде 0, якщо третій стовпець - у векторі буде 2 і т.д.
std::vector<int> noid(std::vector<int> ident){
    std::vector<int> r;
    for(size_t i = 0; i < ident.size(); ++i){
        if(ident[i] == -1)
            r.push_back(i);
    }
    return r;
}

std::vector<int> findident(std::vector<std::vector<double>> a){
    int m = a.size();
    int n = a[0].size();
    std::vector<int> ident(m, -1);//Масив, у якому записується, який стовпець матриці a містить i-й стовпець одиничної матриці
//    int c = 0; //скільки стовпців одиничної матриці виявлено
    for(int j = 0; j < n; j++)
    {
        bool isidentcol = false; //чи є стовпець a стовпцем од. матриці
        int k = 0;//на якій позиції у стовпці стоїть 1
        for(int i = 0; i < m; i++){
            if(a[i][j] == 1)
            {
                if(!isidentcol)
                {
                    isidentcol = true;
                    k = i;
                }
                else//більше, ніж одна одиниця у стовпчику
                {
                    isidentcol = false;
                    break;
                }
            }
            else if(a[i][j] != 0)//не 1 і не 0
            {
                isidentcol = false;
                break;
            }
        }
        if(isidentcol)
        {
            if(ident[k] == -1)
            {
                ident[k] = j;
//                c++;
            }
        }
    }
    return ident;
}

void Constr::Canonify(){
    //всі елементи b мають бути невід'ємними!
    for(int i = 0; i < m; i++){
        if(b[i] < 0){
            for(int j = 0; j < n; j++){
                a[i][j] *= -1;
            }
//            sign[i] *= -1; //й так із СЗЛП переводимо, там же рівності
            b[i] *= -1;
        }
    }
//    std::vector<int> ident(m, -1);//Масив, у якому записується, який стовпець матриці a містить i-й стовпець одиничної матриці
//    int c = 0; //скільки стовпців одиничної матриці виявлено
//    for(int j = 0; j < n; j++)
//    {
//        bool isidentcol = false; //чи є стовпець a стовпцем од. матриці
//        int k = 0;//на якій позиції у стовпці стоїть 1
//        for(int i = 0; i < m; i++){
//            if(a[i][j] == 1)
//            {
//                if(!isidentcol)
//                {
//                    isidentcol = true;
//                    k = i;
//                }
//                else//більше, ніж одна одиниця у стовпчику
//                {
//                    isidentcol = false;
//                    break;
//                }
//            }
//            else if(a[i][j] != 0)//не 1 і не 0
//            {
//                isidentcol = false;
//                break;
//            }
//        }
//        if(isidentcol)
//        {
//            if(ident[k] == -1)
//            {
//                ident[k] = j;
//                c++;
//            }
//        }
//    }
    std::vector<int> ident = findident(a);
    int c = 0;
    for(auto i : ident){
        if(i != - 1)
            c++;
    }
    if(c != m){
        //Якщо не вдалось знайти одиничну підматрицю, вводимо штучні змінні.
        n = n + m - c;
        std::vector<int> noident = noid(ident);
        for(int i = 0; i < m; ++i){
            a[i].resize(n);
        }
        int k = 0;
        for(int j = n + c - m; j < n; j++){
            for(int i = 0; i < m; i++){
                if(i == noident[k])//Якщо потрібно ввести у i-й рядок штучну змінну, вводимо її
                {
                    a[i][j] = 1;
                    break;
                }
            }
        }
    }
}

std::vector<int> Sol_Step::compute_basis(){
    std::vector<int> ident = findident(con->a);
    return ident;
}

std::vector<double> Sol_Step::compute_delta(){
    std::vector<double> deltas;
    for(int j = 0; j < con->n; j++){
        double delta = lf->c[j];
        for(int i = 0; i < con->m; i++){
            delta -= con->a[i][j]*lf->c[basis[i]];
        }
        deltas.push_back(delta);
    }
    double min = INT_MAX;
    int k = -1;
    for(int i = 0; i < con->n; i++){
        if(deltas[i] < min && deltas[i] < 0)
        {
            k = i;
            min = deltas[i];
        }
    }
    min_delta = k;
    if(min_delta == -1)
        steptype = FOUND_OPT;
    return deltas;
}

std::vector<double> Sol_Step::compute_theta(){
    std::vector<double> aj;
    for(int i = 0; i < con->m; i++){
        aj.push_back(con->a[i][min_delta]);
    }
    std::vector<double> thetas(con->m, -1);
    
    for(int i = 0; i < con->m; i++){
        if(con->b[i]/aj[i] > 0){
            thetas[i] = con->b[i]/aj[i];
        }
    }
    double min = INT_MAX;
    int k = -1;
    for(int i = 0; i < con->m; i++){
        if(thetas[i] < min && thetas[i] > 0)
        {
            k = i;
            min = thetas[i];
        }
    }
    min_theta = k;
    if(min_theta == -1)
        steptype = LIN_FUNC_UNLIMITED;
    return thetas;
}

Constr* Sol_Step::get_next_step_con(){
    Constr* retcon = new Constr(con->a, con->sign, con->b, con->m, con->n);
    int i = min_theta;
    int divby = retcon->a[min_delta][min_theta];
    for(int j = 0; j < retcon->n; j++){
        retcon->a[i][j] /= divby;
    }
    retcon->b[i] /= divby;
    for(i = 0; i < retcon->m; i++){
        if(i == min_theta)
            continue;
        int mult_by = retcon->a[i][min_delta];
        for(int j = 0; j < retcon->n; j++){
            retcon->a[i][j] -= retcon->a[min_theta][j]*mult_by;
        }
        retcon->b[i] -= retcon->b[min_theta]*mult_by;
    }
    return retcon;
}

std::vector<double> Sol_Step::get_opt()
{
    std::vector<double> opt(con->n);
    for(int i = 0; i < con->m; i++){
        opt[basis[i]] = con->b[i];
    }
    return opt;
}

Solution Solver::solve(){
    //Переведення задачі у СЗЛП
    start_con->Standartify();
    start_lf->Standartify(start_con->get_n());
    //Переведення СЗЛП у КЗЛП
    start_con->Canonify();
    Sol_Step* step = new Sol_Step(start_lf, start_con);
    steps.push_back(*step);
    if(step->get_steptype() != NOT_A_FINAL_STEP)
        solved = true;
    while(!solved){
        Constr* step_con = step->get_next_step_con();
        step = new Sol_Step(start_lf, step_con);
        steps.push_back(*step);
        if(step->get_steptype() != NOT_A_FINAL_STEP)
            solved = true;
    }
    std::vector<double> sol;
    int type = step->get_steptype();
    if(type == LIN_FUNC_UNLIMITED)
        sol = std::vector<double>(start_con->n, -1);
    else
        sol = {0};
    delete step;
    return {type, sol};
}
#endif // SIMPLEX_H
