#ifndef SIMPLEXMULTI2_H
#define SIMPLEXMULTI2_H
#include "Simplex.h"

Solution Solver::multithreadedLPP_simplex(int num_thr){
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



#endif // SIMPLEXMULTI2_H
