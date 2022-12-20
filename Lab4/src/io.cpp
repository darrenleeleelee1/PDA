#include "io.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
std::stringstream ss;
void tokenLine(std::vector<std::string> &tokens, std::string line)
{
    ss.clear(); ss.str(line);
    tokens.clear(); tokens.resize(0);
    std::string intermediate;
    while(true){
        ss >> intermediate;
        if(ss.fail()) break;
        tokens.push_back(intermediate);
    }
}

void io::readChannel(char const *file_path)
{
    std::ifstream in_file(file_path);
    std::string line;
    std::vector<std::string> tokens;
    while(getline(in_file, line)){
        tokenLine(tokens, line);
        std::cout << tokens.size() << "\n";
    }

}

void io::writeNets(char const *file_path)
{
    std::ofstream out_file(file_path, std::ofstream::trunc);
    out_file << "tmp\n";
}