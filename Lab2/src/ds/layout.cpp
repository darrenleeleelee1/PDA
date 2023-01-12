#include "./ds/layout.hpp"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>
void SP::swapBlocks(int seq, int x, int y)
{
    switch (seq)
    {
    case 0:
        std::swap(this->pos_seq[x], this->pos_seq[y]);
        std::swap(this->match[x].x, this->match[x].y);
        break;
    case 1:
        std::swap(this->neg_seq[x], this->neg_seq[y]);
        break;
    case 2:
        std::swap(this->pos_seq[x], this->pos_seq[y]);
        std::swap(this->neg_seq[x], this->neg_seq[y]);
        break;
    }
}
void SP::swapBorders(int x)
{
    std::swap(this->blocklist[x].borders.width, this->blocklist[x].borders.height);
}
void SP::calMatch()
{
    for(int i = 0; i < this->nodes; i++){
        match[this->pos_seq[i]].x = i;
        match[this->neg_seq[i]].y = i;
    }
}
void SP::randomSP()
{
    std::srand(std::time(NULL));
    std::random_shuffle(this->pos_seq.begin(), this->pos_seq.end());
    std::random_shuffle(this->neg_seq.begin(), this->neg_seq.end());
    this->calMatch();
}
void SP::posMWCS()
{
    for(int i = 0; i < this->nodes; i++){
        int b = this->pos_seq[i];
        int p = this->match[b].y;
        this->P[b].x = this->L[p].x;
        int t = this->P[b].x + this->blocklist[b].borders.width;
        for(int j = p; j < this->nodes; j++){
            this->L[j].x = std::max(this->L[j].x, t);
        }
    }
}
void SP::negMWCS()
{

    for(int i = 0; i < this->nodes; i++){
        int b = this->pos_seq[this->nodes-i-1]; // get reverse order of positive sequence
        int p = this->match[b].y;
        this->P[b].y = this->L[p].y;
        int t = this->P[b].y + this->blocklist[b].borders.height;
        for(int j = p; j < this->nodes; j++){
            this->L[j].y = std::max(this->L[j].y, t);
        }
    }
    
}
void SP::MWCS()
{
    for(auto &i : this->L){
        i.x = 0;
        i.y = 0;
    }
    for(auto &i : this->P){
        i.x = 0;
        i.y = 0;
    }
    this->posMWCS();
    this->negMWCS();
}
int64_t SP::countArea(int &chip_width, int &chip_height){
    chip_width = L[this->nodes-1].x;
    chip_height = L[this->nodes-1].y;
    return chip_width * chip_height;
}
int64_t SP::countHPWL(Layout* layout){
    int64_t totalHPWL = 0;
    for(auto n : layout->netlist){
        int llx = INT32_MAX, lly = INT32_MAX, urx = INT32_MIN, ury = INT32_MIN;
        for(auto ns : n.net_names){
            int idx;
            if(layout->net_name2id.count(ns)){
                idx = layout->net_name2id[ns];
                auto i = this->P[idx];
                auto j = this->blocklist[idx].borders;
                llx = std::min(llx, i.x + j.width / 2);
                lly = std::min(lly, i.y + j.height / 2);
                urx = std::max(urx, i.x + j.width / 2);
                ury = std::max(ury, i.y + j.height / 2);
            }
            else{
                idx = layout->ter_name2id[ns];
                auto i = layout->terlist[idx];
                llx = std::min(llx, i.coor.x);
                lly = std::min(lly, i.coor.y);
                urx = std::max(urx, i.coor.x);
                ury = std::max(ury, i.coor.y);
            }
        }
        totalHPWL += ((int64_t)urx - (int64_t)llx + (int64_t)ury - (int64_t)lly);
    }
    return totalHPWL; 
}
int64_t SP::countCost(Layout* layout, double alpha_ratio, bool add_penalty = true){
    this->calMatch();
    this->MWCS();
    int64_t area = this->countArea(layout->chip_width, layout->chip_height), hpwl = this->countHPWL(layout), cost;
    cost = int64_t(alpha_ratio * area + (1 - alpha_ratio) * hpwl);
    double penalty = (double)(layout->chip_width * layout->height) / (double)(layout->chip_height * layout->width);
    if (penalty < 1) {
        penalty = 1 / penalty;
    }
    if (layout->chip_width > layout->width) {
        penalty *= 1.10;
    }
    if (layout->chip_height > layout->height) {
        penalty *= 1.10;
    }
    if (layout->chip_width <= layout->height && layout->chip_height <= layout->width) { // fit height and width
        penalty *= 0.8;
    }
    if(add_penalty) cost = cost * penalty;
    return cost;
}

