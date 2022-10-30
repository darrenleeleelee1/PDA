#pragma once
#include <string>
#include <vector>
#include <unordered_map>
struct Border
{
    int width, height;
    Border(){}
    Border(int w, int h) : width(w), height(h){}
};
struct Coordinate
{
    int x, y;
    Coordinate(){}
    Coordinate(int a, int b) : x(a), y(b){}
};
struct Net
{
    std::vector<std::string> net_names;
    int size;
    Net() : size(0), net_names(0){}
    Net(int s) : size(s), net_names(0){}
};
struct Block
{
    std::string block_name;
    Border borders;
    Block(){}
    Block(std::string s, int w, int h) : block_name(s), borders(w, h){}
};
struct Terminal
{
    std::string ter_name;
    Coordinate coor;
    Terminal(){}
    Terminal(std::string s, int a, int b) : ter_name(s), coor(a, b){}
};