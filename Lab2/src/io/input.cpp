#include "./io/input.hpp"
#include "./ds/layout.hpp"
#include <fstream>
#include <sstream>
std::stringstream ss;
void readLine(std::vector<std::string> &tokens, std::string line)
{
    ss.clear();
    ss.str(line);
    tokens.clear();
    std::string intermediate;
    while(getline(ss, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }
}
void io::readBlock(char* input_path)
{
    std::ifstream fin(input_path);
    std::vector<std::string> tokens; 
    std::string line;
    while(getline(fin, line))
    {
        readLine(tokens, line);
    }
}