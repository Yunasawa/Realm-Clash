#ifndef PARTICIPANT
#define PARTICIPANT
#include <string>
#include <vector>
#include <unordered_map>
enum Resources
{
    Wood,
    Stone,
    Iron,
    Gold
};

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
struct Spot
{
    int spotID;
    int ownerTeamID;
    Resources resourceType; 
};

struct Castle
{
    int castleID; 
    int ownerTeamID = -1; 
    int defensePoints;
    std::vector<int> equippedItems;
};
struct Spot
{
    int spotID;
    int ownerTeamID; 
};

struct Castle
{
    int castleID; 
    int ownerTeamID = -1; 
    int defensePoints;
    std::vector<int> equippedItems;
};

enum Items{
    BALLISTA, 
    CATAPULT,
    CANNON,
    FENCE,
    WOOD_WALL,
    STONE_WALL,
    LEGEND_WALL,
};

struct Item
{
    Items ItemType;
    std::unordered_map<Resources,int> Cost;
    int AttackPoint = 0;
    int DefensePoint = 0;
};
struct Building
{
    std::unordered_map<int, Castle> Castles;
    std::unordered_map<int, Spot> Spots;
};

std::unordered_map<Resources, int> RESOURCE_MINE_AMOUNT = {
    { Resources::Wood,  30 },
    { Resources::Stone, 15 },
    { Resources::Iron,   5 },
    { Resources::Gold,   5 }
};
std::unordered_map<int,Team> Teams;

#endif