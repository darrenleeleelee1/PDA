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
void io::readBlock(Layout &L, char* input_path)
{
    std::ifstream fin(input_path);
    std::vector<std::string> tokens; 
    std::string line;
    while(getline(fin, line)){
        tokenLine(tokens, line);
        if(tokens[0] == "Outline:"){
            L.width = stoi(tokens[1]);
            L.height = stoi(tokens[2]);
            continue;
        }
        else if(tokens[0] == "NumBlocks:"){
            L.num_blocks = stoi(tokens[1]);
            continue;
        }
        else if(tokens[0] == "NumTerminals:"){
            L.num_ters = stoi(tokens[1]);
            continue;
        }
        else if(tokens.size() == 3){
            L.addBlock(tokens[0], stoi(tokens[1]), stoi(tokens[2]));
        }
        else if(tokens.size() == 4){
            L.addTer(tokens[0], stoi(tokens[2]), stoi(tokens[3]));
        }
    }
}

void io::readNet(Layout &L, char* input_path)
{
    std::ifstream fin(input_path);
    std::vector<std::string> tokens; 
    std::string line;
    Net* n = nullptr;
    while(getline(fin, line)){
        tokenLine(tokens, line);
        if(tokens[0] == "NumNets:"){
            L.num_nets = stoi(tokens[1]);
        }
        else if(tokens[0] == "NetDegree:"){
            if(n != nullptr) L.netlist.push_back(*n);
            n = new Net(stoi(tokens[1]));
        }
        else if(tokens.size() == 1){
            n->net_names.push_back(tokens[0]);
        }
    }
    if(n != nullptr && n->size != 0) L.netlist.push_back(*n);
}

void io::outMetric(Layout &L, char* output_path, clock_t st)
{
    std::ofstream fout(output_path, std::ofstream::trunc);
    fout << L.opt_sp->countCost(&L, L.alpha, false) << std::endl;
    fout << L.opt_sp->countHPWL(&L) << std::endl;
    fout << L.chip_width * L.chip_height << std::endl;
    fout << L.chip_width << " " << L.chip_height << std::endl;
    clock_t et = std::clock();
    fout << ((et - st) / CLOCKS_PER_SEC) << std::endl;
    for(int i = 0; i < L.num_blocks; i++){
        fout << L.opt_sp->blocklist[i].block_name << " " << L.opt_sp->P[i].x << " " << L.opt_sp->P[i].y << " ";
        fout << L.opt_sp->P[i].x + L.opt_sp->blocklist[i].borders.width << " " << L.opt_sp->P[i].y + L.opt_sp->blocklist[i].borders.height << std::endl;
    }
}
void io::outDraw(Layout &L, char *output_path)
{
    char null[100] = {'\0'};
    char text_file_for_draw[100];

    // Tokenize the last token
    const char *delimiters = "/";
    char* tmp_str = (char*)output_path;
    char *token = strtok((char*)tmp_str, delimiters);
    char *last_token = nullptr;
    while (token != nullptr) {
        last_token = token;
        token = strtok(nullptr, delimiters);
    }

    std::strcpy(text_file_for_draw, null);
    std::string draw_dir  = "./drawing/";
    std::strcat(text_file_for_draw, draw_dir.c_str());
    std::strcat(text_file_for_draw, last_token);
    std::ofstream fout(text_file_for_draw, std::ofstream::trunc);
    if(fout.fail()) std::cout << "file create error." << std::endl;
    fout << L.num_blocks << std::endl;
    fout << L.width << " " << L.height << std::endl;
    for(int i = 0; i < L.num_blocks; i++){
        fout << L.opt_sp->blocklist[i].block_name << " " << L.opt_sp->P[i].x << " " << L.opt_sp->P[i].y << " ";
        fout << L.opt_sp->blocklist[i].borders.width << " " << L.opt_sp->blocklist[i].borders.height << std::endl;
    }
    
}