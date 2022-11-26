#include "lg/lg.hpp"

void Abacus::main()
{
    this->splitRow();
    int c_best, c, r_best, sr_best;
    int cell_gp_row_idx; // gp result that cell at which row
    
    for(int i = 0; i < this->L->num_of_cell; i++){ // i : Cell
        c_best = INT32_MAX; r_best = -1; sr_best = -1;
        cell_gp_row_idx = this->findRow(this->L->celllist[i]);
        std::cout << "C" << this->L->celllist[i]->name << " start!\n";
        // go up first
        for(int r = cell_gp_row_idx; r < this->L->num_of_row; r++){
            for(int sr = 0; sr < this->num_of_sub_row[r]; sr++){
                std::vector<Cluster*> trial;

                this->pushRowCell(i, r, sr);
                c = this->placeRow(r, sr, trial);
                if(c < c_best){
                    c_best = c;
                    r_best = r;
                    sr_best = sr;
                }
                this->popRowCell(r, sr);

            }
            // if(c > c_best) break;
        }
        // then go down
        for(int r = cell_gp_row_idx - 1; r >= 0; r--){
            for(int sr = 0; sr < this->num_of_sub_row[r]; sr++){
                std::vector<Cluster*> trial;

                this->pushRowCell(i, r, sr);
                c = this->placeRow(r, sr, trial);
                if(c < c_best){
                    c_best = c;
                    r_best = r;
                    sr_best = sr;
                }
                this->popRowCell(r, sr);

            }
            // if(c > c_best) break;
        }
        
        std::cout << "C" << this->L->celllist[i]->name << " place at row " << r_best << " sub_row " << sr_best << std::endl;
        
        this->pushRowCell(i, r_best, sr_best);
    }

    for(int i = 0; i < this->L->num_of_row; i++){
        for(int j = 0; j < this->row_cells[i].size(); j++){
            if(this->row_cells[i][j].size() > 0) c = this->placeRow(i, j, this->row_clusters[i][j]);
        }
    }
    for(int i = 0; i < this->L->num_of_row; i++){
        for(int j = 0; j < this->row_cells[i].size(); j++){
            this->storeLegalization(i, j);
        }
    }
}