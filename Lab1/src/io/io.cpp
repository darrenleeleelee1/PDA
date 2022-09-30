#include <sstream>
#include <fstream>
#include <string>
#include "io.hpp"

Layout readCaseToLayout(char const *file_path)
{
    std::ifstream input(file_path);
    std::string line, token;
    std::stringstream ss;
    
    getline(input, line);
    ss.clear(); ss.str(line);
    int tmp_width, tmp_height;
    coordinate tmp_coor;
    ss >> tmp_width >> tmp_height;
    Layout L{Layout{tmp_width, tmp_height}};
    while(getline(input, line))
    {
        ss.clear(); ss.str(line);
        ss >> token;
        if(token == "P") 
        {
            ss >> tmp_coor.x >> tmp_coor.y;
            Tile *t_p = L.pointFinding(L.ll_tile, tmp_coor);
            if(t_p != nullptr) t_p->printTile();
            // ----------------debug----------------
            
            // std::vector<Tile*> nl = L.neighborFinding(t_p, orientation::Top);
            // std::cout << nl.size() << std::endl;

            // bool as_b = L.areaSearch({coordinate(0, 0 + tmp_height - 1)}, tmp_width, tmp_height - 1);
            // std::cout << "Area Searching ul_coor(" << 0 << ", " << 0 + tmp_height - 1 << ") width= " << tmp_width << " height= " << tmp_height - 1 << std::endl;
            // std::cout << (as_b ? "Contain solid tile" : "No solid tile") << std::endl;

            // L.tileCreation({coordinate(30, 30)}, 20, 20, 1);
            // ----------------debug----------------
        }
        else
        {
            ss >> tmp_coor.x >> tmp_coor.y >> tmp_width >> tmp_height;
            // becareful tileCreation is using ul coordinate
            if(L.tileCreation(coordinate{tmp_coor.x, tmp_coor.y + tmp_height}, tmp_width, tmp_height, stoi(token))) std::cout << "#" << token << " is created\n";
            else std::cout << "@" << token << " created failed!\n";
        }
    }
    
    return L;
}