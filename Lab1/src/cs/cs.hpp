#pragma once
#include <vector>
enum orientation
{
    Top = 1,
    Right = 2,
    Bottom = 3,
    Left = 4
};
struct coordinate
{
    coordinate(){}
    coordinate(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int x;
    int y;
};
struct Tile
{
    Tile(){}
    Tile(Tile* rt, Tile* tr, Tile* bl, Tile* lb, coordinate ll_coor, int width, int height, int index)
    {
        this->rt = rt;
        this->tr = tr;
        this->bl = bl;
        this->lb = lb;
        this->ll_coor = ll_coor;
        this->width = width;
        this->height = height;
        this->index = index;
    }
    Tile* rt;
    Tile* tr;
    Tile* bl;
    Tile* lb;
    coordinate ll_coor; // lower left coordinate
    int width;
    int height;
    int index;

    void printTile();
};
struct Layout
{
    Layout(){}
    Layout(int width, int height)
    {
        this->width = width;
        this->height = height;
        *ll_tile = {Tile(nullptr, nullptr, nullptr, nullptr, {coordinate(0, 0)}, width, height, -1)};
    } 
    int width;
    int height;
    coordinate hint_point_coor{coordinate(0, 0)};
    Tile* ll_tile;

    Tile* pointFinding(Tile* cur, coordinate p); // Before use it, check whether it is nullptr
    std::vector<Tile*> neighborFinding(Tile* cur, orientation ori);
    bool areaSearch(coordinate ul_coor, int desired_width, int desired_height); // return desired area whether include a solid tile
    std::vector<Tile*> directedAreaEnumeration();
    bool tileCreation(coordinate ul_coor, int desired_width, int desired_height, int index); // if create success, return true
};