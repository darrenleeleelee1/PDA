#include "greedyrouter.hpp"
#include <queue>
#include <algorithm>
struct VerticalSegments
{
    int pin_index, sp, ep, segments_length;
    VerticalSegments(){}
    VerticalSegments(int p, int _y1, int _y2) : pin_index(p), sp(_y1), ep(_y2) {
        this->segments_length = std::abs(_y1 - _y2);
    }
    bool operator<(const VerticalSegments &other) const{
        return segments_length < other.segments_length;
    }
};
bool checkVerTracks(int low, int high, int pin_index, std::vector<int> &ver_tracks)
{
    if(low > high) std::swap(low, high);
    
    bool ok = true;
    int test = 0;
    for(int i = low; i <= high; i++){
        if(ver_tracks[i] == pin_index) test++;
        if(ver_tracks[i] != 0 && ver_tracks[i] != pin_index){
            ok = false;
        }
    }
    if(test == high - low + 1) ok = false;
    return ok;
}
void GreedyRouter::computeNetsStatus(int column)
{
    for(auto n : this->channel->netlist){
        if(n.second->last_column < column) {
            n.second->status = STATUS::steady;
            continue;
        }
        if(n.first == 0) continue;

        int sp = INT32_MAX;
        for(int i = column; i <= n.second->last_column; i++){
            if(this->channel->top_pins.at(i) == n.first || this->channel->bot_pins.at(i) == n.first){
                sp = i;
                break;
            }
        }
        int test = 0;
        int boundry = std::min(sp + this->steady_net_constant, n.second->last_column);
        for(int i = sp; i <= boundry; i++){
            if(this->channel->top_pins.at(i) == n.first) test++;
            else if(this->channel->bot_pins.at(i) == n.first) test--;
        }
        if(test == 0) n.second->status = STATUS::steady;
        else if(test > 0) n.second->status = STATUS::rising;
        else n.second->status = STATUS::falling;
    }
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
            if(checkVerTracks(0, this->channel->number_of_tracks, top_pin_index, this->channel->ver_tracks)){
                this->channel->netlist[top_pin_index]->addVerSegments(column, 0, this->channel->number_of_tracks);
                return 3;
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
        if(checkVerTracks(element.sp, element.ep, element.pin_index, this->channel->ver_tracks)){
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            this->channel->fillHorTracks(element.ep, element.pin_index);
            if(!this->channel->netlist[element.pin_index]->in_tracks.count(element.ep)){
                this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            }
            else{
                if(this->channel->netlist[element.pin_index]->last_column <= column && this->channel->netlist[element.pin_index]->in_tracks.size() == 1){
                    if(top_pin_index != bot_pin_index){
                        this->channel->netlist[element.pin_index]->in_tracks.erase(element.ep);
                        this->channel->hor_tracks.at(element.ep) = 0;
                    }
                }
            }            
            this->channel->netlist[element.pin_index]->addVerSegments(column, element.sp, element.ep);
            if(element.sp != 0) result += 1;
            else result += 2;
        }
    }

    return result;
}

void GreedyRouter::methodB(int column)
{
    std::priority_queue<VerticalSegments> pq;
    
    for(int i = 1; i <= this->channel->number_of_tracks; i++){
        int pin_index = this->channel->hor_tracks.at(i);
        if(pin_index != 0 && this->channel->netlist[pin_index]->in_tracks.size() > 1){
            auto topmost_track = std::max_element(this->channel->netlist[pin_index]->in_tracks.begin()
                                    , this->channel->netlist[pin_index]->in_tracks.end());
            auto botmost_track = std::min_element(this->channel->netlist[pin_index]->in_tracks.begin()
                                    , this->channel->netlist[pin_index]->in_tracks.end());

            // rising pick top one else pick bottom one
            if(this->channel->netlist[pin_index]->status == rising){
                pq.emplace(pin_index, *botmost_track, *topmost_track);
            }
            else{
                pq.emplace(pin_index, *topmost_track, *botmost_track);
            }
        
        }
    }

    // TODO pop out all pq element, check verticle segment can be connected or not
    // If can, connect and record it
    std::unordered_set<int> pop_list;
    while(!pq.empty()){
        auto element = pq.top(); pq.pop();
        
        if(!pop_list.count(element.pin_index) && checkVerTracks(element.sp, element.ep, element.pin_index, this->channel->ver_tracks)){
            pop_list.insert(element.pin_index);
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            this->channel->fillHorTracks(element.ep, element.pin_index);

            // If not the last column, keep going
            if(!this->channel->netlist[element.pin_index]->in_tracks.count(element.ep)){
                this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            }
            else{
                if(this->channel->netlist[element.pin_index]->last_column <= column){
                    this->channel->netlist[element.pin_index]->in_tracks.erase(element.ep);
                    this->channel->hor_tracks.at(element.ep) = 0;
                }
            }
            // The other split nets need to be close
            std::unordered_set<int> tmp_s;
            
            for(auto i : this->channel->netlist[element.pin_index]->in_tracks){
                if(i == element.ep) continue;
                tmp_s.insert(i);
                this->channel->hor_tracks.at(i) = 0;
            }
            for(auto i : tmp_s){
                this->channel->netlist[element.pin_index]->in_tracks.erase(i);
            }
    
            this->channel->netlist[element.pin_index]->addVerSegments(column, element.sp, element.ep);
        }

    }
    
}

