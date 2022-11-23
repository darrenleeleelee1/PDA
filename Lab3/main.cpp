#include <iostream>
#include <algorithm>

#include "ds/layout.hpp"
#include "io/input.hpp"
#include "fm/fm.hpp"
#include "lg/lg.hpp"
int main(int argc, char **argv)
{
    Layout L;
    io::readLayout(L, argv[1]);
    // sort cells by their x coordinate for FM partition and Abacus ligalization
    std::sort(L.celllist.begin(), L.celllist.end(), [](Cell *a, Cell *b) -> bool {return a->ll_coor.x < b->ll_coor.x;});
    std::sort(L.gp_result.begin(), L.gp_result.end(), [](Cell *a, Cell *b) -> bool {return a->ll_coor.x < b->ll_coor.x;});
    FM fm(&L);
    fm.main();

    Abacus abacus(&L);
    abacus.main();
    
    // sort cells by their name for lab3 ouput metric
    std::sort(L.celllist.begin(), L.celllist.end(), [](Cell *a, Cell *b) -> bool {return a->name < b->name;});
    io::writeLayout(L, argv[2]);
    return 0;
}
