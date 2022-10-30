#pragma once
#include <cstdlib>
#include <ctime> 
#include <iostream>
#include "./ds/design.hpp" 
struct Layout;
struct B_star;

struct B_star
{
    std::vector<int> parent;
    std::vector<int> left_child;
    std::vector<int> right_child;
    std::vector<Coordinate> ll_coor;
    std::vector<Border> borders;
    std::vector<int> contour;
    int root_index;
    int nodes;
    B_star(){}
    B_star(int n) : nodes(n), root_index(-1){}
    void binaryTreeInsert(int pa, int src);
    void initBstar(Layout* L);

    int countArea(int& chip_width, int& chip_height);
    int countHPWL(Layout* L);
    int countCost(Layout* L, double alpha_ratio);
    
    void cal(int src, int x, int &x_max, int &y_max);
    bool calBlockCoor(Layout* L);

    void swapNode(int x, int y);
    void del(int x);
    void insert(int pa, int src);

    // DEBUG
    void printInfo();
    void dfs(int src, int& cnt){
        if(src == -1) return;
        cnt += 1;
        dfs(this->left_child[src], cnt);
        dfs(this->right_child[src], cnt);
    }
};

struct Layout
{
    int width, height, num_blocks, num_ters, num_nets;
    int chip_width, chip_height;
    double alpha;
    int opt_area = 0;
    int opt_hpwl = 0;
    int opt_cost = INT32_MAX;
    std::vector<Net> netlist;
    std::vector<Block> blocklist;
    std::vector<Terminal> terlist;
    std::unordered_map<std::string, int> net_name2id;
    std::unordered_map<std::string, int> ter_name2id;
    B_star *B;
    B_star *opt_B;
    
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
