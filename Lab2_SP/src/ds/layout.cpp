#include "./ds/layout.hpp"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>

void Layout::addBlock(SP *sp, std::string s, int w, int h)
{
    net_name2id[s] = sp->blocklist.size();
    sp->blocklist.emplace_back(Block(s, w, h));
}
void Layout::addTer(std::string s, int a, int b)
{
    ter_name2id[s] = terlist.size();
    terlist.emplace_back(Terminal(s, a, b));
}
void Layout::SA()
{
    // std::srand(unsigned(std::time(0)));
    int op, k = 25, T = 4000, T_frozen = 1;
    double r = 0.9;
    B_star* B_prime;
    int old_cost, new_cost;
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