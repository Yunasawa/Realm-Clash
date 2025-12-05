#include "../Cores/CoreIncluding.h"
#include "../Cores/CoreDefinition.h"
#include "ServerDeclaration.h"
#include "../Cores/Networks/MessageHandler.h"

void SplitCommand(const std::string& input, std::string& cmd, std::string& data)
{
    std::istringstream iss(input);

    iss >> cmd;

    std::getline(iss, data);

    if (!data.empty() && data[0] == ' ')
    {
        data.erase(0, 1);
    }
}

Room CreateRoom(Lobby &lobby, int creatorFd, const std::string& roomName) 
{
    Room room;
    room.ID = lobby.NextRoomID++;
    room.Name = roomName;
    room.HostFD = creatorFd;
    room.Members.push_back(creatorFd);

    lobby.Clients[creatorFd].RoomID = room.ID;
    lobby.Clients[creatorFd].IsHost = true;

    lobby.Rooms[room.ID] = room;
    return room;
}

bool JoinRoom(Lobby &lobby, int fd, int roomId) 
{
    auto it = lobby.Rooms.find(roomId);
    if (it == lobby.Rooms.end()) return false;

    Room &room = it->second;
    if ((int)room.Members.size() >= 15)
        return false;

    room.Members.push_back(fd);
    lobby.Clients[fd].RoomID = roomId;
    lobby.Clients[fd].IsHost = false;

    return true;
}

void LeaveRoom(Lobby &lobby, int fd) 
{
    int roomId = lobby.Clients[fd].RoomID;
    if (roomId == -1) return;

    Room &room = lobby.Rooms[roomId];

    room.Members.erase(
        std::remove(room.Members.begin(), room.Members.end(), fd),
        room.Members.end()
    );
    lobby.Clients[fd].RoomID = -1;
    lobby.Clients[fd].IsHost = false;

    // nếu host rời, chuyển host
    if (fd == room.HostFD && !room.Members.empty()) {
        room.HostFD = room.Members[0];
        lobby.Clients[room.HostFD].IsHost = true;
    }

    // nếu phòng không còn ai → xóa
    if (room.Members.empty()) {
        lobby.Rooms.erase(roomId);
    }
}

void PrintRooms(const Lobby& lobby)
{
    std::cout << "===== ROOM LIST =====\n";

    if (lobby.Rooms.empty()) {
        std::cout << "No rooms available.\n";
        return;
    }

    for (const auto& pair : lobby.Rooms) {
        const Room& room = pair.second;

        std::cout << "Room ID: " << room.ID << "\n";
        std::cout << "Name   : " << room.Name << "\n";
        std::cout << "HostFD : " << room.HostFD << "\n";
        std::cout << "Members (" << room.Members.size() << "/15): ";

        if (room.Members.empty()) {
            std::cout << "(none)";
        } else {
            for (size_t i = 0; i < room.Members.size(); ++i) {
                std::cout << room.Members[i];
                if (i + 1 < room.Members.size()) std::cout << ", ";
            }
        }

        std::cout << "\n-------------------------\n";
    }
}

int main()
{
    Lobby lobby;
    string command;

    while (true)
    {
        getline(cin, command);

        std::string cmd, data;
        SplitCommand(command, cmd, data);

        int cmdID = atoi(cmd.c_str());

        if (cmdID == 1)
        {
            std::string id, roomName;
            SplitCommand(data, id, roomName);
            Room room = CreateRoom(lobby, atoi(id.c_str()), roomName);
            PrintRooms(lobby);
        }
        else if (cmdID == 2)
        {
            std::string fd, roomID;
            SplitCommand(data, fd, roomID);
            JoinRoom(lobby, atoi(fd.c_str()), atoi(roomID.c_str()));
            PrintRooms(lobby);
        }
        else if (cmdID == 3)
        {
            LeaveRoom(lobby, atoi(data.c_str()));
            PrintRooms(lobby);
        }
    }

    return 0;
}