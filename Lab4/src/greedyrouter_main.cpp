#include "greedyrouter.hpp"
void GreedyRouter::main()
{
    for(int i = 0; i < this->channel->number_of_columns; i++){
        this->channel->ver_tracks.assign(this->channel->number_of_tracks + 1, 0);
    }
}