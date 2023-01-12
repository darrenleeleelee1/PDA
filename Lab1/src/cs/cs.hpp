#pragma once
#include <vector>
enum Orientation
{
    Top = 1,
    Right = 2,
    Bottom = 3,
    Left = 4
};
struct Coordinate
{
    Coordinate(){}
    Coordinate(int x, int y)
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
    Tile(Tile* rt, Tile* tr, Tile* bl, Tile* lb, Coordinate ll_coor, int width, int height, int index)
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
    Coordinate ll_coor; // lower left coordinate
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
        ll_tile = new Tile(nullptr, nullptr, nullptr, nullptr, {Coordinate(0, 0)}, width, height, -1);
    } 
    int width;
    int height;
    Coordinate hint_point_coor{Coordinate(0, 0)};
    Tile* ll_tile;

    Tile* pointFinding(Tile* cur, Coordinate p); // Before use it, check whether it is nullptr
    std::vector<Tile*> neighborFinding(Tile* cur, Orientation ori);
    bool areaSearch(Coordinate ul_coor, int desired_width, int desired_height); // return desired area whether include a solid tile
    std::vector<Tile*> directedAreaEnumeration(Coordinate ul_coor, int desired_width, int desired_height);
    bool tileCreation(Coordinate ul_coor, int desired_width, int desired_height, int index); // if create success, return true
};


void directedAreaEnumeration_R_procedure(Layout* L, std::vector<Tile*> &t_v, Tile* cur,Coordinate ul_coor, int desired_width, int desired_height);

Tile* split(Layout *l_p, Tile* bot_tile, int lower_tile_height);

std::pair<Tile*, Tile*> divide(Tile* mid_tile, Coordinate ul_coor, int desired_width);

void merge(Layout *L, Tile* top_tile, Tile* bot_tile);