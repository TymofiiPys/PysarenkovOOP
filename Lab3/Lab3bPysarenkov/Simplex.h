#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <vector>

enum Opt{
    MIN, MAX
};

//лінійна функція
class LinFunc{
    //L = c[0]x[0] + c[1]x[1] + ... + c[n]x[n] -> opt
    double* c;
    int n;
    bool opt; //true - max, false - opt
public:
    LinFunc(int n, bool opt){
        this->c = new double[n];
        this->n = n;
        this->opt = opt;
    }
    void Standartify();//Переведення лін. функції у таку, що відповідає СЗЛП, тобто має прямувати в мінімум
    friend class Sol_Step;
    friend class Solver;
};

//Лінійні обмеження (непрямі)
class Constr{
    //a[i][0]x[0] + a[i][1]x[1] + ... + a[i][n]x[n] <= (=, >=) b[i], i = [1, m]
    double** a;
    int* sign; //<= -> -1, = ->0, >= -> 1
    double* b;
    int m;
    int n;
public:
    Constr(int m, int n){
        this->a = new double*[m];
        for(int i = 0; i < m; i++)
            a[i] = new double[n];
        this->sign = new int[m];
        this->b = new double[m];
        this->m = m;
        this->n = n;
    }
    Constr(double** a, int* sign, double* b, int m, int n){
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
    bool operator==(const Constr* c){
        if(m != c->m || n != c->n)
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
    int* basis;
    double* delta;
    double* theta;
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
    for(int i = 0; i < n; i++){
        c[i] = c[i] * -1;
    }
    opt = MIN;
}

void Constr::Standartify(){
    int nequal = 0; //Кількість нерівностей
    for(int i = 0; i < m; i++){
        if(sign[i])
            nequal++;
    }
    if(nequal) //Якщо усі m лін. обмежень є рівностями, то ми вже й так маємо СЗЛП
    {
        double** olda = new double*[m]; //Масив зі старими коефіцієнтами a, без балансних змінних
        for(int i = 0; i < m; i++)
            olda[i] = new double[n];
        for(int i = 0; i < m; i++)
            for(int j = 0; j < n; j++){
                olda[i][j] = a[i][j];
            }
        delete a;
        n = n + nequal;
        //Переініціалізація: у масив a вводитимуться коефіцієнти для нових, балансних змінних
        a = new double*[m];
        for(int i = 0; i < m; i++)
            a[i] = new double[n];
        int k = 0; //індекс нерівності
        for(int i = 0; i < m; i++){
            if(sign[i])//якщо i-те л. о. є нерівністю
            {
                for(int j = 0; j < n; j++){
                    if(j < n - nequal)
                    {
                        a[i][j] = olda[i][j]; //переносимо старий масив у новий
                    }
                    else
                    {
                        //коефіцієнти для балансних змінних
                        if(j == n - nequal + k)
                            a[i][j] = sign[i] * (-1); //л. о. є нерівністю виду <= - маємо ДОДАТИ балансну змінну з коеф. 1,
                        //якщо >= - ВІДНЯТИ із тим же коеф.
                        else
                            a[i][j] = 0;
                    }
                }
                sign[i] = 0;
                k++;
            }
        }
        delete[] olda;
    }
}

void Constr::Canonify(){
    //всі елементи b мають бути невід'ємними!
    for(int i = 0; i < m; i++){
        if(b[i] < 0){
            for(int j = 0; j < n; j++){
                a[i][j] *= -1;
            }
            sign[i] *= -1;
            b[i] *= -1;
        }
    }
    int *ident = new int[m];//Масив, у якому записується, який стовпець матриці a містить i-й стовпець одиничної матриці
    for(int i = 0; i < m; i++){
        ident[i] = -1;
    }
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
            ident[k] = j;
            c++;
        }
    }
    if(c != m){
        //Якщо не вдалось знайти одиничну підматрицю, вводимо штучні змінні.
        n = n + m - c;
        for(int j = 0; j < n + m - c; j++)
        {
            for(int i = 0; i < m; i++){
                
            }
        }
    }
}

void Solver::solve(){
    //Переведення задачі у СЗЛП
    if(start_lf->opt)//якщо opt = MAX
        start_lf->Standartify();
    start_con->Standartify();
    start_con->Canonify();
}
#endif // SIMPLEX_H
