#include <cstring>
#include "./io/io.hpp"

int main(int argc, char const *argv[])
{
    Result R;
    readCaseToLayout(R, argv[1]);
    R.Out(outputs::StandardO);
    for(int i = 2; i < argc; i++)
    {
        if(strcmp(argv[i], "-draw") == 0)
        {
            R.Draw(argv[++i]);
        }
    }
    return 0;
}
