#include "greedyrouter.hpp"
#include <queue>
struct VerticalSegments
{
    int pin_index, sp, ep, segments_length;
    VerticalSegments(){}
    VerticalSegments(int p, int _y1, int _y2) : pin_index(p), sp(_y1), ep(_y2) {
        this->segments_length = std::abs(_y1 - _y2);
    }
    bool operator<(const VerticalSegments &other) const{
        return segments_length > other.segments_length;
    }
};
bool checkVerTracks(int low, int high, std::vector<int> &ver_tracks){
    if(low > high) std::swap(low, high);
    
    int ok = 0;
    for(int i = low; i <= high; i++){
        ok &= ver_tracks[i];
    }
    
    return ok == 0;
}
void GreedyRouter::methodA(int col)
{
    std::priority_queue<VerticalSegments> pq;
    int top_pin_index = this->channel->top_pins[col];
    int bot_pin_index = this->channel->bot_pins[col];
    // Top
    if(top_pin_index != 0){
        for(int i = this->channel->number_of_tracks; i >= 1; i--){
            if(this->channel->hor_tracks.at(i) == 0 || this->channel->hor_tracks.at(i) == top_pin_index){
                pq.emplace(top_pin_index, this->channel->number_of_tracks + 1, i);
                break;
            }
        }
    }
    // Bot
    if(bot_pin_index != 0){
        for(int i = 1; i <= this->channel->number_of_tracks; i++){
            if(this->channel->hor_tracks.at(i) == 0 || this->channel->hor_tracks.at(i) == bot_pin_index){
                pq.emplace(bot_pin_index, 0, i);
            }
        }
    }

    while(!pq.empty()){
        auto top_element = pq.top(); pq.pop();
        if(checkVerTracks(top_element.sp, top_element.ep, this->channel->ver_tracks)){
            this->channel->fillVerTracks(top_element.sp, top_element.ep, top_element.pin_index);
            this->channel->fillHorTracks(top_element.ep, top_element.pin_index);
        }
    }
}

void GreedyRouter::methodB()
{
    
}

void GreedyRouter::methodC()
{
    
}

void GreedyRouter::methodD()
{
    
}

void GreedyRouter::methodE()
{
    
}
