#ifndef SERVER_DECLARATION
#define SERVER_DECLARATION

#include "../Cores/CoreIncluding.h"
#include <iomanip>

struct ClientInfo 
{
    int FD; 
    std::string Name;   
    int RoomID = -1;    
    bool IsHost = false;    
};

struct Room 
{
    int ID;
    std::string Name;
    int HostFD;
    std::vector<int> Members;
    bool InMatch;
};

struct Lobby 
{
    std::unordered_map<int, ClientInfo> Clients;
    std::unordered_map<int, Room> Rooms;
    int NextRoomID = 1;
};

vector<int> clients;
mutex clients_mutex;

#endif