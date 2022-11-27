#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
class Coordinate
{
public:
    int x, y;
    Coordinate(){}
    Coordinate(int _x, int _y) : x(_x), y(_y) {}
};
// Base class
class Block
{
public:
    int name;
    Coordinate ll_coor;
    int width, height;
    Block(){}
    Block(int n, Coordinate c, int w, int h) : name(n), ll_coor(c), width(w), height(h) {}
    int64_t getArea(){ return (int64_t)this->width * (int64_t)this->height;}
    void printBlock();
};

class Terminal : public Block
{
public:
    Terminal() : Block() {}
    Terminal(int n, Coordinate c, int w, int h) : Block(n, c, w, h) {}
    void printBlock(){ this->printBlock(); }
};

class Cell : public  Block
{
public:
    bool is_top;
    Cell() : Block() {}
    Cell(int n, Coordinate c, int w, int h, bool t) : Block(n, c, w, h), is_top(t) {}
    void printBlock();
    int getArea(){
        return this->width * this->height;
    }
};

class Layout
{
public:
    int width, height;
    int row_hei, num_of_row;
    int num_of_ter, num_of_cell;
    std::vector<Cell*> celllist;
    std::vector<Terminal*> terlist;
    std::vector<Cell*> gp_result;
    Layout(){}
    void addTerminal(int n, Coordinate c, int w, int h){
        Terminal* t = new Terminal(n, c, w, h);
        this->terlist.push_back(t);
    }
    void addCell(int n, Coordinate c, int w, int h, bool t){
        Cell* t_c = new Cell(n, c, w, h, t);
        this->celllist.push_back(t_c);
        this->gp_result.push_back(t_c);
    }
    void split(Layout &top, Layout &bot);
    void merge(Layout &top, Layout &bot);
};