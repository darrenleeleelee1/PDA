#pragma once
#include "./ds/design.hpp" 
#include <iostream>
#include <cstdlib>
#include <ctime> 
struct Layout;
struct SP
{
    int nodes;
    std::vector<int> positive_sequence;
    std::vector<int> negative_sequence;
    std::vector<Block> blocklist;
    std::vector<Coordinate> match;
    std::vector<int> L;
    std::vector<int> P;
    SP(){}
    SP(int n) : nodes(s){}

    void randomSP();
    int positiveMWCS();
    int negativeMWCS();

};
struct Layout
{
    int width, height, num_blocks, num_ters, num_nets;
    int chip_width, chip_height;
    double alpha;
    int opt_area = 0;
    int opt_hpwl = 0;
    int opt_cost = INT8_MAX;
    std::vector<Net> netlist;
    std::vector<Terminal> terlist;
    std::unordered_map<std::string, int> net_name2id;
    std::unordered_map<std::string, int> ter_name2id;
    
    

    
    Layout(){
        blocklist.resize(0);
        terlist.resize(0);
    }
    Layout(int w, int h) : width(w), height(h){
        Layout();
    }
    Layout(double alp) : alpha(alp){
        Layout();
    }

    void addBlock(std::string s, int w, int h);
    void addTer(std::string s, int a, int b);
    void SA();
};
