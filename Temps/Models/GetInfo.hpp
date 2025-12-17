#ifndef PART
#define PART
using namespace std;
#include <iostream>
#include "GameEntities.hpp"
#include "ParticipantServer.hpp"
#include "Participant.hpp"
#include <sstream>
#include <unordered_map>
#include <bits/stdc++.h>
#include "../../Servers/ServerIncluding.hpp"

extern unordered_map<int,Team> Teams;
#include "InGameServer.hpp"
#include "Participant.hpp"
#include <sstream>
#include <unordered_map>
#include <bits/stdc++.h>
#include "../../Servers/ServerIncluding.hpp"

extern unordered_map<int,Team> Teams;
extern unordered_map<int,int> Clients; // Map clientFD to accountID
void GetCastleInfo(Building b, int castleId)
{
    Castle tmp = b.Castles[castleId];
    if(tmp.ownerTeamID != -1){
        auto it = Teams.find(tmp.ownerTeamID);
        string ownerName = (it->second).Name;
        int defensePoints = tmp.defensePoints;
        cout << "Castle " << castleId << " is occupied by " << "Team " << ownerName << "!" << endl;
        cout << "Defense point is " << defensePoints << " !" << endl;
    }
    else{
        cout << "Castle " << castleId << "is not be occupied by any teams !" << endl;
    }
}

void GetTeamResourceInfo(int teamId)
{
    auto it = Teams.find(teamId);
    Team tmp = it->second;
    if (it == Teams.end()){
        cout << "Invalid team !" << endl;
    }
    else{
        cout << "Team " << tmp.Name << " has " << (tmp.ResourceQuantity.find(Resources::Wood))->second << " wood unit(s) !" << endl;
        cout << "Team " << tmp.Name << " has " << (tmp.ResourceQuantity.find(Resources::Stone))->second << " stone unit(s) !" << endl;
        cout << "Team " << tmp.Name << " has " << (tmp.ResourceQuantity.find(Resources::Iron))->second << " iron unit(s) !" << endl;
        cout << "Team " << tmp.Name << " has " << (tmp.ResourceQuantity.find(Resources::Gold))->second << " gold unit(s) !" << endl;
    }
}
int GetTeamResult(int teamId)
{
    auto it = Teams.find(teamId);
    Team tmp = it->second;
    if (it == Teams.end()){
        return -1;
    }
    int wood = tmp.ResourceQuantity.find(Resources::Wood)->second;
    int stone = tmp.ResourceQuantity.find(Resources::Stone)->second;
    int iron = tmp.ResourceQuantity.find(Resources::Iron)->second;
    int gold = tmp.ResourceQuantity.find(Resources::Gold)->second;
    int total = wood + 2*stone + 3*iron + 10*gold;
    return total;
}
void GameResult()
{
    int maxPoint = -1;
    int winnerTeamId = -1;
    for (const auto& pair : Teams)
    {
        int teamId = pair.first;
        int teamPoint = GetTeamResult(teamId);
        if (teamPoint > maxPoint)
        {
            maxPoint = teamPoint;
            winnerTeamId = teamId;
        }
    }
    auto it = Teams.find(winnerTeamId);
    stringstream ss;
    ss << "Team " << (it->second).Name << " has " << maxPoint << " point(s) !";
    for(auto client : Clients){
        stringstream ss;
        SendMessage(client.first, ss.str());
    };
}
#endif