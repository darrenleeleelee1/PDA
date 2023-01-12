#include <cstring>
#include "./io/io.hpp"

int main(int argc, char const *argv[])
{
    Result R;
    if(argc == 3) // for homework benchmark
    {
        readCaseToLayout(R, argv[1]);
        R.Out(Outputs::Files, argv[2]);
    }
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], "-input") == 0)
            {
                readCaseToLayout(R, argv[++i]);
            }
            if(strcmp(argv[i], "-draw") == 0)
            {
                R.Draw(argv[++i]);
            }
            if(strcmp(argv[i], "-output") == 0)
            {
                // R.Out(outputs::StandardO, argv[++i]);
                R.Out(Outputs::Files, argv[++i]);
            }
        }
    }
    

    
    return 0;
}
