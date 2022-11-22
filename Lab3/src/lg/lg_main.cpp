#include "lg/lg.hpp"

void Abacus::main()
{
    int c_best;
    int cell_gp_row_idx; // gp result that cell at which row
    
    for(auto &i : this->L->celllist){ // i : Cell
        c_best = INT32_MAX;
        cell_gp_row_idx = this->findRow(i);

        // go up first
        for(int r = cell_gp_row_idx; r < this->L->num_of_row; r++){
            /* 
            TODO:
                Insert cell i into row r
                PlaceRow r (trial)
                Determine cost c
                    if row last cluster's x_opt + cell i's width > chip width, then c = INT32_MAX
                if c < c_best then c_best = c, r_best = r
                Remove cell i from row r

                if c > c_best break;
            */
        }
        // then go down
        for(int r = cell_gp_row_idx - 1; r >= 0; r--){
            /* 
            TODO:
                Insert cell i into row r
                PlaceRow r (trial)
                Determine cost c
                    if row last cluster's x_opt + cell i's width > chip width, then c = INT32_MAX
                if c < c_best then c_best = c, r_best = r
                Remove cell i from row r
                
                if c > c_best break;
            */
        }
        /*
        TODO:
            Insert Cell i rto row r_best
            PlaceRow r_best(final)
        */
    }
}