#include "./io/input.hpp"


int main(int argc, char** argv)
{
    clock_t st = std::clock(), mt = std::clock();
    if(argc != 5) std::cout << "Error: Usage: ./Lab2 [α value] [input.block name] [input.net name] [output name]" << std::endl;
    
    Layout L(std::stod(argv[1]));
    io::readBlock(L, argv[2]);
    io::readNet(L, argv[3]);
    L.initLayout();
    // std::srand(std::time(NULL));
    while(L.opt_sp == nullptr){
        L.sp->randomSP(); 
        L.SA(4e8, 1e6, 0.85, 500, 50, L.alpha); // T, T_frozen, r, k, num_nei, alpha_ratio
    }
    io::outMetric(L, argv[4], st);
    io::outDraw(L, argv[4]);

    return 0;
}