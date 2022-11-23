#pragma once
#include "ds/layout.hpp"
struct Cluster
{
    int n_first, n_last; // mapping to celllist which sorted by x coordinate
    int num_of_cells;
    int x_opt; // optimal position(lower left corner)
    int e, w, q;
    Cluster() {
        this->e = 0;
        this->w = 0;
        this->q = 0;
        this->num_of_cells = 0;
    }
    Cluster(int x, int i){
        Cluster();
        this->n_first = i;
        this->x_opt = x;
        this->num_of_cells++;
    }

};

struct Abacus
{
    Layout *L;
    std::vector<std::vector<Cluster*>> row_clusters;
    std::vector<std::vector<int>> row_cells;
    Abacus(){}
    Abacus(Layout *_L) : L(_L) {
        this->row_clusters.resize(this->L->num_of_row);
        this->row_cells.resize(this->L->num_of_row);
    }
    int findRow(Cell* c); // return this cell should be on which row by the gp result
    void pushRowCell(int idx, int r); // r means add to which row
    void popRowCell(int r); // r means add to which row
    Cluster* rowClusterPredecessor(std::vector<Cluster*> &r_ptr);
    void addCell(Cluster* c, int i);
    void addCluster(Cluster* c_prime, Cluster* c);
    void collapse(std::vector<Cluster*> &r_ptr, Cluster* c);
    int placeRow(int r, std::vector<Cluster*> &r_ptr); // return offset cost
    void storeLegalization(int r);
    void main();
};
