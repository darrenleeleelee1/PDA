#include "./ds/layout.hpp"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>
void B_star::binaryTreeInsert(int pa, int src)
{
    if(src < pa){
        if(this->left_child[pa] != -1) binaryTreeInsert(this->left_child[pa], src);
        else{
            this->left_child[pa] = src;
            this->parent[src] = pa; 
        }
    }
    else{
        if(this->right_child[pa] != -1) binaryTreeInsert(this->right_child[pa], src);
        else{
            this->right_child[pa] = src;
            this->parent[src] = pa; 
        }
    }
}
void B_star::initBstar(Layout* L)
{
    parent.resize(this->nodes); fill(parent.begin(), parent.end(), -1);
    left_child.resize(this->nodes); fill(left_child.begin(), left_child.end(), -1);
    right_child.resize(this->nodes); fill(right_child.begin(), right_child.end(), -1);
    ll_coor.resize(this->nodes);
    borders.resize(this->nodes);
    for(int i = 0; i < this->nodes; i++){
        borders[i] = L->blocklist[i].borders;
    }
    // std::srand(unsigned(std::time(0)));
    std::vector<int> rand_tree; rand_tree.resize(nodes);
    
    // set increasing values
    for (int i = 0; i < this->nodes; ++i) rand_tree[i] = i;
    std::random_shuffle(rand_tree.begin(),rand_tree.end());
    int pa = -1;
    this->root_index = rand_tree[0];
    for(auto i : rand_tree){
        if(i == root_index) continue;
        binaryTreeInsert(this->root_index, i);
    }
    std::cout << std::endl;
    printInfo();
}
int B_star::countArea(int& chip_width, int& chip_height){
    int llx = INT8_MAX, lly = INT8_MAX, urx = INT8_MIN, ury = INT8_MIN;
    for(int i = 0; i < nodes; i++){
        llx = std::min(llx, this->ll_coor[i].x);
        lly = std::min(lly, this->ll_coor[i].y);
        urx = std::max(urx, this->ll_coor[i].x + this->borders[i].width);
        ury = std::max(ury, this->ll_coor[i].y + this->borders[i].height);
    }
    chip_width = (urx - llx);
    chip_height = (ury - lly);
    return chip_width * chip_height;
}
int B_star::countHPWL(Layout* L){
    int totalHPWL = 0;
    for(auto n : L->netlist){
        int llx = INT8_MAX, lly = INT8_MAX, urx = INT8_MIN, ury = INT8_MIN;
        for(auto ns : n.net_names){
            int idx;
            if(L->net_name2id.count(ns)){
                idx = L->net_name2id[ns];
                auto i = this->ll_coor[idx];
                auto j = this->borders[idx];
                if(i.x < llx) llx = i.x;
                if(i.y < lly) lly = i.y;
                if(i.x + j.width > urx) urx = i.x + j.width;
                if(i.y + j.height > ury) ury = i.y + j.height;
            }
            else{
                idx = L->ter_name2id[ns];
                auto i = L->terlist[idx];
                if(i.coor.x < llx) llx = i.coor.x;
                if(i.coor.y < lly) lly = i.coor.y;
                if(i.coor.x > urx) urx = i.coor.x;
                if(i.coor.y > ury) ury = i.coor.y;
            }
        }
        totalHPWL += ((int)urx - (int)llx + (int)ury - (int)lly);
    }
    return totalHPWL; 
}
int B_star::countCost(Layout* L, double alpha_ratio){
    int area = this->countArea(L->chip_width, L->chip_height), hpwl = this->countHPWL(L), cost;
    cost = int(alpha_ratio * area + (1 - alpha_ratio) * hpwl);
    return cost;
}
void B_star::cal(int src, int x, int &x_max, int &y_max)
{
    if(src == -1) return;
    if(x + this->borders[src].width > this->contour.size()) this->contour.resize(this->contour.size() * 2);
    int h = -1;
    int w = this->borders[src].width;
    for(int i = x; i < x + w; i++) h = std::max(h, this->contour[i]);
    this->ll_coor[src].x = x;
    this->ll_coor[src].y = h;

    x_max = std::max(x_max, x + w);
    y_max = std::max(y_max, h + this->borders[src].height);
    
    for(int i = x; i < x + w; i++) this->contour[i] = this->ll_coor[src].y + this->borders[src].height;

    B_star::cal(this->left_child[src], x + w, x_max, y_max);
    B_star::cal(this->right_child[src], x, x_max, y_max);   
}
bool B_star::calBlockCoor(Layout* L)
{
    this->contour.clear();
    this->contour.resize(L->width);
    fill(this->contour.begin(), this->contour.end(), 0);
    int x_max = 0, y_max = 0;
    B_star::cal(this->root_index, 0, x_max, y_max);
    if(x_max <= L->width && y_max <= L->height) return true;
    else return false;
}
// TODO
void B_star::insert(int pa, int src)
{
    if(pa == -1) return;
    // std::srand(unsigned(std::time(0)));
    int r = std::rand() % 100;
    if(r < 50){ // 50% insert left
        if(left_child[pa] == -1) {// no left child
            parent[src] = pa;
            left_child[pa] = src;
        }
        else{
            int olc = left_child[pa]; // original left child
            parent[olc] = src;
            parent[src] = pa;
            left_child[pa] = src;
        }
    }
    else{ // 50% insert right
        if(right_child[pa] == -1) {// no right child
            parent[src] = pa;
            right_child[pa] = src;
        }
        else{
            int orc = right_child[pa]; // original right child
            parent[orc] = src;
            parent[src] = pa;
            right_child[pa] = src;
        }
    }
}
void B_star::del(int x)
{
    if(x == root_index){
        
    }
    if(left_child[x] != -1){ // one childreen
        if(left_child[parent[x]] == x) {
            left_child[parent[x]] = left_child[x];
            parent[left_child[x]] = parent[x];
        }
        else if(right_child[parent[x]] == x) {
            right_child[parent[x]] = left_child[x];
            parent[left_child[x]] = parent[x];
        }
    }
    else if(right_child[x] != -1){
        if(left_child[parent[x]] == x) {
            left_child[parent[x]] = right_child[x];
            parent[right_child[x]] = parent[x];
        }
        else if(right_child[parent[x]] == x) {
            right_child[parent[x]] = right_child[x];
            parent[right_child[x]] = parent[x];
        }
    }
    else{ // no childreen
        if(left_child[parent[x]] == x) left_child[parent[x]] = -1;
        else if(right_child[parent[x]] == x) right_child[parent[x]] = -1;
    }
}

