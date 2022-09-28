#include <iostream>
#include "cs.hpp"
void Tile::printTile()
{
    std::cout << "#" << Tile::index << std::endl;
    std::cout << "coordinate(" << Tile::ll_coor.x << ", " << Tile::ll_coor.y << ")" << std::endl;
    std::cout << "Width: " << Tile::width << ", Height: " << Tile::height << std::endl;
}
Tile* Layout::pointFinding(Tile* cur, coordinate p)
{
    if((p.x == Layout::width || p.x < 0) && (p.y == Layout::height || p.y < 0)) {std::cout << "x, y coordinate out of Layout bound\n"; return nullptr;}
    else if(p.x == Layout::width || p.x < 0) {std::cout << "x coordinate out of Layout bound\n"; return nullptr;}
    else if(p.y == Layout::height || p.y < 0) {std::cout << "y coordinate out of Layout bound.\n"; return nullptr;}
    
    if((p.y >= cur->ll_coor.y) && (p.y < cur->ll_coor.y + cur->height))
    {
        if((p.x >= cur->ll_coor.x) && (p.x < cur->ll_coor.x + cur->width))
        {
            return cur;
        }
        else
        {
            if((p.x < cur->ll_coor.x)) // go left
            {
                if(cur->bl == nullptr)
                {
                    return nullptr;
                }
                return Layout::pointFinding(cur->bl, p);
            }
            else if(p.x >= cur->ll_coor.x + cur->width)
            {
                if(cur->tr == nullptr)
                {
                    return nullptr;
                }
                return Layout::pointFinding(cur->tr, p); // go right
            }
        }
    }
    else
    {
        if(p.y < cur->ll_coor.y) // go down
        {
            if(cur->lb == nullptr)
            {
                return nullptr;
            }
            return Layout::pointFinding(cur->lb, p);
        }
        else if(p.y >= cur->ll_coor.y + cur->height) // go up
        {
            if(cur->rt == nullptr)
            {
                return nullptr;
            }
            return Layout::pointFinding(cur->rt, p);
        }
    }
    
    
}
std::vector<Tile*> Layout::neighborFinding(Tile* cur, orientation ori)
{
    std::vector<Tile*> nt; // neightbor tiles
    Tile* start_tile = cur;
    switch (ori)
    {
    case orientation::Top:
        std::cout << "Top neighbor finding" << std::endl;
        if(cur->rt != nullptr) // go top
        {
            nt.push_back(cur->rt);
            cur = cur->rt;
        }
        else
        {
            std::cout << "No top neighbor" << std::endl;
            return nt;
        }
        while(cur->bl != nullptr && (cur->ll_coor.x + cur->width < start_tile->ll_coor.x)) // go left
        {
            nt.push_back(cur->bl);
            cur = cur->bl;
        }
        break;
    case orientation::Right:
        std::cout << "Right neighbor finding" << std::endl;
        if(cur->tr != nullptr) // go right
        {
            nt.push_back(cur->tr);
            cur = cur->tr;
        }
        else
        {
            std::cout << "No right neighbor" << std::endl;
            return nt;
        }
        while(cur->lb != nullptr && (cur->ll_coor.y + cur->height < start_tile->ll_coor.y)) // go down
        {
            nt.push_back(cur->lb);
            cur = cur->lb;
        }
        break;
    case orientation::Bottom:
        std::cout << "Bottom neighbor finding" << std::endl;
        if(cur->lb != nullptr) // go bot
        {
            nt.push_back(cur->lb);
            cur = cur->lb;
        }
        else
        {
            std::cout << "No bottom neighbor" << std::endl;
            return nt;
        }
        while(cur->tr != nullptr && (cur->ll_coor.x > start_tile->ll_coor.x + start_tile->width)) // go right
        {
            nt.push_back(cur->tr);
            cur = cur->tr;
        }
        break;
    case orientation::Left:
        std::cout << "Left neighbor finding" << std::endl;
        if(cur->bl != nullptr) // go right
        {
            nt.push_back(cur->bl);
            cur = cur->bl;
        }
        else
        {
            std::cout << "No left neighbor" << std::endl;
            return nt;
        }
        while(cur->rt != nullptr && (cur->ll_coor.y > start_tile->ll_coor.y + height)) // go down
        {
            nt.push_back(cur->rt);
            cur = cur->rt;
        }
        break;
    default:
        break;
    }
}
bool Layout::areaSearch(coordinate ul_coor, int desired_width, int desired_height)
{
    Tile* t_p = Layout::pointFinding(Layout::ll_tile, ul_coor);
    
    if(t_p == nullptr){std::cout << "ul_coor out of bound.\n"; return false;}

    if(t_p->index >= 0) return true; // the start point(ul_coor) is in a solid tile
    if(t_p->ll_coor.x + t_p->width < ul_coor.x + desired_with) return true; // the space tile not longer then desired area, the right neighbor must be a solid tile
    
    while(t_p->lb != nullptr)
    {
        t_p = t_p->lb; // go down
        while(t_p->ll_coor.x + t_p->width <= ul_coor.x && t_p->tr != nullptr) t_p = t_p->tr; // go right to eliminate misalignment
        
        if(t_p->ll_coor.y + t_p->height <= ul_coor.y - desired_height) return false; // if the search out of the bottom edge

        if(t_p->index >= 0) return true; // the tile a solid tile
        if(t_p->ll_coor.x + t_p->width < ul_coor.x + desired_with) return true; // the space tile not longer then desired area, the right neighbor must be a solid tile
    
    }

}
std::vector<Tile*> Layout::directedAreaEnumeration()
{
    std::vector<Tile*> t_v;

    return t_v;
}
Tile* split(Tile* original, int height)
{
    Tile be_split = {Tile(original->rt, original->tr, nullptr, original, original->width, original->height - height)};
    original->rt = &be_split; original->tr = nullptr
}
bool Layout::tileCreation(coordinate ul_coor, int desired_width, int desired_height, int index)
{
    Tile* topmost, bottommost;
    Tile* pre_left, pre_mid, pre_right;

    if(Layout::areaSearch(ul_coor, desired_width, desired_height)) Tile* start_tile = Layout::pointFinding(Layout::ll_tile, ul_coor);
    else return false;

    if(ul_coor.y == start_tile->ll_coor.y + start_tile->height) // 不需split
    {
        topmost = nullptr;
    }
    else
    {
        topmost = split(start_tile, ul_coor.y - start_tile->ll_coor.y);
    }


    return true;
}