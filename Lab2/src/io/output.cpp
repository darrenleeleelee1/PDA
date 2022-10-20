#include "./io/output.hpp"
#include <iostream>

int Metric::countCost()
{
    if(area == 0 || hpwl == 0){
        if(area == 0) std::cout << "Error: area = 0" << std::endl;
        else std::cout << "Error: hpwl = 0" << std::endl;
        return -1;
    }
    cost = alpha * area + (1 - alpha) * hpwl;
    return cost;
}
