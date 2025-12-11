#ifndef SERVER_INCLUDING
#define SERVER_INCLUDING

mutex ClientsMutex;
unordered_map<int, int> Clients;
int RoomLeader = 0;

#include "ServerNetwork.hpp"

#include "Models/Entities/AccountEntity.hpp"
#include "Models/Records/AccountRecord.hpp"

unordered_map<int, AccountEntity> Accounts;

#include "Models/Entities/LobbyEntity.hpp"

#include "Handlers/AccountHandler.hpp"
#include "Handlers/LobbyHandler.hpp"

#endif