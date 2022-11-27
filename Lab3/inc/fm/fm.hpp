#pragma once
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <list>
#include "ds/layout.hpp"
class FM{
public:
    // adjcent graph, adj[x] = (y, w) means x --- y, weight = w
    std::vector<std::vector<std::pair<int, int>>> adj;
    Layout *L;
    // balance factor
    double balance_factor = 0.5;
    Cell* largest_Cell;
    int num_top_cell, num_bot_cell;
    // gain, cell index by sorted ordered
    std::map<int, std::list<int>> gain; // store (gain, list of the cell index)
    std::map<int, int> cell2gain; // find the cells gain (cell index, gain)
    // opt
    std::vector<bool> opt_prti;
    std::set<int> locklist;
    FM() {}
    FM(Layout *_L) : L(_L) { 
        opt_prti.resize(_L->num_of_cell);
        largest_Cell == nullptr;
    }
    int overlap(Cell* a, Cell* b);
    void initGraph();
    void initGain();
    void storePartition();
    void popPartition();
    void partition();
    void main();
};