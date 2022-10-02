#include <iostream>
#include <tuple>
#include "cs.hpp"
void Tile::printTile()
{
    std::cout << "#" << Tile::index << std::endl;
    std::cout << "coordinate(" << Tile::ll_coor.x << ", " << Tile::ll_coor.y << ")" << std::endl;
    std::cout << "Width: " << Tile::width << ", Height: " << Tile::height << std::endl;
    if(Tile::rt != nullptr) std::cout << "^" << Tile::rt->index << std::endl;
    else std::cout << "^ is null\n";
    if(Tile::tr != nullptr) std::cout << "->" << Tile::tr->index << std::endl;
    else std::cout << "-> is null\n";
    if(Tile::lb != nullptr) std::cout << "ˇ" << Tile::lb->index << std::endl;
    else std::cout << "ˇ is null\n";
    if(Tile::bl != nullptr) std::cout << "<-" << Tile::bl->index << std::endl;
    else std::cout << "<- is null\n";
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
    
    return nullptr; // Should not go to here
    
}
std::vector<Tile*> Layout::neighborFinding(Tile* cur, orientation ori)
{
    std::vector<Tile*> nt; // neightbor tiles
    Tile* start_tile = cur;
    switch (ori)
    {
    case orientation::Top:
        if(cur->rt != nullptr) // go top
        {
            nt.push_back(cur->rt);
            cur = cur->rt;
        }
        else
        {
            return nt;
        }
        while(cur->bl != nullptr && !(cur->bl->ll_coor.x + cur->bl->width <= start_tile->ll_coor.x)) // go left
        {
            nt.push_back(cur->bl);
            cur = cur->bl;
        }
        break;
    case orientation::Right:
        if(cur->tr != nullptr) // go right
        {
            nt.push_back(cur->tr);
            cur = cur->tr;
        }
        else
        {
            return nt;
        }
        while(cur->lb != nullptr && !(cur->lb->ll_coor.y + cur->lb->height <= start_tile->ll_coor.y)) // go down
        {
            nt.push_back(cur->lb);
            cur = cur->lb;
        }
        break;
    case orientation::Bottom:
        if(cur->lb != nullptr) // go bot
        {
            nt.push_back(cur->lb);
            cur = cur->lb;
        }
        else
        {
            return nt;
        }
        while(cur->tr != nullptr && !(cur->tr->ll_coor.x >= start_tile->ll_coor.x + start_tile->width)) // go right
        {
            nt.push_back(cur->tr);
            cur = cur->tr;
        }
        break;
    case orientation::Left:
        if(cur->bl != nullptr) // go left
        {
            nt.push_back(cur->bl);
            cur = cur->bl;
        }
        else
        {
            // std::cout << "No left neighbor" << std::endl;
            return nt;
        }
        while(cur->rt != nullptr && !(cur->rt->ll_coor.y >= start_tile->ll_coor.y + start_tile->height)) // go down
        {
            cur = cur->rt;
            nt.push_back(cur);
        }
        break;
    default:
        break;
    }
    return nt;
}
bool Layout::areaSearch(coordinate ul_coor, int desired_width, int desired_height)
{
    Tile* t_p = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - 1)});
    
    if(t_p == nullptr){std::cout << "ul_coor out of bound.\n"; return false;}

    if(t_p->index >= 0) return true; // the start point(ul_coor) is in a solid tile
    if(t_p->ll_coor.x + t_p->width < ul_coor.x + desired_width) return true; // the space tile not longer then desired area, the right neighbor must be a solid tile
    
    while(t_p->lb != nullptr)
    {
        t_p = t_p->lb; // go down
        while(t_p->ll_coor.x + t_p->width <= ul_coor.x && t_p->tr != nullptr) t_p = t_p->tr; // go right to eliminate misalignment
        
        if(t_p->ll_coor.y + t_p->height <= ul_coor.y - desired_height) return false; // if the search out of the bottom edge

        if(t_p->index >= 0) return true; // the tile a solid tile
        if(t_p->ll_coor.x + t_p->width < ul_coor.x + desired_width) return true; // the space tile not longer then desired area, the right neighbor must be a solid tile
    }

    return false;

}
void directedAreaEnumeration_R_procedure(Layout* L, std::vector<Tile*> &t_v, Tile* cur,coordinate ul_coor, int desired_width, int desired_height)
{
    if(cur->ll_coor.x + cur->width > ul_coor.x + desired_width) return;
    t_v.push_back(cur);
    
    std::vector<Tile*> rnt = L->neighborFinding(cur, orientation::Right);

    for(auto i : rnt)
    {
        if(cur->ll_coor.y <= ul_coor.y - desired_height && ul_coor.y - desired_height < cur->ll_coor.y + cur->height)
        {
            if(i->ll_coor.y <= ul_coor.y - desired_height && ul_coor.y - desired_height < i->ll_coor.y + i->height)
            {
                directedAreaEnumeration_R_procedure(L, t_v, i, ul_coor, desired_width, desired_height);
            }
        }
        else if(i->bl == cur)
        {
            directedAreaEnumeration_R_procedure(L, t_v, i, ul_coor, desired_width, desired_height);
        }
    }

    return;
}
std::vector<Tile*> Layout::directedAreaEnumeration(coordinate ul_coor, int desired_width, int desired_height)
{
    std::vector<Tile*> t_v;
    Tile* ul_tile = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - 1)});
    Tile* t_p = ul_tile;
    while(t_p->ll_coor.y + t_p->height >= ul_coor.y - desired_height)
    {
        directedAreaEnumeration_R_procedure(this, t_v, t_p, ul_coor, desired_width, desired_height);
        if(t_p->lb == nullptr) break;
        
        t_p = t_p->lb;
        while(t_p->ll_coor.x + t_p->width <= ul_coor.x) t_p = t_p->tr;
    }
    return t_v;
}
bool isNeighbor(orientation ori, Tile* cur, Tile* test) 
{
    // check test tile 的 stitch 是否能接到 cur tile
    // orientation 為 test tile 在 cur tile 的哪邊， 但是是 test tile 的 stitch 往 cur tile 指
    // conclusion stitch 方向 為 orientation 的反向
    if(cur == nullptr || test == nullptr) return false;
    if(cur == test) return false;

    switch(ori)
    {
    case orientation::Left:
        if(test->ll_coor.x + test->width != cur->ll_coor.x) return false;
        if(cur->ll_coor.y < test->ll_coor.y + test->height && test->ll_coor.y + test->height <= cur->ll_coor.y + cur->height) return true;
        else return false;
        break;
    case orientation::Right:
        if(cur->ll_coor.x + cur->width != test->ll_coor.x) return false;
        if(cur->ll_coor.y <= test->ll_coor.y && test->ll_coor.y < cur->ll_coor.y + cur->height) return true;
        else return false;
        break;
    case orientation::Top:
        if(cur->ll_coor.y + cur->height != test->ll_coor.y) return false;
        if(cur->ll_coor.x <= test->ll_coor.x && test->ll_coor.x < cur->ll_coor.x + cur->width) return true;
        else return false;
        break;
    case orientation::Bottom:
        if(test->ll_coor.y + test->height != cur->ll_coor.y) return false;
        if(cur->ll_coor.x < test->ll_coor.x + test->width && test->ll_coor.x + test->width <= cur->ll_coor.x + cur->width) return true;
        else return false;
        break;
    default:
        return false;
        break;
    }
}
Tile* split(Layout *l_p, Tile* bot_tile, int lower_tile_height)
{
    // store neighbors first
    std::vector<Tile*> lnt = l_p->neighborFinding(bot_tile, orientation::Left); // left neighbors
    std::vector<Tile*> rnt = l_p->neighborFinding(bot_tile, orientation::Right); // right neighbors
    std::vector<Tile*> tnt = l_p->neighborFinding(bot_tile, orientation::Top); // Top neighbors
    std::vector<Tile*> bnt = l_p->neighborFinding(bot_tile, orientation::Bottom); // Bottom neighbors
    // linking stitching and resize
    Tile* top_tile = new Tile(bot_tile->rt, bot_tile->tr, nullptr, bot_tile, {coordinate(bot_tile->ll_coor.x, bot_tile->ll_coor.y + lower_tile_height)}, bot_tile->width, bot_tile->height - lower_tile_height, -1);
    bot_tile->rt = top_tile; bot_tile->tr = nullptr; bot_tile->height = lower_tile_height;
    // check neighbor not stitch to wrong tile
    for(auto i : lnt)
    {
        if(isNeighbor(orientation::Left, bot_tile, i)) i->tr = bot_tile;
        if(isNeighbor(orientation::Left, top_tile, i)) i->tr = top_tile;
        if(isNeighbor(orientation::Right, i, top_tile)) top_tile->bl = i;
    }
    for(auto i : rnt)
    {
        if(isNeighbor(orientation::Right, bot_tile, i)) i->bl = bot_tile;
        if(isNeighbor(orientation::Right, top_tile, i)) i->bl = top_tile;
        if(isNeighbor(orientation::Left, i, bot_tile)) bot_tile->tr = i;
    }
    for(auto i : tnt)
    {
        if(isNeighbor(orientation::Top, bot_tile, i)) i->lb = bot_tile;
        if(isNeighbor(orientation::Top, top_tile, i)) i->lb = top_tile;
    }
    for(auto i : bnt)
    {
        if(isNeighbor(orientation::Top, bot_tile, i)) i->rt = bot_tile;
        if(isNeighbor(orientation::Top, top_tile, i)) i->rt = top_tile;
    }

    return top_tile;
}
std::pair<Tile*, Tile*> divide(Tile* mid_tile, coordinate ul_coor, int desired_width)
{
    Tile* left_tile, * right_tile;
    coordinate mid_ll_coor(mid_tile->ll_coor.x + (ul_coor.x - mid_tile->ll_coor.x), mid_tile->ll_coor.y);
    if(mid_tile->ll_coor.x == ul_coor.x)
    {
        left_tile = mid_tile;
    }
    else
    {
        left_tile = new Tile(nullptr, nullptr, mid_tile->bl, mid_tile->lb, mid_tile->ll_coor, (ul_coor.x - mid_tile->ll_coor.x), (mid_tile->height), -1);
    }

    if(mid_tile->ll_coor.x + mid_tile->width == ul_coor.x + desired_width)
    {
        right_tile = mid_tile;
    }
    else if (left_tile == mid_tile) // 只能分成中、右兩塊時
    {
        right_tile = new Tile(mid_tile->rt, mid_tile->tr, nullptr, nullptr, {coordinate((mid_ll_coor.x + desired_width), mid_ll_coor.y)}, (mid_tile->width - desired_width), (mid_tile->height), -1);
    }
    else
    {
        right_tile = new Tile(mid_tile->rt, mid_tile->tr, nullptr, nullptr, {coordinate((mid_ll_coor.x + desired_width), mid_ll_coor.y)}, (mid_tile->width - desired_width - left_tile->width), (mid_tile->height), -1);
    }

    if(mid_tile != left_tile)
    {
        mid_tile->bl = left_tile;
        mid_tile->lb = nullptr;
        left_tile->tr = mid_tile;
    }
    if(mid_tile != right_tile)
    {
        mid_tile->tr = right_tile;  
        mid_tile->rt = nullptr; 
        right_tile->bl = mid_tile;
    }
    mid_tile->ll_coor = mid_ll_coor; mid_tile->width = desired_width;
    
    // stitch bl
    Tile* tmp_p = left_tile->lb;
    while(!isNeighbor(orientation::Top, tmp_p, mid_tile) && tmp_p->tr != nullptr) tmp_p = tmp_p->tr;
    mid_tile->lb = tmp_p;
    while(!isNeighbor(orientation::Top, tmp_p, right_tile) && tmp_p->tr != nullptr) tmp_p = tmp_p->tr;
    right_tile->lb = tmp_p;
    return std::make_pair(left_tile, right_tile);
}
void merge(Layout *L, Tile* top_tile, Tile* bot_tile)
{
    std::vector<Tile*> lnt = L->neighborFinding(bot_tile, orientation::Left); // left neighbors
    std::vector<Tile*> rnt = L->neighborFinding(bot_tile, orientation::Right); // right neighbors
    std::vector<Tile*> bnt = L->neighborFinding(bot_tile, orientation::Bottom); // Bottom neighbors
    
    top_tile->bl = bot_tile->bl;
    top_tile->lb = bot_tile->lb;
    top_tile->ll_coor = bot_tile->ll_coor;
    top_tile->height = top_tile->height + bot_tile->height;
    
    for(auto i : lnt)
    {
        if(isNeighbor(orientation::Left, top_tile, i)) i->tr = top_tile;
    }
    for(auto i : rnt)
    {
        if(isNeighbor(orientation::Right, top_tile, i)) i->bl = top_tile;
    }
    for(auto i : bnt)
    {
        if(isNeighbor(orientation::Bottom, top_tile, i)) i->rt = top_tile;
    }
    delete bot_tile;
    return;
}
bool Layout::tileCreation(coordinate ul_coor, int desired_width, int desired_height, int index)
{
    Tile* topmost, *bottommost, *start_tile, *last_tile;
    Tile* pre_left, *pre_mid, *pre_right;

    if(!Layout::areaSearch(ul_coor, desired_width, desired_height)) start_tile = Layout::pointFinding(Layout::ll_tile, ul_coor);
    else return false;

    if(ul_coor.y == start_tile->ll_coor.y) // ul_coor 壓在tile上 top不需split 但start_tile需要往下找一個
    {
        topmost = start_tile;
        start_tile = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - 1)});
    }
    else // top split
    {
        topmost = split(this, start_tile, ul_coor.y - start_tile->ll_coor.y);
    }

    bottommost = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - desired_height)});
    if(bottommost->ll_coor.y == ul_coor.y - desired_height) // ll_coor 壓在tile上 bot不需split 但bottommost需往下找一個
    {
        last_tile = bottommost;
        bottommost = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - desired_height - 1)});
    }
    else // bot split
    {
        last_tile = split(this, bottommost, (ul_coor.y - desired_height) - bottommost->ll_coor.y);
    }

    //Enumerate all space tile in desired area
    std::vector<Tile*> space_tile;
    start_tile = Layout::pointFinding(Layout::ll_tile, {coordinate(ul_coor.x, ul_coor.y - 1)});
    Tile* t_p = start_tile;
    while(t_p != last_tile)
    {
        space_tile.push_back(t_p);
        if(t_p->lb == nullptr) break;
        
        t_p = t_p->lb;
        while(t_p->ll_coor.x + t_p->width <= ul_coor.x) t_p = t_p->tr;
        // while(t_p->ll_coor.x > ul_coor.x || t_p->ll_coor.x + t_p->width < ul_coor.x + desired_width) t_p = t_p->tr;
    }
    space_tile.push_back(last_tile);

    // Divide every space tile into 3 piece
    Tile *left_tile, *mid_tile, *right_tile;
    pre_mid = pre_left = pre_right = topmost;
    
    for(auto s_t : space_tile)
    {
        mid_tile = s_t;
        std::vector<Tile*> lnt = Layout::neighborFinding(mid_tile, orientation::Left); // left neighbors
        std::vector<Tile*> rnt = Layout::neighborFinding(mid_tile, orientation::Right); // right neighbors
        std::vector<Tile*> tnt = Layout::neighborFinding(mid_tile, orientation::Top); // Top neighbors
        std::vector<Tile*> bnt = Layout::neighborFinding(mid_tile, orientation::Bottom); // Bottom neighbors
        
        // 先把自己上排的最右上tile存起來
        pre_right = mid_tile->rt;
        
        std::tie(left_tile, right_tile) = divide(mid_tile, ul_coor, desired_width);
        // auto [left_tile, right_tile] = divide(mid_tile, ul_coor, desired_width);


        // Find your top tile
        pre_left = pre_mid = pre_right;
        while(!isNeighbor(orientation::Bottom, pre_left, left_tile) && pre_left->bl != nullptr) pre_left = pre_left->bl;
        while(!isNeighbor(orientation::Bottom, pre_mid, mid_tile) && pre_mid->bl != nullptr) pre_mid = pre_mid->bl;

        // check whether can merge itself top tile
        if(pre_mid->width == mid_tile->width && pre_mid->ll_coor.x == mid_tile->ll_coor.x && (pre_mid->index < 0 && mid_tile->index < 0)) merge(this, pre_mid, mid_tile);
        else 
        {
            mid_tile->rt = pre_mid;
            pre_mid = mid_tile;
        }
        if(mid_tile != left_tile){
            if(pre_left->width == left_tile->width && pre_left->ll_coor.x == left_tile->ll_coor.x && (pre_left->index < 0 && left_tile->index < 0)) merge(this, pre_left, left_tile);
            else // merge failed 
            {
                left_tile->rt = pre_left;
                pre_left = left_tile;
                pre_left->tr = pre_mid;
            }
            pre_mid->bl = pre_left;
        }
        if(mid_tile != right_tile){
            if(pre_right->width == right_tile->width && pre_right->ll_coor.x == right_tile->ll_coor.x && (pre_right->index < 0 && right_tile->index < 0)) merge(this, pre_right, right_tile);
            else 
            {
                right_tile->rt = pre_right;
                pre_right = right_tile;
                pre_right->bl = pre_mid;
            }
        }
        
        // help correct neighbors' stitch
        for(auto i : lnt)
        {
            if(isNeighbor(orientation::Left, pre_left, i)) i->tr = pre_left;
        }
        for(auto i : rnt)
        {
            if(isNeighbor(orientation::Right, pre_right, i)) i->bl = pre_right;
        }
        for(auto i : tnt)
        {
            if(isNeighbor(orientation::Top, pre_left, i)) i->lb = pre_left;
            if(isNeighbor(orientation::Top, pre_mid, i)) i->lb = pre_mid;
            if(isNeighbor(orientation::Top, pre_right, i)) i->lb = pre_right;
        }
        for(auto i : bnt)
        {
            if(isNeighbor(orientation::Bottom, pre_left, i)) i->rt = pre_left;
            if(isNeighbor(orientation::Bottom, pre_mid, i)) i->rt = pre_mid;
            if(isNeighbor(orientation::Bottom, pre_right, i)) i->rt = pre_right;
        }
        if(s_t == last_tile)
        {
            // 把最底層的鄰居接到正確的位置
            Tile* tmp_p = bottommost;
            // bottommost
            if(tmp_p == nullptr) // last_tile已經在整個layout最底部了
            {
                pre_left->lb = tmp_p;
                pre_mid->lb = tmp_p;
                pre_right->lb = tmp_p;
            }
            else
            {
                if(isNeighbor(orientation::Top, tmp_p, pre_left)) pre_left->lb = tmp_p;
                if(isNeighbor(orientation::Top, tmp_p, pre_mid)) pre_mid->lb = tmp_p;
                if(isNeighbor(orientation::Top, tmp_p, pre_right)) pre_right->lb = tmp_p;
                // traverse right
                while(tmp_p->tr != nullptr && !(tmp_p->ll_coor.x > ul_coor.x + desired_width) ){ 
                    tmp_p = tmp_p->tr;
                    if(pre_right->width == tmp_p->width && pre_right->ll_coor.x == tmp_p->ll_coor.x && (pre_right->index < 0 && tmp_p->index < 0)) 
                    {
                        merge(this, pre_right, tmp_p);
                    }
                    else
                    {
                        if(isNeighbor(orientation::Top, tmp_p, pre_left)) pre_left->lb = tmp_p;
                        if(isNeighbor(orientation::Top, tmp_p, pre_mid)) pre_mid->lb = tmp_p;
                        if(isNeighbor(orientation::Top, tmp_p, pre_right)) pre_right->lb = tmp_p;
                    }
                }
                // traverse left
                tmp_p = bottommost;
                while(tmp_p->bl != nullptr && !(tmp_p->ll_coor.x + tmp_p->width < ul_coor.x)){ 
                    tmp_p = tmp_p->bl;
                    if(pre_left->width == tmp_p->width && pre_left->ll_coor.x == tmp_p->ll_coor.x && (pre_left->index < 0 && tmp_p->index < 0))
                    {
                        merge(this, pre_left, tmp_p);
                    }
                    else
                    {
                        if(isNeighbor(orientation::Top, tmp_p, pre_left)) pre_left->lb = tmp_p;
                        if(isNeighbor(orientation::Top, tmp_p, pre_mid)) pre_mid->lb = tmp_p;
                        if(isNeighbor(orientation::Top, tmp_p, pre_right)) pre_right->lb = tmp_p;
                    }
                }
            }
        }
    }
    pre_mid->index = index;
    return true;
}