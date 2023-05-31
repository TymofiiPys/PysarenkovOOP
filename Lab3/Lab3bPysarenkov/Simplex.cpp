#include "Simplex.h"
#include <thread>

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
    if(steptype == FOUND_OPT){
        min_theta = -2;
        return std::vector<double> (con->m, -1);
    }
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
    double divby = retcon->a[min_theta][min_delta];
    for(int j = 0; j < retcon->n; j++){
        retcon->a[i][j] /= divby;
    }
    retcon->b[i] /= divby;
    for(i = 0; i < retcon->m; i++){
        if(i == min_theta)
            continue;
        double mult_by = retcon->a[i][min_delta];
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

double mult(std::vector<double> l, std::vector<double> r){
    double s = 0;
    for(size_t i = 0; i < l.size(); i++){
        s += l[i]*r[i];
    }
    return s;
}

Solution Solver::solve(){
    //Переведення задачі у СЗЛП
    start_con->Standartify();
    start_lf->Standartify(start_con->get_n());
    //Переведення СЗЛП у КЗЛП
    start_con->Canonify();
    start_lf->Canonify(start_con->get_n());
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
    double lfvalue = 0;
    int type = step->get_steptype();
    if(type == LIN_FUNC_UNLIMITED)
    {
        sol = std::vector<double>(start_con->n, -1);
        lfvalue = start_lf->opt ? INT_MAX : INT_MAX * -1;
    }
    else
    {
        sol = step->get_opt();
        lfvalue = mult(start_lf->c, sol);
    }
    delete step;
    return {type, sol, lfvalue};
}




std::vector<int> findident_mult(std::vector<std::vector<double>> a){
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

std::vector<int> Sol_Step_Mult::compute_basis(){
    std::vector<int> ident = findident_mult(con->a);
    return ident;
}

void delta_i(){
    
}

std::vector<double> Sol_Step_Mult::compute_delta(int num_thr){
    std::vector<double> deltas;
//    std::vector<std::thread> threads; 
//    for(int i = 0; i < num_thr; i++){
//        threads.push_back(std::thread(delta_i, num_thr, i, con->a, con->b, min_delta));
//    }
//    for(int i = 0; i < num_thr; i++){
//        threads[i].join();
//    }
    for(int j = /*thread_id*/0; j < con->n; j+=num_thr){
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

std::vector<double> Sol_Step_Mult::compute_theta(){
    if(steptype == FOUND_OPT){
        min_theta = -2;
        return std::vector<double> (con->m, -1);
    }
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

void subtract_row(int num_thr, int thread_id, std::vector<std::vector<double>> a, std::vector<double> b, int min_theta, int min_delta){
    int m = b.size();
    int n = a[0].size();
    for(int i = thread_id; i < m; i+= num_thr){
        if(i == min_theta)
            continue;
        double mult_by = a[i][min_delta];
        for(int j = 0; j < n; j++){
            a[i][j] -= a[min_theta][j]*mult_by;
        }
        b[i] -= b[min_theta]*mult_by;
    }
}

Constr* Sol_Step_Mult::get_next_step_con(int num_thr){
    Constr* retcon = new Constr(con->a, con->sign, con->b, con->m, con->n);
    int i = min_theta;
    double divby = retcon->a[min_theta][min_delta];
    for(int j = 0; j < retcon->n; j++){
        retcon->a[i][j] /= divby;
    }
    retcon->b[i] /= divby;
    std::vector<std::thread> threads; 
    for(int i = 0; i < num_thr; i++){
        threads.push_back(std::thread(subtract_row, num_thr, i, retcon->a, retcon->b, min_theta, min_delta));
    }
    for(int i = 0; i < num_thr; i++){
        threads[i].join();
    }
//    for(i = 0; i < retcon->m; i++){
//        if(i == min_theta)
//            continue;
//        double mult_by = retcon->a[i][min_delta];
//        for(int j = 0; j < retcon->n; j++){
//            retcon->a[i][j] -= retcon->a[min_theta][j]*mult_by;
//        }
//        retcon->b[i] -= retcon->b[min_theta]*mult_by;
//    }
    return retcon;
}

std::vector<double> Sol_Step_Mult::get_opt()
{
    std::vector<double> opt(con->n);
    for(int i = 0; i < con->m; i++){
        opt[basis[i]] = con->b[i];
    }
    return opt;
}
