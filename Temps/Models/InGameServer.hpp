#ifndef IN_GAME
#define IN_GAME
#include <vector>
#include "Participant.hpp"
#include "../../Cores/CoreDefinition.hpp"
using namespace std;
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

struct Building
{
    std::unordered_map<int, Castle> Castles;
    std::unordered_map<int, Spot> Spots;
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

Item* GetItem(Items ItemType)
{
    Item* item = new Item;
    item->ItemType = ItemType;
    switch(ItemType){
        case BALLISTA: 
            item->AttackPoint = 1000; 
            item->Cost = {
                {Resources::Wood,1500},
                {Resources::Stone,0},
                {Resources::Iron,200}
            };
            break;
        case CATAPULT: 
            item->AttackPoint = 3000;
            item->Cost = {
                {Resources::Wood,400},
                {Resources::Stone,1500},
                {Resources::Iron,300}
            };
            break;
        case CANNON: 
            item->AttackPoint = 8000;
            item->Cost = {
                {Resources::Wood,500},
                {Resources::Stone,2500},
                {Resources::Iron,1800}
            };
            break;
        case FENCE: 
            item->DefensePoint = 200; 
            item->Cost = {
                {Resources::Wood,200},
                {Resources::Stone,50},
                {Resources::Iron,0}
            };
            break;
        case WOOD_WALL: 
            item->DefensePoint = 1000;
            item->Cost = {
                {Resources::Wood,1000},
                {Resources::Stone,100},
                {Resources::Iron,100}
            }; 
            break;
        case STONE_WALL: 
            item->DefensePoint = 3000; 
            item->Cost = {
                {Resources::Wood,200},
                {Resources::Stone,1000},
                {Resources::Iron,200}
            };
            break;
        case LEGEND_WALL: 
            item->DefensePoint = 8000; 
            item->Cost = {
                {Resources::Wood,1000},
                {Resources::Stone,2000},
                {Resources::Iron,1000}
            };
            break;
    }
    return item;
}

int ResourceCompare(unordered_map<Resources,int> own, unordered_map<Resources,int> require){
    for (const auto& req : require)
    {
        Resources type = req.first;
        int requiredAmount = req.second;
        auto it = own.find(type);
        if (it == own.end() || it->second < requiredAmount)
            return false;
    }
    return true; 
}

void UpdateResourcesQuantity(unordered_map<Resources,int>& own, unordered_map<Resources,int> cost){
    for (const auto& c : cost)
    {
        Resources type = c.first;
        int requiredAmount = c.second;
        auto it = own.find(type);
        it->second -= requiredAmount;
    }
}

int BuyDefense(Castle* castle, Team* team, Items item_type){
    if(castle->ownerTeamID != team->ID) return 0; /*Chỉ có team chiếm đóng mới được trang bị phòng thủ*/


    Item* item = GetItem(item_type);
    int cmp_res = ResourceCompare(team->ResourceQuantity,item->Cost);
    if(cmp_res == 0){
        delete item;
        return SHOP_EQUIPMENT_F_LACK_RESOURCE;
    }
    else{
        castle->equippedItems.push_back(item_type);
        castle->defensePoints += item->DefensePoint;
        UpdateResourcesQuantity(team->ResourceQuantity,item->Cost);
        delete item;
        return SHOP_EQUIPMENT_S;
    }
}

int BuyWeapon(Team* team, Items item_type){
    Item* item = GetItem(item_type);
    int cmp_res = ResourceCompare(team->ResourceQuantity,item->Cost);
    if(cmp_res == 0){
        delete item;
        return SHOP_EQUIPMENT_F_LACK_RESOURCE;
    }
    else{
        team->Inventory.push_back(item_type);
        UpdateResourcesQuantity(team->ResourceQuantity,item->Cost);
        delete item;
        return SHOP_EQUIPMENT_S;
    }
}


int FindWeapon(Team* team, Items weapon){
    std::vector<int> tmp = team->Inventory;
    for(int i = 0; i < tmp.size(); i++){
        if(tmp[i] == weapon) return i;
    }
    return -1;
}
void RemoveWeapon(vector<int>& Inventory, int index){
    Inventory.erase(Inventory.begin() + index);
}

int AttackCastle(Castle* castle, Team* team, Items weapon){
    
    if(castle->ownerTeamID == team->ID) return -1; /*Tự tấn công bản thân*/

    int find_result_index = FindWeapon(team,weapon);
    if(find_result_index >= 0){
        Item* attack_item = GetItem(weapon);
        if(castle->defensePoints > attack_item->AttackPoint){
            delete attack_item;
            return ATTACK_CASTLE_F_INSUFFICIENT_POWER;
        }
        else if(castle->defensePoints <= attack_item->AttackPoint){
            castle->defensePoints = 0;
            castle->equippedItems;
            RemoveWeapon(team->Inventory,find_result_index);
            castle->ownerTeamID = team->ID;
            /* Change questions ...*/

            delete attack_item;
            return ATTACK_CASTLE_S;
        }
    }
    else{
        return 0; /* Bổ sung mã trả về : Trường hợp không tồn tại vũ khí trong kho đồ*/
    }
}
#endif