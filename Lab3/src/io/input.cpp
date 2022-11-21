#include "./io/input.hpp"
#include <fstream>
#include <sstream>
#include <cstring>
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

void io::readLayout(Layout &L, char* file_path)
{
    std::ifstream in_file(file_path);
    std::string line;
    std::vector<std::string> tokens;
    while(getline(in_file, line)){
        tokenLine(tokens, line);
        if(tokens[0] == "DieSize"){
            L.width = stoi(tokens[1]); L.height = stoi(tokens[2]);
        }
        if(tokens[0] == "DieRows"){
            L.row_hei = stoi(tokens[1]); L.num_of_row = stoi(tokens[2]);
        }
        if(tokens[0] == "Terminal"){
            L.num_of_ter = stoi(tokens[1]);
            for(int i = 0; i < L.num_of_ter; i++){
                getline(in_file, line);
                tokenLine(tokens, line);
                L.addTerminal(tokens[0], {Coordinate(stoi(tokens[1]), stoi(tokens[2]))}, stoi(tokens[3]), stoi(tokens[4]));
            }
        }
        if(tokens[0] == "NumCell"){
            L.num_of_cell = stoi(tokens[1]);
            for(int i = 0; i < L.num_of_cell; i++){
                getline(in_file, line);
                tokenLine(tokens, line);
                L.addCell(tokens[0], {Coordinate(stoi(tokens[1]), stoi(tokens[2]))}, stoi(tokens[3]), stoi(tokens[4]), i % 2);
            }
        }
    }
}
void io::writeLayout(Layout &L, char* file_path)
{
    std::ofstream out_file(file_path, std::ofstream::trunc);
    for(int i = 0; i < L.num_of_cell; i++){
        out_file << L.celllist[i]->name << " " << L.celllist[i]->ll_coor.x << " " << L.celllist[i]->ll_coor.y;
        out_file << " " << L.celllist[i]->is_top << std::endl;
    }
}