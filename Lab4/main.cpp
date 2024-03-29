#include "io.hpp"
#include "timer.hpp"
#include "greedyrouter.hpp"
#include <iostream>
#include <utility>
int main(int argc, char const *argv[])
{
    // usage: ./Lab4 <input> <output> [drawing]
    // eg: ./Lab4 ./case/case1.txt ./out/case1.txt ./drawing/case1
    
    Timer timer;

    GreedyRouter *gr = new GreedyRouter(20, 1, 0);
    std::cout << "Read Channel\n"; timer.setShortTerm();
    io::readChannel(gr->channel, argv[1]);
    std::cout << "Read time: " << timer.getShortTerm() << "\n";
    
    std::cout << "Routing\n"; timer.setShortTerm();
    if(gr->channel->number_of_columns <= 15) gr->main(true);
    else gr->main();
    std::cout << "Routing time: " << timer.getShortTerm() << "\n";

    std::cout << "Write Nets\n"; timer.setShortTerm();
    io::writeNets(gr->channel, argv[2]);
    std::cout << "Write time: " << timer.getShortTerm() << "\n";

    if(argc == 4){
        std::cout << "Draw Nets\n"; timer.setShortTerm();
        io::drawNets(argv[1], argv[2], argv[3]);
        std::cout << "Draw time: " << timer.getShortTerm() << "\n";
        
        std::cout << "Verify Results\n"; timer.setShortTerm();
        int used_tracks;
        io::verifyResults(argv[1], argv[2], used_tracks, 1);
        std::cout << "Verify time: " << timer.getShortTerm() << "\n";
    }

    std::cout << "Tottal time: " << timer.getAndReset() << "\n";
    return 0;
}
