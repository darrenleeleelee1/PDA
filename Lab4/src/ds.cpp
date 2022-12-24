#include "ds.hpp"
#include <algorithm>
#include <iostream>
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
    this->pre_hor_tracks.insert(this->pre_hor_tracks.begin() + track, 0);
    this->tmp_hor_segments.insert(this->tmp_hor_segments.begin() + track, nullptr);
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
        std::unordered_set<int> increase_set;
        for(auto j : i.second->in_tracks){
            if(j >= k){
                i.second->in_tracks.erase(j);
                increase_set.insert(j + 1);
            }
        }
        for(auto j : increase_set){
            i.second->in_tracks.insert(j);
        }
    }
    for(auto i : this->tmp_hor_segments){
        if(i == nullptr) continue;
        if(i->y >= k){
            i->y += 1;
        }
    }
}