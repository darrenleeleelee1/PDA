#include "io.hpp"
int main(int argc, char const *argv[])
{
    io::readChannel(argv[1]);
    // io::writeNets(argv[2]);
    if(argc == 4){
        io::drawNets(argv[1], argv[2], argv[3]);
    }
    return 0;
}
