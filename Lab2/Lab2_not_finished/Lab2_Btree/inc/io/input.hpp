#pragma once
#include "./ds/layout.hpp"
namespace io
{
    void readBlock(Layout &L, char* input_path);
    void readNet(Layout &L, char* input_path);

    void outMetric(Layout &L, char* output_path, clock_t st);
    void outDraw(Layout &L, char* output_path);
}