void B_star::swapNode(int x, int y)
{
    if(this->parent[x] == y || this->parent[y] == x){
        if (this->parent[x] == y) {
            std::swap(x, y);
        }
        bool is_left = this->left_child[x] == y ;
        if (this->root_index == x) this->root_index = y;
        else if ( this->root_index == y )
            this->root_index = x;
        if (is_left) {
            if (this->right_child[x] != -1) 
                this->parent[this->right_child[x]] = y;
            if (this->right_child[y] != -1) 
                this->parent[this->right_child[y]] = x;
            std::swap(this->right_child[x], this->right_child[y]);
            if (this->left_child[y] != -1) 
                this->parent[this->left_child[y]] = x;
            this->left_child[x] = this->left_child[y];
            this->left_child[y] = x;

        } else {
            if (this->left_child[x] != -1) 
                this->parent[this->left_child[x]] = y;
            if (this->left_child[y] != -1) 
                this->parent[this->left_child[y]] = x;
            std::swap(this->left_child[x], this->left_child[y]);
            if (this->right_child[y] != -1) 
                this->parent[this->right_child[y]] = x;
            this->right_child[x] = this->right_child[y];
            this->right_child[y] = x;
        }
        if (this->parent[x] != -1) {
            int p = this->parent[x];
            if(this->left_child[p] == x) 
                this->left_child[p] = y;
            else    
                this->right_child[p] = y;
        }
        this->parent[y] = this->parent[x];
        this->parent[x] = y;
    }
    else{
        // left child parent
        if(this->left_child[x] != -1){
            auto& lcp_x = parent[this->left_child[x]];
            lcp_x = y;
        }
        if(this->left_child[y] != -1){
            auto& lcp_y = parent[this->left_child[y]];
            lcp_y = x;
        }
        // right child parent
        if(this->right_child[x] != -1){
            auto& rcp_x = parent[this->right_child[x]];
            rcp_x = y;
        }
        if(this->right_child[y] != -1){
            auto& rcp_y = parent[this->right_child[y]];
            rcp_y = x;
        }
        // parent left or right child
        if(x != this->root_index){
            if(this->left_child[this->parent[x]] == x){
                this->left_child[this->parent[x]] = y;
            }
            else{
                this->right_child[this->parent[x]] = y;
            }
        }
        
        if(y != this->root_index){
            if(this->left_child[this->parent[y]] == y){
                this->left_child[this->parent[y]] = x;
            }
            else{
                this->right_child[this->parent[y]] = x;
            }
        }
        

        if(x == this->root_index) this->root_index = y;
        else if(y == this->root_index) this->root_index = x;
        
        // sawp their node
        std::swap(this->parent[x], this->parent[y]);
        std::swap(this->left_child[x], this->left_child[y]);
        std::swap(this->right_child[x], this->right_child[y]);
    }
    // this->printInfo();
}

