#pragma once
#include "ds/layout.hpp"
struct Cluster
{
    int n_first, n_last; // mapping to celllist which sorted by x coordinate
    int num_of_cells;
    int x_opt; // optimal position(lower left corner)
    int e, w, q;

};

struct Abacus
{
    Layout *L;
    std::vector<std::vector<Cluster*>> trial_clusters;
    std::vector<std::vector<Cluster*>> row_clusters;
    std::vector<std::vector<Cell*>> row_cells;
    Abacus(){}
    Abacus(Layout *_L) : L(_L) {
        this->row_clusters.resize(this->L->num_of_row);
        this->row_cells.resize(this->L->num_of_row);
    }

    int findRow(Cell* c); // return this cell should be on which row by the gp result
    void addRowCell(Cell* c, int r); // r means add to which row
    int placeRow(); // return offset cost
    void main();
};
