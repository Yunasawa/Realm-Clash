#ifndef LOBBY2
#define LOBBY2

#include "../CoreIncluding.hpp"

struct Account
{
    int ID;
    string Email;
    string Password;
    
    string Username;
};

struct Client
{
    int FD;
    string Name;
    int RoomID;
    bool IsHost;
};

struct Room
{
    int ID;
    string Name;
    int HostFD;
    vector<int> Members;
};

struct Lobby
{
    int NextRoomID;
    unordered_map<int, Client> Clients;
    unordered_map<int, Room> Rooms;
};

Room CreateRoom(Lobby& lobby, int hostFD, const string& roomName)
{
    Room room;
    room.ID = lobby.NextRoomID++;
    room.Name = roomName;
    room.HostFD = hostFD;
    room.Members.push_back(hostFD);

    lobby.Clients[hostFD].RoomID = room.ID;
    lobby.Clients[hostFD].IsHost = true;
    lobby.Rooms[room.ID] = room;
    return room;
}

json SerializeLobby(const Lobby& lobby) 
{
    json j;
    for (const auto& [id, room] : lobby.Rooms) 
    {
        j["Rooms"].push_back({
            {"ID", room.ID},
            {"Name", room.Name},
            {"Host", lobby.Clients.at(room.HostFD).Name},
        });
    }
    return j;
}

struct MyUIRoom 
{
    int ID;
    string Name;
    string Host;
};

vector<MyUIRoom> DeserializeLobby(const string& msg) 
{
    json j = json::parse(msg);

    vector<MyUIRoom> rooms;
    for (const auto& r : j["Rooms"]) 
    {
        MyUIRoom ui;
        ui.ID = r["ID"];
        ui.Name = r["Name"];
        ui.Host = r["Host"];
        rooms.push_back(ui);
    }

    return rooms;
}

#endif