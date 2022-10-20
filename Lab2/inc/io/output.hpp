#pragma once

struct Metric
{
    double alpha;
    int area = 0;
    int hpwl = 0;
    int cost;
    Metric(){}
    Metric(double alpha)
    {
        this->alpha = alpha;
    }
    int countCost();
};