// debug for B_star structure
void B_star::printInfo()
{
    std::cout << "i       : ";
    for(int i = 0; i < parent.size(); i++) std::cout << std::setw(3) << i << ", ";
    std::cout << std::endl;

    std::cout << "parent  : ";
    for(auto i : parent) std::cout << std::setw(3) << i << ", ";
    std::cout << std::endl;
    
    std::cout << "left_c  : ";
    for(auto i : left_child) std::cout << std::setw(3) << i << ", ";
    std::cout << std::endl;

    std::cout << "right_c : ";
    for(auto i : right_child) std::cout << std::setw(3) << i << ", ";
    std::cout << std::endl;
}


void Layout::addBlock(std::string s, int w, int h){
        net_name2id[s] = blocklist.size();
        blocklist.emplace_back(Block(s, w, h));
}
void Layout::addTer(std::string s, int a, int b){
    ter_name2id[s] = terlist.size();
    terlist.emplace_back(Terminal(s, a, b));
}
void Layout::SA()
{
    // std::srand(unsigned(std::time(0)));
    int op, k = 25, T = 4000, T_frozen = 1;
    double r = 0.9;
    this->B = new B_star(num_blocks);
    this->B->initBstar(this);
    this->B->calBlockCoor(this);
    B_star* B_prime;
    int old_cost = this->B->countCost(this, 1), new_cost;
    do{
        B_prime = new B_star(*(this->B));
        
        op = std::rand() % 2 + 1;
        int n1, n2;
        if(op == 1){ // op1
            n1 = std::rand() % this->num_blocks;
            std::swap(B_prime->borders[n1].width, B_prime->borders[n1].height);
        }
        else if(op == 2){ // op2
            n1 = std::rand() % this->num_blocks;
            n2 = std::rand() % this->num_blocks;
            while(n1 == n2){
                n2 = std::rand() % this->num_blocks;
            }
            int tmp = 0; B_prime->dfs(B_prime->root_index, tmp);
            B_prime->swapNode(n1, n2);
        }
        B_prime->calBlockCoor(this);
        new_cost = B_prime->countCost(this, 1);
        int accept = std::rand() % RAND_MAX;
        if(new_cost <= old_cost){
            this->B = B_prime;
            old_cost = new_cost;
            std::cout << "down hill" << std::endl;
        }
        else if(accept > std::exp(-(new_cost - old_cost) / T)){
            this->B = B_prime;
            old_cost = new_cost;
            std::cout << "  up hill" << std::endl;
        }
        std::cout << "old cost: " << old_cost << " new_cost: " << new_cost << std::endl; 
    }while(!this->B->calBlockCoor(this));
    this->opt_B = this->B;
    /*

    while(T > T_frozen){
        for(int i = 0; i < k i++){
            if(op == 1){ // op1
                int n1 = std::rand() % this->num_blocks + 1;
                std::swap(this->blocklist[n1].width, this->blocklist[n1].height);
            }
            else if(op == 2){ // op2
                int n1 = std::rand() % this->num_blocks + 1;
                int n2 = std::rand() % this->num_blocks + 1;
                while(n1 == n2){
                    n2 = std::rand() & this->num_blocks + 1;
                }
                this->B->swapNode(n1, n2);
            }
            else{
                continue;
            }
        }
    }
    */
}