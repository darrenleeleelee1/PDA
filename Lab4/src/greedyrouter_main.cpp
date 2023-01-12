#include "greedyrouter.hpp"
#include "io.hpp"

void GreedyRouter::drawForDebug(int column, int whichColumnStart){
    if(column >= whichColumnStart){
        std::string t = "./case/case1.txt";
        std::string k = "./out/case1_" + std::to_string(column) + ".txt";
        std::string j = "./drawing/case1_" + std::to_string(column);
        io::writeNets(this->channel, k.c_str());
        std::string command = "rm ./drawing/case1_" + std::to_string(column) + ".gdt";
        io::drawNets(t.c_str(), k.c_str(), j.c_str());
        system(command.c_str());
    }
}
void GreedyRouter::needMoreColumn(int column)
{
    if(column == this->channel->number_of_columns - 1){
        bool need_more = false;
        for(auto h : this->channel->hor_tracks)
            if(h != 0) need_more = true;
        if(need_more){
            this->channel->number_of_columns++;
            this->channel->top_pins.push_back(0);
            this->channel->bot_pins.push_back(0);
        }
    }
}
void GreedyRouter::main(bool for_small_case)
{
    for(int i = 0; i < this->channel->number_of_columns; i++){
        // save pre horizontal tracks
        this->channel->pre_hor_tracks.assign(this->channel->hor_tracks.begin(), this->channel->hor_tracks.end());
        
        // compute all nets status
        this->computeNetsStatus(i + 1);

        int result_A = this->methodA(i);
        this->methodB(i);
        if(for_small_case) this->methodForSmallCase(i);
        this->methodC(i);
        this->methodD(i);
        this->methodE(result_A, i);
        this->methodF(i);
        this->needMoreColumn(i);
        
        // clear verticle track
        this->channel->clearVerTracks();

        this->drawForDebug(i, INT32_MAX);
        
    }
}