void Layout::initLayout()
{
    this->sp = new SP(this, this->num_blocks);
    this->opt_area = 0;
    this->opt_hpwl = 0;
    this->opt_cost = INT64_MAX;
}
void Layout::addBlock(std::string s, int w, int h)
{
    this->net_name2id[s] = this->blocklist.size();
    this->blocklist.emplace_back(Block(s, w, h));
}
void Layout::addTer(std::string s, int a, int b)
{
    ter_name2id[s] = terlist.size();
    terlist.emplace_back(Terminal(s, a, b));
}
void Layout::SA(double T, double T_frozen, double r, int k, int num_look_nei, double alpha_ratio)
{
      
    int64_t old_cost = this->sp->countCost(this, alpha_ratio), new_cost, nei_cost;
    while(T >= T_frozen){
        int n1, n2, op, action, action_rot = -1, nei_n1, nei_n2, nei_rot, nei_action;
        op = std::rand() % 100;
        std::cout << "Temperature: " << T << std::endl;
        for(int _k = 0; _k < k; _k++){
            new_cost = INT64_MAX;
            for(int i = 0; i < num_look_nei; i++){
                action_rot = -1;
                if(op < 35){ // op1
                    n1 = std::rand() % this->num_blocks;
                    action_rot = n1;
                    this->sp->swapBorders(n1);
                }
                if(op < 50){ // op2
                    n1 = std::rand() % this->num_blocks;
                    n2 = std::rand() % this->num_blocks;
                    while(n1 == n2) n2 = std::rand() % this->num_blocks;
                    
                    if(std::rand() & 1){
                        action = 1;
                        this->sp->swapBlocks(0, n1, n2);
                    }
                    else{
                        action = 2;
                        this->sp->swapBlocks(1, n1, n2);
                    }
                }
                else{
                    action = 3;
                    n1 = std::rand() % this->num_blocks;
                    n2 = std::rand() % this->num_blocks;
                    while(n1 == n2) n2 = std::rand() % this->num_blocks;
                    this->sp->swapBlocks(2, n1, n2);
                }
                nei_cost = this->sp->countCost(this, alpha_ratio);
                if(nei_cost < new_cost){
                    new_cost = nei_cost;
                    nei_rot = action_rot;
                    nei_n1 = n1;
                    nei_n2 = n2;
                    nei_action = action;
                }
                
                // undo
                if(~action_rot){
                    this->sp->swapBorders(action_rot);
                }
                switch (action){
                case 1:
                    this->sp->swapBlocks(0, n1, n2);
                    break;
                case 2:
                    this->sp->swapBlocks(1, n1, n2);
                    break;
                case 3:
                    this->sp->swapBlocks(2, n1, n2);
                    break;
                }
            }
            
            if(~nei_rot){
                this->sp->swapBorders(nei_rot);
            }
            switch (nei_action){
            case 1:
                this->sp->swapBlocks(0, nei_n1, nei_n2);
                break;
            case 2:
                this->sp->swapBlocks(1, nei_n1, nei_n2);
                break;
            case 3:
                this->sp->swapBlocks(2, nei_n1, nei_n2);
                break;
            }

            double accept = (double)std::rand() / (double)RAND_MAX;
            if(new_cost < old_cost){
                old_cost = new_cost;
            }
            else if(accept < (double)std::exp((double)-(new_cost) / (double)T)){
                old_cost = new_cost;
            }
            else{
                // undo
                if(~nei_rot){
                    this->sp->swapBorders(nei_rot);
                }
                switch (nei_action){
                case 1:
                    this->sp->swapBlocks(0, nei_n1, nei_n2);
                    break;
                case 2:
                    this->sp->swapBlocks(1, nei_n1, nei_n2);
                    break;
                case 3:
                    this->sp->swapBlocks(2, nei_n1, nei_n2);
                    break;
                }
            }
            if(this->opt_cost > new_cost && (this->chip_width <= this->width && this->chip_height <= this->height)){
                this->opt_cost = new_cost;
                std::cout << "Find better cost = " << new_cost << " in fixed outline.\n";
                this->opt_sp = new SP(*(this->sp));
            }
        }
        T = r * T;
    }
}