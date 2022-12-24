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
bool checkVerTracks(int low, int high, std::vector<int> &ver_tracks)
{
    if(low > high) std::swap(low, high);
    
    bool ok = true;
    for(int i = low; i <= high; i++){
        if(ver_tracks[i] != 0){
            ok = false;
        }
    }
    
    return ok;
}
int GreedyRouter::methodA(int column)
{
    std::priority_queue<VerticalSegments> pq;
    int top_pin_index = this->channel->top_pins[column];
    int bot_pin_index = this->channel->bot_pins[column];
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
                break;
            }
        }
    }
    if(top_pin_index != 0 && top_pin_index == bot_pin_index && pq.size() == 0){
        if(column >= this->channel->netlist[top_pin_index]->last_column){
            if(checkVerTracks(0, this->channel->number_of_tracks, this->channel->ver_tracks)){
                this->channel->netlist[top_pin_index]->addVerSegments(column, 0, this->channel->number_of_tracks);
                return 4;
            }
        }
    }
    // 0 means no enough tracks for both top and bot
    // 1 means no enough tracks for bot
    // 2 means no enough tracks for top
    // 3 both top and bot success
    int result = 0;
    while(!pq.empty()){
        auto element = pq.top(); pq.pop();
        if(checkVerTracks(element.sp, element.ep, this->channel->ver_tracks)){
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            this->channel->fillHorTracks(element.ep, element.pin_index);
            if(!this->channel->netlist[element.pin_index]->in_tracks.count(element.ep)){
                this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            }
            else{
                if(this->channel->netlist[element.pin_index]->last_column == column){
                    this->channel->netlist[element.pin_index]->in_tracks.erase(element.ep);
                    this->channel->hor_tracks[element.ep] = 0;
                }
            }            
            this->channel->netlist[element.pin_index]->addVerSegments(column, element.sp, element.ep);
            if(top_pin_index == element.pin_index) result += 1;
            else result += 2;
        }
    }

    return result;
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

void GreedyRouter::methodE(int result_A, int column)
{
    // bot not enough tracks
    if(result_A == 0 || result_A == 1){
        int pin_index = this->channel->bot_pins[column];
        if(pin_index != 0){
            for(int i = static_cast<int>((this->channel->number_of_tracks + 1) / 2); i >= 1; i--){
                if(checkVerTracks(0, i-1, this->channel->ver_tracks)){
                    this->channel->insertVerTracks(i, pin_index);
                    this->channel->insertHorTracks(i, pin_index);
                    this->channel->number_of_tracks++;
                    this->channel->fillVerTracks(0, i, pin_index);
                    this->channel->fillHorTracks(i, pin_index);
                    this->channel->refineOldSegments(i);
                    this->channel->netlist[pin_index]->in_tracks.insert(i);
                    this->channel->netlist[pin_index]->addVerSegments(column, 0, i);
                    break;
                }
            }
        }
    }
    // top not enough tracks
    if(result_A == 0 || result_A == 2){
        int pin_index = this->channel->top_pins[column];
        if(pin_index != 0){
            for(int i = static_cast<int>((this->channel->number_of_tracks) / 2 + 1); i <= this->channel->number_of_tracks; i++){
                if(checkVerTracks(i, this->channel->number_of_tracks + 1, this->channel->ver_tracks)){
                    this->channel->insertVerTracks(i, pin_index);
                    this->channel->insertHorTracks(i, pin_index);
                    this->channel->number_of_tracks++;
                    this->channel->fillVerTracks(i, this->channel->number_of_tracks + 1, pin_index);
                    this->channel->fillHorTracks(i, pin_index);
                    this->channel->refineOldSegments(i);
                    this->channel->netlist[pin_index]->in_tracks.insert(i);
                    this->channel->netlist[pin_index]->addVerSegments(column, i, this->channel->number_of_tracks + 1);
                    break;
                }
            }
        }
    }
}
