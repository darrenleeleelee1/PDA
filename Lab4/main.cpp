#include "io.hpp"
#include "timer.hpp"
#include "greedyrouter.hpp"
#include <iostream>
int main(int argc, char const *argv[])
{
    Timer timer;
    GreedyRouter *gr = new GreedyRouter(4, 10, 10);
    std::cout << "Read Channel\n"; timer.setShortTerm();
    io::readChannel(gr->channel, argv[1]);
    std::cout << "Read time: " << timer.getShortTerm() << "\n";
    
    std::cout << "Routing\n"; timer.setShortTerm();
    gr->main();
    std::cout << "Routing time: " << timer.getShortTerm() << "\n";

    std::cout << "Write Nets\n"; timer.setShortTerm();
    io::writeNets(gr->channel, argv[2]);
    std::cout << "Write time: " << timer.getShortTerm() << "\n";

    if(argc == 4){
        std::cout << "Draw Nets\n"; timer.setShortTerm();
        io::drawNets(argv[1], argv[2], argv[3]);
        std::cout << "Draw time: " << timer.getShortTerm() << "\n";

    }

    std::cout << "Tottal time: " << timer.getAndReset() << "\n";
    return 0;
}
