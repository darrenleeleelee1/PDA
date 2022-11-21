#include <algorithm>
#include "ds/layout.hpp"
#include "fm/fm.hpp"
int FM::overlap(Cell* a, Cell* b)
{
    int la_x = a->ll_coor.x, la_y = a->ll_coor.y;
    int lb_x = b->ll_coor.x, lb_y = b->ll_coor.y;
    int ra_x = la_x + a->width, ra_y = la_y + a->height;
    int rb_x = lb_x + b->width, rb_y = lb_y + b->height;

    
    int x_dist = std::min(ra_x, rb_x) - std::max(la_x, lb_x);
    int y_dist = std::min(ra_y, rb_y) - std::max(la_y, lb_y);
    int area_I = 0;
    if( x_dist > 0 && y_dist > 0 ) area_I = x_dist * y_dist;
    
    return area_I;
}
void FM::initGraph()
{
    std::sort(this->L->celllist.begin(), this->L->celllist.end(), [](Cell* a, Cell* b) -> bool {return a->ll_coor.x < b->ll_coor.x;});
    this->adj.resize(this->L->num_of_cell);
    for(int i = 0; i < this->L->num_of_cell; i++){
        for(int j = i + 1; j < this->L->num_of_cell; j++){
            if(this->L->celllist[i]->ll_coor.x + this->L->celllist[i]->width < this->L->celllist[j]->ll_coor.x) break;
            int area_I = FM::overlap(this->L->celllist[i], this->L->celllist[j]);
            if(area_I != 0){
                adj[i].push_back(std::make_pair(j, area_I));
                adj[j].push_back(std::make_pair(i, area_I));
            }
        }
    }
}
void FM::initGain()
{
    int tmp_g;
    bool base, nei;
    for(int i = 0; i < this->L->num_of_cell; i++){
        tmp_g = 0;
        base = this->L->celllist[i]->is_top;
        for(auto j : this->adj[i]){
            int nei_idx = j.first, nei_cost = j.second;
            nei = this->L->celllist[nei_idx]->is_top;
            if(base != nei){ // base and its neighbor is on the different side
                tmp_g += nei_cost;
            }
            else{ // same side
                tmp_g -= nei_cost;
            }
        }
        gain[tmp_g].push_back(i);
        cell2gain[i] = tmp_g;
    }
}
void FM::storePartition()
{
    for(int i = 0 ; i < this->L->num_of_cell; i++){
        this->opt_prti[i] = this->L->celllist[i]->is_top;
    }
}
void FM::popPartition()
{
    for(int i = 0 ; i < this->L->num_of_cell; i++){
        this->L->celllist[i]->is_top = this->opt_prti[i];
    }
}
void FM::partition()
{
    int cnt = this->L->num_of_cell;
    int opt_cost = INT32_MAX, cost = 0;
    while(cnt--){
        for(auto &i : gain){
            if(i.second.size() == 0){
                gain.erase(i.first);
                continue;
            }
            int base_idx = i.second.front(), nei_idx;
            int base_gain = cell2gain[base_idx], nei_gain;
            bool base = this->L->celllist[base_idx]->is_top, nei;
            
            // update neightbor
            for(auto &nei_cell : this->adj[base_idx]){
                nei_idx = nei_cell.first;
                if(locklist.count(nei_idx)) continue;
                int nei_cost = nei_cell.second;
                nei = this->L->celllist[nei_idx]->is_top;
                if(base != nei){ // base and its neighbor is on the different side
                    int ori_gain = cell2gain[nei_idx];
                    gain[ori_gain].remove(nei_idx);
                    ori_gain -= 2 * nei_cost;
                    cell2gain[nei_idx] = ori_gain;
                    gain[ori_gain].push_back(nei_idx);
                }
                else{ // same side
                    int ori_gain = cell2gain[nei_idx];
                    gain[ori_gain].remove(nei_idx);
                    ori_gain += 2 * nei_cost;
                    cell2gain[nei_idx] = ori_gain;
                    gain[ori_gain].push_back(nei_idx);
                }
            }

            // delete base from gain list
            i.second.pop_front();
            this->L->celllist[base_idx]->is_top = !base;
            cost += base_gain;
            locklist.insert(base_idx);

            if(cost < opt_cost){
                opt_cost = cost;
                FM::storePartition(); 
            }
            break;
        }
    }
}