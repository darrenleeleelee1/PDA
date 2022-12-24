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
    void computeNetsStatus(int column);
    int methodA(int column);
    void methodB(int column);
    void methodC(int column);
    void methodD(int column);
    void methodE(int result_A, int column);
    
};