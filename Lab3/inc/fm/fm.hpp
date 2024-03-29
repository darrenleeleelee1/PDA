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
    double balance_factor;
    Cell* largest_Cell;
    int bal_top_cell, bal_bot_cell;
    // gain, cell index by sorted ordered
    std::map<int, std::list<int>> gain; // store (gain, list of the cell index)
    std::map<int, int> cell2gain; // find the cells gain (cell index, gain)
    // opt
    std::vector<bool> opt_prti;
    std::set<int> locklist;
    FM() {}
    FM(Layout *_L, double bf) : L(_L){ 
        this->opt_prti.resize(_L->num_of_cell);
        this->balance_factor = std::max(bf, 1 - bf);
        this->largest_Cell == nullptr;
        this->bal_top_cell = this->bal_bot_cell = 0;
    }
    int overlap(Cell* a, Cell* b);
    void initGraph();
    void initGain();
    void storePartition();
    void popPartition();
    void partition();
    void main();
};