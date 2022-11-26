#pragma once
#include "ds/layout.hpp"
struct Subrow
{
    int x_left_bry, x_right_bry; // x coordinate left and right boundry 
    Subrow() {}
    Subrow(int l, int r) : x_left_bry(l), x_right_bry(r) {}
};
struct Cluster
{
    int n_first, n_last; // mapping to celllist which sorted by x coordinate
    int x_opt; // optimal position(lower left corner)
    int e, w, q;
    Cluster() {
        this->e = 0;
        this->w = 0;
        this->q = 0;
    }
    Cluster(int x, int i){
        this->e = 0;
        this->w = 0;
        this->q = 0;
        this->n_first = i;
        this->x_opt = x;
    }

};

struct Abacus
{
    Layout *L;
    std::vector<std::vector<std::vector<Cluster*>>> row_clusters; // row_clusters[main row][sub_row]
    std::vector<std::vector<std::vector<int>>> row_cells; // row_cells[main row][sub_row]
    std::vector<std::vector<Subrow*>> subrow_drc; // sub row constraint, subrow_drc[main row][sub_row]
    std::vector<int> num_of_sub_row; // num of sub row at i, sub_row[i]
    Abacus(){}
    Abacus(Layout *_L) : L(_L) {
        this->row_clusters.resize(this->L->num_of_row);
        this->row_cells.resize(this->L->num_of_row);
        this->subrow_drc.resize(this->L->num_of_row);
        this->num_of_sub_row.resize(this->L->num_of_row, 1);
    }
    void splitRow();
    int findRow(Cell* c); // return this cell should be on which row by the gp result
    void pushRowCell(int idx, int r, int sr); // r means add to which row
    void popRowCell(int r, int sr); // r means add to which row
    Cluster* rowClusterPredecessor(std::vector<Cluster*> &r_ptr);
    void addCell(Cluster* c, int i);
    void addCluster(Cluster* c_prime, Cluster* c);
    void collapse(std::vector<Cluster*> &r_ptr, Cluster* c, int left_bry, int right_bry);
    int placeRow(int r, int sr, std::vector<Cluster*> &r_ptr); // return offset cost
    void storeLegalization(int r, int sr);
    void main();
};
