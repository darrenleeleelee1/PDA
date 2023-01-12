#include <iostream>
#include <algorithm>
#include "ds/layout.hpp"
#include "ds/time.hpp"
#include "io/input.hpp"
#include "fm/fm.hpp"
#include "lg/lg.hpp"
int main(int argc, char **argv)
{
    Timer timer;
    Layout L;
    
    std::cout << "Reading layout.\n"; timer.setShortTerm();
    io::readLayout(L, argv[1]);
    std::cout << "Reading time: " << timer.getShortTerm() << "\n\n";


    // sort cells by their x coordinate for FM partition and Abacus ligalization
    std::cout << "Sorting celllist.\n"; timer.setShortTerm();
    std::sort(L.celllist.begin(), L.celllist.end(), [](Cell *a, Cell *b) -> bool {return a->ll_coor.x < b->ll_coor.x;});
    std::sort(L.gp_result.begin(), L.gp_result.end(), [](Cell *a, Cell *b) -> bool {return a->ll_coor.x < b->ll_coor.x;});
    std::cout << "Sort time: " << timer.getShortTerm() << "\n\n";
    
    
    std::cout << "FM partitioning.\n"; timer.setShortTerm();
    FM fm(&L, 0.5);
    fm.main();
    std::cout << "FM time: " << timer.getShortTerm() << "\n\n";
    

    std::cout << "Abacus legalizationing.\n"; timer.setShortTerm();
    Layout top, bot;
    L.split(top, bot);
    Abacus *abacus_top = new Abacus(&top);
    Abacus *abacus_bot = new Abacus(&bot);
    abacus_top->main();
    abacus_bot->main();
    std::cout << "Abacus time: " << timer.getShortTerm() << "\n\n";
    
    // sort cells by their name for lab3 ouput metric
    std::cout << "Writing layout.\n"; timer.setShortTerm();
    std::sort(L.celllist.begin(), L.celllist.end(), [](Cell *a, Cell *b) -> bool {return a->name < b->name;});
    L.merge(top, bot);
    io::writeLayout(L, argv[2]);
    std::cout << "Writing time: " << timer.getShortTerm() << "\n\n";

    std::cout << "Process time: " << timer.getTime() << "\n\n";
    return 0;
}