void GreedyRouter::methodC(int column)
{
    std::priority_queue<VerticalSegments> pq;
    
    for(int i = 1; i <= this->channel->number_of_tracks; i++){
        int pin_index = this->channel->hor_tracks.at(i);
        if(pin_index != 0 && this->channel->netlist[pin_index]->in_tracks.size() > 1){
            auto topmost_track = std::max_element(this->channel->netlist[pin_index]->in_tracks.begin()
                                    , this->channel->netlist[pin_index]->in_tracks.end());
            auto botmost_track = std::min_element(this->channel->netlist[pin_index]->in_tracks.begin()
                                    , this->channel->netlist[pin_index]->in_tracks.end());
            for(int j = *topmost_track - 1; j > *botmost_track; j--){
                if(this->channel->hor_tracks.at(j) == 0 || this->channel->hor_tracks.at(j) == pin_index){
                    pq.emplace(pin_index, *topmost_track, j);
                }
            }
            
            for(int j = *botmost_track + 1; j < *topmost_track; j++){
                if(this->channel->hor_tracks.at(j) == 0 || this->channel->hor_tracks.at(j) == pin_index){
                    pq.emplace(pin_index, *botmost_track, j);
                }
            }
        }
    }
    while(!pq.empty()){
        auto element = pq.top(); pq.pop();
        // if(element.segments_length < this->minimum_jog_length) break;
        if(checkVerTracks(element.sp, element.ep, element.pin_index, this->channel->ver_tracks)){
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            

            // The other split nets between this verticle need to be close
            std::unordered_set<int> tmp_s;
            
            for(int i = std::min(element.sp, element.ep); i <= std::max(element.sp, element.ep); i++){
                if(this->channel->netlist[element.pin_index]->in_tracks.count(i)){
                    tmp_s.insert(i);
                    this->channel->hor_tracks.at(i) = 0;
                }
            }
            for(auto i : tmp_s){
                this->channel->netlist[element.pin_index]->in_tracks.erase(i);
            }

            this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            this->channel->fillHorTracks(element.ep, element.pin_index);
            this->channel->netlist[element.pin_index]->addVerSegments(column, element.sp, element.ep);
        }
    }
}

void GreedyRouter::methodD(int column)
{
    std::priority_queue<VerticalSegments> pq;
    
    for(int i = 1; i <= this->channel->number_of_tracks; i++){
        int pin_index = this->channel->hor_tracks.at(i);
        if(pin_index != 0 && this->channel->netlist[pin_index]->in_tracks.size() == 1){

            // rising pick top one else pick bottom one
            if(this->channel->netlist[pin_index]->status == rising){
                for(int j = i + 1; j <= this->channel->number_of_tracks; j++){
                    if(this->channel->hor_tracks.at(j) == 0 || this->channel->hor_tracks.at(j) == pin_index){
                        pq.emplace(pin_index, i, j);
                    }
                }
            }
            else{
                for(int j = i - 1; j >= 1; j--){
                    if(this->channel->hor_tracks.at(j) == 0 || this->channel->hor_tracks.at(j) == pin_index){
                        pq.emplace(pin_index, i, j);
                    }
                }
            }
        }
    }
    while(!pq.empty()){
        auto element = pq.top(); pq.pop();
        if(element.segments_length < this->minimum_jog_length) break;
        if(checkVerTracks(element.sp, element.ep, element.pin_index, this->channel->ver_tracks)){
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            this->channel->fillHorTracks(element.ep, element.pin_index);
            this->channel->hor_tracks.at(element.sp) = 0;

            this->channel->netlist[element.pin_index]->in_tracks.erase(element.sp);
            this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            
            this->channel->netlist[element.pin_index]->addVerSegments(column, element.sp, element.ep);
        }
    }
}

void GreedyRouter::methodE(int result_A, int column)
{
    // bot not enough tracks
    if(result_A == 0 || result_A == 1){
        int pin_index = this->channel->bot_pins[column];
        if(pin_index != 0){
            for(int i = static_cast<int>((this->channel->number_of_tracks + 1) / 2); i >= 0; i--){
                if(checkVerTracks(0, i-1, pin_index, this->channel->ver_tracks)){
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
            for(int i = static_cast<int>((this->channel->number_of_tracks) / 2 + 1); i <= this->channel->number_of_tracks + 1; i++){
                if(checkVerTracks(i, this->channel->number_of_tracks + 1, pin_index, this->channel->ver_tracks)){
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
