#include "./io/input.hpp"


int main(int argc, char** argv)
{
    clock_t st = std::clock();
    if(argc != 5) std::cout << "Error: Usage: ./Lab2 [α value] [input.block name] [input.net name] [output name]" << std::endl;
    
    Layout L(std::stod(argv[1]));
    io::readBlock(L, argv[2]);
    io::readNet(L, argv[3]);
    L.initLayout();
    L.SA();
    io::outMetric(L, argv[4], st);
    io::outDraw(L, argv[4]);
    return 0;
}