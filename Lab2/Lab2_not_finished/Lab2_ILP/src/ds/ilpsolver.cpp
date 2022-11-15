#include "./ds/layout.hpp"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

void ilpFloorplan(Layout *L, IloEnv& env, IloModel& model, IloIntVarArray& x, IloIntVarArray& y,  IloIntVarArray& z)
{
    IloArray<IloIntVarArray> xx(env, L->num_blocks), yy(env, L->num_blocks);
    IloIntExprArray x_plus_width(env, L->num_blocks), y_plus_height(env, L->num_blocks);
    IloIntExprArray expr_hpwl;
    for(auto n : L->netlist){
        IloIntExprArray llx, lly, urx, ury;
        for(auto ns : n.net_names){
            int idx;
            if(L->net_name2id.count(ns)){
                idx = L->net_name2id[ns];
                auto width = L->blocklist[idx].borders.width;
                auto height = L->blocklist[idx].borders.height;
                llx.add(IloIntExpr((x[idx] + (z[idx] * height + (1 - z[idx]) * width) / 2).asIntExpr()));
                urx.add(IloIntExpr((x[idx] + (z[idx] * height + (1 - z[idx]) * width) / 2).asIntExpr()));
                lly.add(IloIntExpr((y[idx] + (z[idx] * width + (1 - z[idx]) * height) / 2).asIntExpr()));
                ury.add(IloIntExpr((y[idx] + (z[idx] * width + (1 - z[idx]) * height) / 2).asIntExpr()));
            }
            else{
                idx = L->ter_name2id[ns];
                auto i = L->terlist[idx];
                llx.add(IloIntExpr(env, i.coor.x));
                urx.add(IloIntExpr(env, i.coor.x));
                lly.add(IloIntExpr(env, i.coor.y));
                ury.add(IloIntExpr(env, i.coor.y));
            }
        }
        expr_hpwl.add(IloMax(urx) - IloMin(llx) + IloMax(ury) - IloMin(lly));
    }
    for(int i = 0; i < L->num_blocks; i++){
        x.add(IloIntVar(env, 0, L->width));
        y.add(IloIntVar(env, 0, L->height));
        z.add(IloIntVar(env, 0, 1));
        xx[i] = IloIntVarArray(env, L->num_blocks);
        yy[i] = IloIntVarArray(env, L->num_blocks);
        for(int j = 0; j < L->num_blocks; j++){
            xx[i][j] = IloIntVar(env, 0, 1);
            yy[i][j] = IloIntVar(env, 0, 1);
        }
    }
    int M = std::max(L->width, L->height);
    for(int i = 0; i < L->num_blocks; i++){
        auto i_width = L->blocklist[i].borders.width;
        auto i_height = L->blocklist[i].borders.height;
        x_plus_width[i] = x[i] + z[i] * i_height + (1 - z[i]) * i_width;
        y_plus_height[i] = y[i] + z[i] * i_width + (1 - z[i]) * i_height;
        model.add(x[i] >= 0);
        model.add(y[i] >= 0);
        model.add(x[i] + z[i] * i_height + (1 - z[i]) * i_width <= L->width);
        model.add(y[i] + z[i] * i_width + (1 - z[i]) * i_height <= L->height);
        for(int j = i + 1; j < L->num_blocks; j++){
            auto j_width = L->blocklist[j].borders.width;
            auto j_height = L->blocklist[j].borders.height;
            model.add(x[i] + z[i] * i_height + (1 - z[i]) * i_width <= x[j] + M * (xx[i][j] + yy[i][j]));
            model.add(x[i] - z[j] * j_height - (1 - z[j]) * j_width >= x[j] - M * (1 - xx[i][j] + yy[i][j]));
            model.add(y[i] + z[i] * i_width + (1 - z[i]) * i_height <= y[j] + M * (1 + xx[i][j] - yy[i][j]));
            model.add(x[i] + z[j] * j_width + (1 - z[j]) * j_height >= y[j] - M * (2 - xx[i][j] - yy[i][j]));
        }
    }
    IloIntExprArg cost = L->alpha * (IloMax(x_plus_width) * IloMax(y_plus_height)) +
                            (1 - L->alpha) * IloSum(expr_hpwl);
    model.add(IloMinimize(env, ));
}

// void ilpsolver(Layout *L, IloEnv& env, IloModel& model, IloNumVarArray& x)
// {
//     IloNumVarArray xoffset(env);
    
//     int numOfTerminals = box.numOfTerminal;
//     int boundryL = box.diellX + box.terminalSpace, boundryH = box.dieurX - box.terminalSpace;
//     int terminalSpace = box.terminalSpace, terminalHeight = box.terminalSizeX, terminalWidth = box.terminalSizeX;
//     // int a = 8, b = 15, c = 8, d = 15;
//     for(int i = 1; i <= numOfTerminals; i++)
//     {
//         cout << max(boundryL, box.terBound[i].bbx.lx - 3 * terminalSpace) << " " << min(boundryH, box.terBound[i].bbx.hx + 3 * terminalSpace) << endl;
//         xoffset.add(IloNumVar(env));
//         x.add(IloNumVar(env, max(boundryL, box.terBound[i].bbx.lx - 3 * terminalSpace)
//                             , min(boundryH, box.terBound[i].bbx.hx + 3 * terminalSpace), ILOINT));
//     }

//     for(int i = 1; i <= numOfTerminals; i++)
//     {
//         for(int j = i + 1; j <= numOfTerminals; j++)
//         {
//             model.add(IloAbs(x[i-1] - x[j-1]) >= (terminalSpace + terminalHeight));
//         }
//     }
    
//     for(int i = 1; i <= numOfTerminals; i++)
//     {
//         int lx = box.terBound[i].bbx.lx, hx = box.terBound[i].bbx.hx;
//         model.add(IloIfThen(env, x[i-1] < lx, (xoffset[i-1] == lx - x[i-1])));
//         model.add(IloIfThen(env, x[i-1] > hx, (xoffset[i-1] == x[i-1] - hx)));
//     }
//     model.add(IloMinimize(env, IloSum(xoffset)));
// }

void ilpInit(Layout *L)
{
    IloEnv env;
    try {
        IloModel model(env);
        IloIntVarArray x(env);
        IloIntVarArray y(env);
        IloIntVarArray z(env);
        IloIntArray v(env);
        ilpFloorplan(L, env, model, x, y, z);
        IloCplex cplex(model);
        /* get the log out */
        cplex.setOut(env.getNullStream());  
        cplex.setError(env.getNullStream());
        /* get the log out */
        cplex.solve();
        cout << "Min= " << cplex.getObjValue() << endl;
        cplex.getValues(v, x);
        cout << "Vars " << v << endl;
    }
    catch (IloException& e) {
    cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
    cerr << "Unknown exception caught" << endl;
    }
    env.end();
}