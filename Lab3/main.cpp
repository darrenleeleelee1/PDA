#include <iostream>
#include "ds/layout.hpp"
#include "io/input.hpp"
#include "fm/fm.hpp"
int main(int argc, char **argv)
{
    Layout L;
    io::readLayout(L, argv[1]);
    io::writeLayout(L, argv[2]);
    
    FM fm(&L);
    fm.main();
    return 0;
}
