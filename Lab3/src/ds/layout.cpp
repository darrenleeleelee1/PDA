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
