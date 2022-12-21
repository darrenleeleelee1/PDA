#pragma once
#include <vector>
#include <map>
#include <set>
enum STATUS{
    rising = 1,
    steady = 0,
    falling = -1
};

struct Segments{
    // vertical    bot x , bot y , top neighbor y
    // horizontal  left x, left y, right neighbor x
    int x, y, neighbor;
};

struct Net
{
    std::set<int> in_tracks;
    STATUS status;
    int last_column;
    int number_of_pins;
    std::vector<Segments*> ver_segments;
    std::vector<Segments*> hor_segments;
    Net(int col): last_column(col), number_of_pins(1){}

    void addPin(int col){
        this->last_column = std::max(this->last_column, col);
        this->number_of_pins++;
    }

    void addVerSegments(){

    }

    void addHorSegments(){
        
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
    Channel(){}

    Channel(int t)
    {
        this->ver_tracks.resize(t+1, 0);
        this->hor_tracks.resize(t+1, 0);
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

    void fillVerTracks(int low, int high, int pin_index);
    void fillHorTracks(int track, int pin_index);
};

