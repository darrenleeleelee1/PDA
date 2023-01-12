#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include "../cs/cs.hpp"

enum Outputs
{
    StandardO = 1,
    Files = 2
};
struct Result
{
    Result(){}
    Layout L;
    std::vector<Tile*> all_tiles;
    std::vector<Coordinate> point_finding_list;
    std::priority_queue<int, std::vector<int>, std::greater<int>> all_block_tile_min_pq;
    std::unordered_map<int, Coordinate> tile_index2coor;
    void Out(Outputs o, char const *file_path);
    void Draw(char const *file_path);
};


void readCaseToLayout(Result &R, char const *file_path);