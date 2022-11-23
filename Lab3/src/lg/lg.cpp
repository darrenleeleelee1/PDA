#include "lg/lg.hpp"
#include <cmath>
#include <algorithm>
int Abacus::findRow(Cell* c)
{
    int gp_y = c->ll_coor.y;
    // in case some out of row
    return std::min((int)std::round(gp_y / this->L->row_hei), this->L->num_of_row-1);
}

void Abacus::pushRowCell(int idx, int r)
{
    this->row_cells[r].push_back(idx);
}

void Abacus::popRowCell(int r)
{
    this->row_cells[r].pop_back();
}

void Abacus::addCell(Cluster* c, int i)
{
    c->n_last = i;
    c->e = c->e + 1;
    c->q = c->q + (this->L->gp_result[i]->ll_coor.x - c->w); // q = q + e(x'(i) - w)
    c->w = c->w + this->L->gp_result[i]->width;
}

void Abacus::addCluster(Cluster* c_prime, Cluster* c)
{
    c_prime->n_last = c->n_last;
    c_prime->e = c_prime->e + c->e;
    c_prime->q = c_prime->q + c->q - c_prime->w;
    c_prime->w = c_prime->w + c->w;
}
Cluster* Abacus::rowClusterPredecessor(std::vector<Cluster*> &r_ptr)
{
    if(r_ptr.size() > 1) return r_ptr[r_ptr.size()-2];
    else return nullptr;
}
void Abacus::collapse(std::vector<Cluster*> &r_ptr, Cluster* c)
{
    c->x_opt = c->q / c->e;
    if(c->x_opt < 0) c->x_opt = 0;
    if(c->x_opt > this->L->width - c->w) c->x_opt = this->L->width - c->w;
    
    Cluster* c_prime = rowClusterPredecessor(r_ptr);
    if(c_prime != nullptr && c_prime->x_opt + c_prime->w > c->x_opt){
        this->addCluster(c_prime, c);
        r_ptr.pop_back();
        this->collapse(r_ptr, c_prime);
    }
}
// TODO: 先check 全部在同一層的時候ligalization結果如何 忘記分上下層去做legalization
void Abacus::storeLegalization(int r)
{
    int tmp_x;
    int i = -1;
    for(auto c : this->row_clusters[r]){
        tmp_x = c->x_opt;
        do{
            i++;
            this->L->celllist[this->row_cells[r][i]]->ll_coor.x = tmp_x;
            this->L->celllist[this->row_cells[r][i]]->ll_coor.y = r * this->L->row_hei;
            tmp_x += this->L->celllist[this->row_cells[r][i]]->width;
        }while(this->row_cells[r][i] != c->n_last);
    }
}

int Abacus::placeRow(int r, std::vector<Cluster*> &r_ptr)
{
    Cluster* c = nullptr;
    for(auto &i : this->row_cells[r]){
        if(r_ptr.size() > 0) c = r_ptr.back(); // last cluster
        if(r_ptr.size() == 0 || c->x_opt + c->w <= this->L->gp_result[i]->ll_coor.x){
            r_ptr.push_back(new Cluster(this->L->gp_result[i]->ll_coor.x, i));
            c = r_ptr.back();
            this->addCell(c, i);
        }
        else{
            this->addCell(c, i);
            this->collapse(r_ptr, c);
        }
        c = r_ptr.back();
        if(c->x_opt < 0) return INT32_MAX;
    }
    int cell_idx = this->row_cells[r].back();
    int x_offset = std::abs(c->x_opt + c->w - this->L->gp_result[cell_idx]->ll_coor.x);
    int y_offset = std::abs(r * this->L->row_hei - this->L->gp_result[cell_idx]->ll_coor.y);
    return x_offset + y_offset;
}