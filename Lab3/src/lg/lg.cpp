#include "lg/lg.hpp"
#include <cmath>

int Abacus::findRow(Cell* c)
{
    auto gp_cell = this->L->gp_result[c->name];
    int gp_x = gp_cell->ll_coor.x;
    return std::round(gp_x / this->L->row_hei);
}

void Abacus::addRowCell(Cell* c, int r)
{
    this->row_cells
}

int Abacus::placeRow()
{
    // TODO
}