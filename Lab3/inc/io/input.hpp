#pragma once
#include "./ds/layout.hpp"
namespace io
{
    void readLayout(Layout &L, char* file_path);
    void writeLayout(Layout &L, char* file_path);
}
