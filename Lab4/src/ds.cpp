#include "ds.hpp"
#include <algorithm>
void Channel::clearVerTracks()
{
    std::fill(this->ver_tracks.begin(), this->ver_tracks.end(), 0);
}

void Channel::fillVerTracks(int low, int high, int pin_index)
{
    if(low > high) std::swap(low, high);
    std::fill(this->ver_tracks.begin() + low, this->ver_tracks.begin() + high + 1, pin_index);
}

void Channel::fillHorTracks(int track, int pin_index)
{
    this->hor_tracks.at(track) = pin_index;
}

void Channel::insertVerTracks(int track, int pin_index)
{
    this->ver_tracks.insert(this->ver_tracks.begin() + track, pin_index);
}

void Channel::insertHorTracks(int track, int pin_index)
{
    this->hor_tracks.insert(this->hor_tracks.begin() + track, pin_index);
}

void Channel::refineOldSegments(int k)
{
    for(auto i : this->netlist){
        for(auto j : i.second->ver_segments){
            if(j->y >= k){
                j->y += 1;
            }
            if(j->neighbor >= k){
                j->neighbor += 1;
            }
        }
        for(auto j : i.second->hor_segments){
            if(j->y >= k){
                j->y += 1;
            }
        }
    }
}