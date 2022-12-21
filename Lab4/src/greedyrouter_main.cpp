#include "greedyrouter.hpp"
#include "io.hpp"
void GreedyRouter::main()
{
    for(int i = 0; i < this->channel->number_of_columns; i++){
        int result_A = this->methodA(i);
        this->methodE(result_A, i);
        
        std::string t = "./case/case1.txt";
        std::string k = "./out/case1_" + std::to_string(i) + ".txt";
        std::string j = "./drawing/case1_" + std::to_string(i);
        io::writeNets(this->channel, k.c_str());
        std::string command = "rm ./drawing/case1_" + std::to_string(i) + ".gdt";
        io::drawNets(t.c_str(), k.c_str(), j.c_str());
        system(command.c_str());
        this->channel->clearVerTracks();
    }
}