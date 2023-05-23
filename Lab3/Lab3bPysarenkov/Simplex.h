#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <vector>
#include <iostream>
enum Opt{
    MIN, MAX
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
    void Standartify();//Переведення лін. функції у таку, що відповідає СЗЛП, тобто має прямувати в мінімум
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
    LinFunc* lf;
    Constr* con;
    std::vector<int> basis;
    std::vector<double> delta;
    std::vector<double> theta;
};

//Клас-розв'язувач задачі лінійного програмування
class Solver{
    //Початкові умови:
    LinFunc* start_lf;
    Constr* start_con;
    //Кроки симплекс-методу (можна зробити list)
    std::vector<Sol_Step> steps;
public:
    Solver(LinFunc* lf, Constr* con){
        this->start_lf = lf;
        this->start_con = con;
    }
    void solve();
};

void LinFunc::Standartify(){
    c.resize(n);
    if(this->opt)//якщо opt = MAX
    {
        for(int i = 0; i < n; i++){
            c[i] = c[i] * -1;
        }
        opt = MIN;
    }
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
    std::vector<int> ident(m, -1);//Масив, у якому записується, який стовпець матриці a містить i-й стовпець одиничної матриці
    int c = 0; //скільки стовпців одиничної матриці виявлено
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
                c++;
            }
        }
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

void Solver::solve(){
    //Переведення задачі у СЗЛП
    start_lf->Standartify();
    start_con->Standartify();
    start_con->Canonify();
}
#endif // SIMPLEX_H
