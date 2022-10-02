#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include "io.hpp"

void readCaseToLayout(Result &R, char const *file_path)
{
    std::ifstream input(file_path);
    std::string line, token;
    std::stringstream ss;
    
    getline(input, line);
    ss.clear(); ss.str(line);
    int tmp_width, tmp_height;
    coordinate tmp_coor;
    ss >> tmp_width >> tmp_height;
    Layout L{Layout{tmp_width, tmp_height}};
    while(getline(input, line))
    {
        ss.clear(); ss.str(line);
        ss >> token;
        if(token == "P") 
        {
            ss >> tmp_coor.x >> tmp_coor.y;
            Tile *t_p = L.pointFinding(L.ll_tile, tmp_coor);
            if(t_p != nullptr) 
            {
                R.point_finding_list.push_back(t_p->ll_coor);
            }
        }
        else
        {
            ss >> tmp_coor.x >> tmp_coor.y >> tmp_width >> tmp_height;
            // becareful tileCreation is using ul coordinate
            if(L.tileCreation(coordinate{tmp_coor.x, tmp_coor.y + tmp_height}, tmp_width, tmp_height, stoi(token))) std::cout << "#" << token << " is created\n";
            else std::cout << "@" << token << " created failed!\n";
            R.all_block_tile_min_pq.push(stoi(token));
            R.tile_index2coor[stoi(token)] = tmp_coor;

            // debug;
            // auto d_v = L.directedAreaEnumeration({coordinate(0, L.height)}, L.width, L.height);
            // for(auto i : d_v)
            // {
            //     i->printTile();
            // }
            R.all_tiles = L.directedAreaEnumeration({coordinate(0, L.height)}, L.width, L.height);
        }
    }
    R.all_tiles = L.directedAreaEnumeration({coordinate(0, L.height)}, L.width, L.height);
    
    R.L = L;
    return;
}
std::pair<int, int> calcNeighborSpaceOrBlock(Layout L, coordinate ll_core, int index)
{
    Tile* cur = L.pointFinding(L.ll_tile, ll_core);
    if(index != cur->index)
    {
        std::cout << "Index Error.\n";
        return std::make_pair(-1, -1);
    }
    std::vector<Tile*> lnt = L.neighborFinding(cur, orientation::Left); // left neighbors
    std::vector<Tile*> rnt = L.neighborFinding(cur, orientation::Right); // right neighbors
    std::vector<Tile*> tnt = L.neighborFinding(cur, orientation::Top); // Top neighbors
    std::vector<Tile*> bnt = L.neighborFinding(cur, orientation::Bottom); // Bottom neighbors
    int block = 0, space = 0;
    for(auto i : lnt)
    {
        if(i->index > 0) block++;
        else space++;
    }
    for(auto i : rnt)
    {
        if(i->index > 0) block++;
        else space++;
    }
    for(auto i : tnt)
    {
        if(i->index > 0) block++;
        else space++;
    }
    for(auto i : bnt)
    {
        if(i->index > 0) block++;
        else space++;
    }
    return std::make_pair(block, space);
}
void Result::Out(outputs o)
{
    int top, tmp_block, tmp_space;
    switch (o)
    {
    case outputs::StandardO:
        std::cout << all_tiles.size() << std::endl;
        while(!all_block_tile_min_pq.empty())
        {
            int top = all_block_tile_min_pq.top();
            all_block_tile_min_pq.pop();
            std::tie(tmp_block, tmp_space) = calcNeighborSpaceOrBlock(Result::L, Result::tile_index2coor[top], top);
            std::cout << top << " " << tmp_block << " " << tmp_space << std::endl;
        }
        for(auto i :point_finding_list) std::cout << i.x << " " << i.y << std::endl;
        break;
    case outputs::Files:
        break;
    default:
        break;
    }
}
void Result::Draw(char const *file_path)
{
    std::ofstream out_draw(file_path, std::ofstream::trunc);
    out_draw << Result::all_tiles.size() << std::endl;
    out_draw << Result::L.width << " " << Result::L.height << std::endl;
    for(auto i : Result::all_tiles)
    {
        out_draw << i->index << " " << i->ll_coor.x << " " << i->ll_coor.y << " "; 
        out_draw << i->width << " " << i->height << std::endl;
    }
}