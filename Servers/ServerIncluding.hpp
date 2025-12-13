#ifndef SERVER_INCLUDING
#define SERVER_INCLUDING

mutex ClientsMutex;
mutex SessionsMutex;
ofstream LogFile;

unordered_map<int, int> Clients; // Map clientFD to accountID
vector<int> JoinedMembers; // List of accountIDs to determine the order of members joined to game
int RoomLeader = 0;

int GetValueByKey(unordered_map<int,int>& m, int v)
{
    for (auto& p : m)
    {
        if (p.second == v) return p.first;
    }
    return -1;
}

#include "ServerDefinition.hpp"
#include "ServerNetwork.hpp"

#include "Utilities/ServerLogger.hpp"

#include "Models/Entities/AccountEntity.hpp"
#include "Models/Records/AccountRecord.hpp"

unordered_map<int, AccountEntity> Accounts; // Map accountID to AccountEntity
unordered_map<int, SessionEntity> Sessions; // Map accountID to SessionEntity

#include "Models/Entities/LobbyEntity.hpp"

LobbyEntity Lobby;

#include "Handlers/Handlers/AccountHandler.hpp"
#include "Handlers/Handlers/LobbyHandler.hpp"

#include "Handlers/Phases/ExitPhase.hpp"
#include "Handlers/Phases/WelcomePhase.hpp"
#include "Handlers/Phases/LobbyPhase.hpp"

#endif