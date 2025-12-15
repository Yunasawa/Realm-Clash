#include "../Commons/Models/Lobby2.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

void RemoveMember(Lobby &lobby, int hostfd, int removefd){
    int roomId = lobby.Clients[hostfd].RoomID;
    int mem_roomId = lobby.Clients[removefd].RoomID;
    if (roomId == -1 || roomId != mem_roomId) return;
    Room &room = lobby.Rooms[roomId];
    room.Members.erase(
        remove(room.Members.begin(), room.Members.end(), removefd),
        room.Members.end()
    );
    lobby.Clients[removefd].RoomID = -1;
    lobby.Clients[removefd].IsHost = false;
} 

int main()
{

}