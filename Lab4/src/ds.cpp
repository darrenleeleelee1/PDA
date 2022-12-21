#include "ds.hpp"
#include <algorithm>

void Channel::fillVerTracks(int low, int high, int pin_index){
    if(low > high) std::swap(low, high);
    std::fill(this->ver_tracks.begin() + low, this->ver_tracks.begin() + high + 1, pin_index);
}

void Channel::fillHorTracks(int track, int pin_index){
    this->hor_tracks.at(track) = pin_index;
}