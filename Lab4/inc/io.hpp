#pragma once
#include "ds.hpp"
namespace io
{
    bool verifyResults(char const *input_path, char const *result_path, int &used_tracks, int verbose);
    void readChannel(Channel *channel, char const *file_path);
    void writeNets(Channel *channel, char const *file_path);
    void drawNets(char const *input_path, char const *result_path, char const *output_path);
}