#include "lg/lg.hpp"

void Abacus::main()
{
    int c_best, c, r_best;
    int cell_gp_row_idx; // gp result that cell at which row
    
    for(int i = 0; i < this->L->num_of_cell; i++){ // i : Cell
        c_best = INT32_MAX; r_best = -1;
        std::cout << "C" << this->L->celllist[i]->name << " start.\n";
        cell_gp_row_idx = this->findRow(this->L->celllist[i]);
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
        // go up first
        for(int r = cell_gp_row_idx; r < this->L->num_of_row; r++){
            
            std::vector<Cluster*> trial;

            this->pushRowCell(i, r);
            c = this->placeRow(r, trial);
            if(c < c_best){
                c_best = c;
                r_best = r;
            }
            this->popRowCell(r);

            if(c > c_best) break;
        }
        // then go down
        for(int r = cell_gp_row_idx - 1; r >= 0; r--){
            std::vector<Cluster*> trial;

            this->pushRowCell(i, r);
            c = this->placeRow(r, trial);
            if(c < c_best){
                c_best = c;
                r_best = r;
            }
            this->popRowCell(r);

            if(c > c_best) break;
        }
        /*
        TODO:
            Insert Cell i rto row r_best
            PlaceRow r_best(final)
        */
        std::cout << "C" << this->L->celllist[i]->name << " place at " << r_best << std::endl;
        this->pushRowCell(i, r_best);
    }
    for(int i = 0; i < this->L->num_of_row; i++){
        if(row_cells[i].size() > 0) c = this->placeRow(i, this->row_clusters[i]);
    }
    for(int i = 0; i < this->L->num_of_row; i++){
        this->storeLegalization(i);
    }
}