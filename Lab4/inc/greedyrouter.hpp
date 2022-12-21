#include "ds.hpp"
struct GreedyRouter
{
    Channel* channel;
    int initial_channel_width;
    int minimum_jog_length;
    int steady_net_constant;
    GreedyRouter(){}
    GreedyRouter(int i, int m, int s)
        : initial_channel_width(i), minimum_jog_length(m), steady_net_constant(s)
    {
        this->channel = new Channel(i);
    }
    void main();
    void methodA(int col);
    void methodB();
    void methodC();
    void methodD();
    void methodE();
    
};