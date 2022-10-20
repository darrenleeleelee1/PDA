#include "./io/input.hpp"
#include "./io/output.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if(argc != 5) std::cout << "Error: Usage: ./Lab2 [α value] [input.block name] [input.net name] [output name]" << std::endl;
    
    Metric M(std::stod(argv[1]));
    io::readBlock(argv[2]);
    // readNet();
}