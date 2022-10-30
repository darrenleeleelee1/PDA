#pragma once
#include "./ds/design.hpp" 
#include <iostream>
#include <cstdlib>
#include <ctime> 
struct SP;
struct Layout
{
    int width, height, num_blocks, num_ters, num_nets;
    int chip_width, chip_height;
    double alpha;
    int opt_area;
    int opt_hpwl;
    int opt_cost;
    std::vector<Net> netlist;
    std::vector<Block> blocklist;
    std::vector<Terminal> terlist;
    std::unordered_map<std::string, int> net_name2id;
    std::unordered_map<std::string, int> ter_name2id;
    
    SP* sp;
    SP* opt_sp;
    Layout(){
        this->blocklist.resize(0);
        this->terlist.resize(0);
    }
    Layout(int w, int h) : width(w), height(h){
        Layout();
    }
    Layout(double alp) : alpha(alp){
        Layout();
    }
    void initLayout();
    void addBlock(std::string s, int w, int h);
    void addTer(std::string s, int a, int b);
    void SA();
};
struct SP
{
    int nodes;
    std::vector<int> pos_seq;
    std::vector<int> neg_seq;
    std::vector<Block> blocklist;
    std::vector<Coordinate> match; // here coordinate x and y represent x is pos_seq, y is neg_seq
    std::vector<Coordinate> L; // here coordinate x and y represent x is pos_seq, y is neg_seq
    std::vector<Coordinate> P; // here coordinate x and y represent x is pos_seq, y is neg_seq
    SP(){}
    SP(int n) : nodes(n), pos_seq(n), neg_seq(n), match(n), L(n), P(n){
        for(int i = 0; i < n; i++){
            this->pos_seq[i] = i;
            this->neg_seq[i] = i;
        }
    }
    SP(Layout* layout, int n) : nodes(n), pos_seq(n), neg_seq(n), match(n), L(n), P(n){
        for(int i = 0; i < n; i++){
            this->pos_seq[i] = i;
            this->neg_seq[i] = i;
        }
        for(auto i : layout->blocklist){
            this->blocklist.emplace_back(Block(i));
        }
    }
    void swapBlocks(int seq, int x, int y);
    void swapBorders(int x);
    void randomSP();
    void calMatch();
    void posMWCS();
    void negMWCS();
    void MWCS();
    int countArea(int &chip_width, int &chip_height);
    int countHPWL(Layout *layout);
    int countCost(Layout *layout, double alpha_ratio);
};

