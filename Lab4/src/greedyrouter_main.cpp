#include "greedyrouter.hpp"
#include "io.hpp"
void GreedyRouter::main()
{
    for(int i = 0; i < this->channel->number_of_columns; i++){
        // save pre horizontal tracks
        this->channel->pre_hor_tracks.assign(this->channel->hor_tracks.begin(), this->channel->hor_tracks.end());
        // compute all nets status
        this->computeNetsStatus(i + 1);

        int result_A = this->methodA(i);
        this->methodB(i);
        this->methodC(i);
        this->methodD(i);
        this->methodE(result_A, i);

        // create and save horizontal segments
        for(int j = 1; j <= this->channel->number_of_tracks; j++){
            int now_pin_index = this->channel->hor_tracks.at(j);
            int pre_pin_index = this->channel->pre_hor_tracks.at(j);
            if(pre_pin_index == 0 && now_pin_index != 0){
                this->channel->tmp_hor_segments.at(j) = new HSegments(i, j, i);
            }
            else if(pre_pin_index != now_pin_index){
                this->channel->tmp_hor_segments.at(j)->neighbor = i;
                this->channel->netlist[pre_pin_index]->hor_segments.push_back(this->channel->tmp_hor_segments.at(j));
                this->channel->tmp_hor_segments.at(j) = nullptr;
            }
        }
        // clear verticle track
        this->channel->clearVerTracks();

        // debug
        if(i >= 999){
            std::string t = "./case/case3.txt";
            std::string k = "./out/case3_" + std::to_string(i) + ".txt";
            std::string j = "./drawing/case3_" + std::to_string(i);
            io::writeNets(this->channel, k.c_str());
            std::string command = "rm ./drawing/case3_" + std::to_string(i) + ".gdt";
            io::drawNets(t.c_str(), k.c_str(), j.c_str());
            system(command.c_str());
        }
        // debug

    }
}