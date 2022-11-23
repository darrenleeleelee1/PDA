#include "ds/layout.hpp"
#include <iostream>
void Block::printBlock()
{   
    std::cout << this->name << " " ;
    std::cout << "ll_x: " << this->ll_coor.x << " ll_y: " <<  this->ll_coor.y;
    std::cout << " width: " << this->width << " height: " <<  this->height << std::endl;
}

void Cell::printBlock()
{
    std::cout << this->name << " is_top: " << this->is_top << " ";
    std::cout << "ll_x: " << this->ll_coor.x << " ll_y: " <<  this->ll_coor.y;
    std::cout << " width: " << this->width << " height: " <<  this->height << std::endl;;
}

void Layout::split(Layout &top, Layout &bot)
{
    for(int i = 0; i < this->num_of_cell; i++){
        if(this->celllist[i]->is_top){
            top.celllist.push_back(new Cell(*(this->celllist[i])));
            top.gp_result.push_back(new Cell(*(this->gp_result[i])));
        }
        else{
            bot.celllist.push_back(new Cell(*(this->celllist[i])));
            bot.gp_result.push_back(new Cell(*(this->gp_result[i])));
        }
    }
    /*
    int width, height;
    int row_hei, num_of_row;
    int num_of_ter, num_of_cell;
    */
    for(int i = 0; i < this->num_of_ter; i++){
        top.terlist.push_back(new Terminal(*(this->terlist[i])));
        bot.terlist.push_back(new Terminal(*(this->terlist[i])));
    }
    top.width = this->width; bot.width = this->width;
    top.height = this->height; bot.height = this->height;
    top.row_hei = this->row_hei; bot.row_hei = this->row_hei;
    top.num_of_row = this->num_of_row; bot.num_of_row = this->num_of_row;
    top.num_of_ter = this->num_of_ter; bot.num_of_ter = this->num_of_ter;
    top.num_of_cell = top.celllist.size(); bot.num_of_cell = bot.celllist.size();
}

void Layout::merge(Layout &top, Layout &bot)
{
    for(auto i : top.celllist){
        int idx = i->name;
        this->celllist[idx]->ll_coor.x = i->ll_coor.x;
        this->celllist[idx]->ll_coor.y = i->ll_coor.y;
    }
    for(auto i : bot.celllist){
        int idx = i->name;
        this->celllist[idx]->ll_coor.x = i->ll_coor.x;
        this->celllist[idx]->ll_coor.y = i->ll_coor.y;
    }
}