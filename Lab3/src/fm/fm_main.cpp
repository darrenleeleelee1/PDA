#include "fm/fm.hpp"
void FM::main()
{
    this->initGraph();
    this->initGain();
    this->partition();
    this->popPartition();
}