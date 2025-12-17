#ifndef IN_GAME
#define IN_GAME
#include <vector>
#include "Participant.hpp"
#include "../../Cores/CoreDefinition.hpp"
#include <unordered_map>
using namespace std;
Building CreateMap()
{
    auto building = Building();
    building.Castles[1] = Castle{1, -1, 0, {}};
    building.Castles[2] = Castle{2, -1, 0, {}};
    building.Castles[3] = Castle{3, -1, 0, {}};
    building.Spots[1] = Spot{1, -1};
    building.Spots[2] = Spot{2, -1};
    building.Spots[3] = Spot{3, -1};
    building.Spots[4] = Spot{4, -1};
    building.Spots[5] = Spot{5, -1};
    building.Spots[6] = Spot{6, -1};
    return building;
}

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
void SendMsg(Team* team, const string &msg)
{
    for (int memberFD : team->Members)
    {
        SendMessage(memberFD, msg);
    }
}
Building CreateMap()
{
    auto building = Building();
    building.Castles[1] = Castle{1, -1, 0, {}};
    building.Castles[2] = Castle{2, -1, 0, {}};
    building.Castles[3] = Castle{3, -1, 0, {}};
    for(int i = 0; i < 3; i++){
        for (int j = 0;j < 6;j++){
            building.Spots[j] = Spot{j, -1, static_cast<Resources>(i)};
        }
    }
    return building;
}

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
            SendMsg(team, string(RS_SHOP_EQUIPMENT_F_LACK_RESOURCE));
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

void AddResourcesQuantity(unordered_map<Resources,int>& own, Resources resourceType, int amount){
    auto it = own.find(resourceType);
    it->second += amount;
}
int BuyDefense(Castle* castle, Team* team, Items item_type){
    if(castle->ownerTeamID != team->ID) return 0; /*Chỉ có team chiếm đóng mới được trang bị phòng thủ*/


    Item* item = GetItem(item_type);
    int cmp_res = ResourceCompare(team->ResourceQuantity,item->Cost);
    if(cmp_res == 0){
        delete item;
        SendMsg(team, string(RS_SHOP_EQUIPMENT_F_LACK_RESOURCE));
    }
    else{
        castle->equippedItems.push_back(item_type);
        castle->defensePoints += item->DefensePoint;
        UpdateResourcesQuantity(team->ResourceQuantity,item->Cost);
        delete item;
        SendMsg(team, string(RS_SHOP_EQUIPMENT_S));
    }
}
int BuyWeapon(Team* team, Items item_type){
    Item* item = GetItem(item_type);
    int cmp_res = ResourceCompare(team->ResourceQuantity,item->Cost);
    if(cmp_res == 0){
        delete item;
        SendMsg(team,string(RS_SHOP_EQUIPMENT_F_LACK_RESOURCE));
    }
    else{
        team->Inventory.push_back(item_type);
        UpdateResourcesQuantity(team->ResourceQuantity,item->Cost);
        delete item;
        SendMsg(team,string(RS_SHOP_EQUIPMENT_S));
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
    
    if(castle->ownerTeamID == team->ID) return RS_ATTACK_CASTLE_F_SELF_ATTACK; /*Tự tấn công bản thân*/

    int find_result_index = FindWeapon(team,weapon);
    if(find_result_index >= 0){
        Item* attack_item = GetItem(weapon);
        if(castle->defensePoints > attack_item->AttackPoint){
            delete attack_item;
            SendMsg(team,string(RS_ATTACK_CASTLE_F_NOT_ENOUGH_ATTACK_POWER));
        }
        else if(castle->defensePoints <= attack_item->AttackPoint){
            castle->defensePoints = 0;
            castle->equippedItems;
            RemoveWeapon(team->Inventory,find_result_index);
            castle->ownerTeamID = team->ID;
            /* Change questions ...*/

            delete attack_item;
            SendMsg(team,string(RS_ATTACK_CASTLE_S));
        }
    }
    else{
        SendMsg(team,string(RS_ATTACK_CASTLE_F_NO_WEAPON));
    }
}
void MineResourceFromSpot(Spot* spot, Team* team, Resources resourceType)
{
    if (spot->ownerTeamID != team->ID)
       SendMsg(team,string(RS_REQUEST_QUESTION_F_SLOT_OCCUPIED));
    auto mineIt = RESOURCE_MINE_AMOUNT.find(resourceType);
    if (mineIt == RESOURCE_MINE_AMOUNT.end())
        SendMsg(team,string(RS_GIVE_RESOURCE_F)) ;

    int amount = mineIt->second;

    auto it = team->ResourceQuantity.find(resourceType);
    if (it == team->ResourceQuantity.end())
    {
        SendMsg(team,string(RS_GIVE_RESOURCE_F));
    }
    else
    {
        it->second += amount;
    }

    SendMsg(team, string(RS_GIVE_RESOURCE_S));
}
#endif