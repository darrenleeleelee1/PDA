#include "greedyrouter.hpp"
#include <queue>
#include <algorithm>
class VerticalSegments
{
public:
    int pin_index, sp, ep, segments_length;
    VerticalSegments(){}
    VerticalSegments(int p, int _y1, int _y2) : pin_index(p), sp(_y1), ep(_y2) {
        this->segments_length = std::abs(_y1 - _y2);
    }
    bool operator<(const VerticalSegments &other) const{
        return this->segments_length < other.segments_length;
    }
};
class VS_Longest : public VerticalSegments
{
public:
    VS_Longest() : VerticalSegments(){}
    VS_Longest(int p, int _y1, int _y2) : VerticalSegments(p , _y1, _y2){}
    bool operator<(const VS_Longest &other) const{
        return this->segments_length < other.segments_length;
    }
};
class VS_FreeUp : public VerticalSegments
{
public:
    Net *net;
    int column;
    int number_of_free_hor_segments;
    double offset;
    VS_FreeUp() : VerticalSegments(){}
    VS_FreeUp(int p, int c, int o, Net *n, int _y1, int _y2) : VerticalSegments(p , _y1, _y2), column(c){
        this->net = n;
        this->number_of_free_hor_segments = 0;
        this->offset = std::max(static_cast<double>(o) / 2 - this->ep, this->ep - static_cast<double>(o) / 2);
        for(int i = std::min(this->sp, this->ep); i <= std::max(this->sp, this->ep); i++){
            if(this->net->in_tracks.count(i)) this->number_of_free_hor_segments++;
        }
        if(column < this->net->last_column || this->number_of_free_hor_segments < static_cast<int>(this->net->in_tracks.size())){
            this->number_of_free_hor_segments--;
        }
    }
    bool operator<(const VS_FreeUp &other) const{
        if(this->number_of_free_hor_segments == other.number_of_free_hor_segments){
            if(this->offset == other.offset) return this->segments_length < other.segments_length;
            return this->offset < other.offset;
        }
        return this->number_of_free_hor_segments < other.number_of_free_hor_segments;
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
        // compute all top, bot number
        // int test = 0;
        // int boundry = std::min(sp + this->steady_net_constant, n.second->last_column);
        // for(int i = sp; i <= boundry; i++){
        //     if(this->channel->top_pins.at(i) == n.first) test++;
        //     else if(this->channel->bot_pins.at(i) == n.first) test--;
        // }
        // if(test == 0) n.second->status = STATUS::steady;
        // else if(test > 0) n.second->status = STATUS::rising;
        // else n.second->status = STATUS::falling;

        // count steady if exist in top bot, no matter the number
        bool top = false, bot = false;
        int boundry = std::min(sp + this->steady_net_constant, n.second->last_column);
        for(int i = sp; i <= boundry; i++){
            if(this->channel->top_pins.at(i) == n.first) top = true;
            else if(this->channel->bot_pins.at(i) == n.first) bot = true;
        }
        n.second->status = STATUS::steady;
        if(top && !bot) n.second->status = STATUS::rising;
        else if(!top && bot) n.second->status = STATUS::falling;
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
    std::priority_queue<VS_FreeUp> pq;
    
    for(int i = 1; i <= this->channel->number_of_tracks; i++){
        int pin_index = this->channel->hor_tracks.at(i);
        if(pin_index != 0 && this->channel->netlist[pin_index]->in_tracks.size() > 1){
            std::vector<int> in_track_vector(this->channel->netlist[pin_index]->in_tracks.begin(), this->channel->netlist[pin_index]->in_tracks.end());
            for(int t = 0; t < static_cast<int>(in_track_vector.size()); t++){
                for(int p = t + 1; p < static_cast<int>(in_track_vector.size()); p++){
                    if(t != p){
                        pq.emplace(pin_index, column, this->channel->number_of_tracks
                        , this->channel->netlist[pin_index], in_track_vector.at(t), in_track_vector.at(p));
                    }
                }    
            }
        }
    }

    // TODO pop out all pq element, check verticle segment can be connected or not
    // If can, connect and record it
    while(!pq.empty()){
        auto element = pq.top(); pq.pop();

        if(checkVerTracks(element.sp, element.ep, element.pin_index, this->channel->ver_tracks)){
            this->channel->fillVerTracks(element.sp, element.ep, element.pin_index);
            this->channel->fillHorTracks(element.ep, element.pin_index);

            // If not the last column, keep going
            if(!this->channel->netlist[element.pin_index]->in_tracks.count(element.ep)){
                this->channel->netlist[element.pin_index]->in_tracks.insert(element.ep);
            }
            else{
                unsigned int number_to_free = 0;
                for(int i = std::min(element.sp, element.ep); i <= std::max(element.sp, element.ep); i++){
                    if(this->channel->netlist[element.pin_index]->in_tracks.count(i)) number_to_free++;
                }
                if(this->channel->netlist[element.pin_index]->last_column <= column && this->channel->netlist[element.pin_index]->in_tracks.size() <= number_to_free){
                    this->channel->netlist[element.pin_index]->in_tracks.erase(element.ep);
                    this->channel->hor_tracks.at(element.ep) = 0;
                }
            }
            // The other split nets need to be close
            std::unordered_set<int> tmp_s;
            for(int i = std::min(element.sp, element.ep); i <= std::max(element.sp, element.ep); i++){
                if(i == element.ep) continue;
                if(this->channel->netlist[element.pin_index]->in_tracks.count(i)){
                    tmp_s.insert(i);
                    this->channel->hor_tracks.at(i) = 0;
                }
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
    std::priority_queue<VS_Longest> pq;
    
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
    std::priority_queue<VS_Longest> pq;
    
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
            else if(this->channel->netlist[pin_index]->status == falling){
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