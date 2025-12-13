#ifndef SERVER_INCLUDING
#define SERVER_INCLUDING

mutex ClientsMutex;
unordered_map<int, int> Clients;
int RoomLeader = 0;

int GetValueByKey(unordered_map<int,int>& m, int v)
{
    for (auto& p : m)
    {
        if (p.second == v) return p.first;
    }
    return -1;
}

#include "ServerNetwork.hpp"

#include "Models/Entities/AccountEntity.hpp"
#include "Models/Records/AccountRecord.hpp"

unordered_map<int, AccountEntity> Accounts;

#include "Models/Entities/LobbyEntity.hpp"

#include "Handlers/Handlers/AccountHandler.hpp"
#include "Handlers/Handlers/LobbyHandler.hpp"

#include "Handlers/Phases/WelcomePhase.hpp"
#include "Handlers/Phases/LobbyPhase.hpp"

#endif