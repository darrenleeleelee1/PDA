#pragma once
#include <vector>
#include <string>
struct Coordinate
{
    int x;
    int y;
    Coordinate(){}
    Coordinate(int a, int b) : x(a), y(b){}
};
struct Net
{
    std::vector<std::string> net_names;
    int size;
    Net(){}
    Net(int s) : size(s)
    {
        net_names.resize(size);
    }
};
struct Block
{
    std::string block_name;
    Coordinate ll_coor;
    Block(){}
    Block(std::string s, int a, int b) : block_name(s), ll_coor(a, b){}
};
struct Terminal
{

};


struct Layout
{

};
