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
    std::srand(time(NULL));
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
int SP::countArea(int &chip_width, int &chip_height){
    chip_width = L[this->nodes-1].x;
    chip_height = L[this->nodes-1].y;
    return chip_width * chip_height;
}
int SP::countHPWL(Layout* layout){
    int totalHPWL = 0;
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
        totalHPWL += ((int)urx - (int)llx + (int)ury - (int)lly);
    }
    return totalHPWL; 
}
int SP::countCost(Layout* layout, double alpha_ratio){
    this->calMatch();
    this->MWCS();
    int area = this->countArea(layout->chip_width, layout->chip_height), hpwl = this->countHPWL(layout), cost;
    cost = int(alpha_ratio * area + (1 - alpha_ratio) * hpwl);
    if(layout->width < layout->chip_width) cost += (layout->chip_width / layout->width) * 10000000;  
    if(layout->height < layout->chip_height) cost += (layout->chip_height / layout->height) * 10000000;  
    return cost;
}

void Layout::initLayout()
{
    this->sp = new SP(this, this->num_blocks);
    this->opt_area = 0;
    this->opt_hpwl = 0;
    this->opt_cost = INT32_MAX;
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
void Layout::SA()
{
    std::srand(unsigned(std::time(0)));
    double T = 40000000, T_frozen = 100, r = 0.85;
    int k = 10000;
    this->sp->randomSP();   
    int old_cost = this->sp->countCost(this, 1), new_cost;
    while(T >= T_frozen){
        int n1, n2, op, action;
        op = std::rand() % 100;
        std::cout << "Temperature: " << T << std::endl;
        for(int _k = 0; _k < k; _k++){
            if(op < 35){ // op1
                action = 1;
                n1 = std::rand() % this->num_blocks;
                this->sp->swapBorders(n1);
            }
            else if(op < 50){ // op2
                n1 = std::rand() % this->num_blocks;
                n2 = std::rand() % this->num_blocks;
                while(n1 == n2) n2 = std::rand() % this->num_blocks;
                
                if(1){
                    action = 2;
                    this->sp->swapBlocks(0, n1, n2);
                }
                else{
                    action = 3;
                    this->sp->swapBlocks(1, n1, n2);
                }
            }
            else{
                action = 4;
                n1 = std::rand() % this->num_blocks;
                n2 = std::rand() % this->num_blocks;
                while(n1 == n2) n2 = std::rand() % this->num_blocks;
                this->sp->swapBlocks(2, n1, n2);
            }
            double accept = (double)std::rand() / (double)RAND_MAX;
            new_cost = this->sp->countCost(this, 1);
            if(new_cost <= old_cost){
                old_cost = new_cost;
            }
            else if(accept < (double)std::exp((double)-(new_cost - old_cost) / (double)T)){
                old_cost = new_cost;
            }
            else{
                // undo
                switch (action)
                {
                case 1:
                    this->sp->swapBorders(n1);
                    break;
                case 2:
                    this->sp->swapBlocks(0, n1, n2);
                    break;
                case 3:
                    this->sp->swapBlocks(1, n1, n2);
                    break;
                case 4:
                    this->sp->swapBlocks(2, n1, n2);
                    break;
                }
            }
            // if(this->opt_cost > new_cost && (this->chip_width <= this->width && this->chip_height <= this->width)){
            if(this->opt_cost > new_cost){
                this->opt_cost = new_cost;
                // std::cout << this->opt_cost << std::endl;
                this->opt_sp = new SP(*(this->sp));
            }
        }
        T = r * T;
    }
}