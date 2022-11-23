#include "lg/lg.hpp"

void Abacus::main()
{
    int c_best, c, r_best;
    int cell_gp_row_idx; // gp result that cell at which row
    
    for(int i = 0; i < this->L->num_of_cell; i++){ // i : Cell
        c_best = INT32_MAX; r_best = -1;
        if(this->L->celllist[i]->name == 8) {
            c_best = INT32_MAX;
        }
        cell_gp_row_idx = this->findRow(this->L->celllist[i]);
        std::cout << "C" << this->L->celllist[i]->name << " start!\n";
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
        
        std::cout << "C" << this->L->celllist[i]->name << " place at row " << r_best << std::endl;
        this->pushRowCell(i, r_best);
    }
    for(int i = 0; i < this->L->num_of_row; i++){
        if(row_cells[i].size() > 0) c = this->placeRow(i, this->row_clusters[i]);
    }
    for(int i = 0; i < this->L->num_of_row; i++){
        this->storeLegalization(i);
    }
}