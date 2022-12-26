#pragma once
#include <vector>
#include <map>
#include <unordered_set>
enum STATUS{
    rising = 1,
    steady = 0,
    falling = -1
};

struct VSegments{
    // vertical    bot x , bot y , top neighbor y
    int x, y, neighbor;
    VSegments(int _x, int _y, int _n) : x(_x){
        this->y = _y;
        this->neighbor = _n;
        if(this->y > this->neighbor) std::swap(this->y, this->neighbor);
    }
};
struct HSegments{
    // horizontal  left x, left y, right neighbor x
    int x, y, neighbor;
    HSegments(int _x, int _y, int _n) : y(_y){
        this->x = _x;
        this->neighbor = _n;
        if(this->x > this->neighbor) std::swap(this->x, this->neighbor);
    }
};

struct Net
{
    std::unordered_set<int> in_tracks;
    STATUS status;
    int last_column;
    int number_of_pins;
    std::vector<VSegments*> ver_segments;
    std::vector<HSegments*> hor_segments;
    Net(int col): last_column(col), number_of_pins(1){
        this->status = STATUS::steady;
    }

    void addPin(int col){
        this->last_column = std::max(this->last_column, col);
        this->number_of_pins++;
    }

    void addVerSegments(int x, int y, int neighbor){
        this->ver_segments.push_back(new VSegments(x, y, neighbor));
    }
    
    void addHorSegments(int x, int y, int neighbor){
    }
};

struct Channel
{
    std::vector<int> top_pins;
    std::vector<int> bot_pins;
    int number_of_columns;
    int number_of_tracks;
    std::map<int, Net*> netlist;
    std::vector<int> ver_tracks;
    std::vector<int> hor_tracks;
    std::vector<int> pre_hor_tracks;
    std::vector<HSegments*> tmp_hor_segments;
    Channel(){}

    Channel(int t)
    {
        this->ver_tracks.resize(t+2, 0);
        this->hor_tracks.resize(t+2, 0);
        this->pre_hor_tracks.resize(t+2, 0);
        this->tmp_hor_segments.resize(t+2, nullptr);
        this->number_of_columns = 0;
        this->number_of_tracks = t;
    }

    void addPins(int src, bool top, int column){
        if(top) this->top_pins.push_back(src);
        else this->bot_pins.push_back(src);
        if(!this->netlist.count(src)){
            this->netlist[src] = new Net(column);
        }
        else{
            this->netlist[src]->addPin(column);
        }
    }
    void clearVerTracks();
    void fillVerTracks(int low, int high, int pin_index);
    void fillHorTracks(int track, int pin_index);
    void insertVerTracks(int track, int pin_index);
    void insertHorTracks(int track, int pin_index);
    void refineOldSegments(int k);
};

