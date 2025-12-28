#ifndef PARTICIPANT
#define PARTICIPANT
#include <string>
#include <vector>
#include <unordered_map>
#include "GameEntities.hpp"

struct Player
{
    int FD; 
    std::string Name;   
    int TeamID;    
};

struct Team
{
    int ID;
    std::string Name;
    std::vector<int> Members;
    std::vector<int> Inventory; /*Store Items*/
    std::unordered_map<Resources,int> ResourceQuantity;
};

std::unordered_map<int,Team> Teams;

#endif