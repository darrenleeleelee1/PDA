#include "io.hpp"
int main(int argc, char const *argv[])
{
    io::readChannel(argv[1]);
    io::writeNets(argv[2]);
    return 0;
}
