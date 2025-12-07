#include "Views/LobbyView.h"
#include "../Cores/Models/Lobby2.hpp"
#include "../Cores/Networks/MessageHandler.hpp"
#include "../Cores/CoreFunction.hpp" 
#include "../Cores/CoreIncluding.hpp" 
#include "../Cores/CoreDefinition.hpp"
#include "ClientDeclaration.hpp"

// vector<UIRoom> ConvertLobbyRooms(const Lobby& lobby) 
// {
//     vector<UIRoom> ui;
//     ui.reserve(lobby.Rooms.size());

//     // Sort rooms by ID ascending
//     vector<pair<int, Room>> sortedRooms(lobby.Rooms.begin(), lobby.Rooms.end());
//     sort(sortedRooms.begin(), sortedRooms.end(),
//          [](auto& a, auto& b){ return a.first < b.first; });

//     for (const auto& pair : sortedRooms) {
//         const Room& r = pair.second;

//         UIRoom u;
//         u.id = r.ID;
//         u.name = r.Name;
//         u.current = r.Members.size();
//         u.max = 15;
//         u.inMatch = r.InMatch;

//         ui.push_back(u);
//     }

//     return ui;
// }

void PrintRoomList(const vector<MyUIRoom>& rooms) 
{
    for (const auto& room : rooms) 
    {
        cout << room.ID << " " << room.Name << " - " << room.Host << endl;
    }
}

void ReceiveThread(int clientFD)
{
    while (true)
    {
        string msg = ReceiveMessage(clientFD);
        if (msg.empty()) break;

        auto split = SplitMessage(msg);
        auto code = atoi(split[0].c_str());

        if (code == RS_NETWORK_CONNECTED)
        {
            // ClientFD = atoi(data.c_str());
            cout << "Connect to server successfully!" << endl;
        }
        else if (code == RS_SIGN_UP_S)
        {
            cout << "Sign up successfully" << endl;
        }
        else if (code == RS_SIGN_UP_F_ACCOUNT_EXISTED)
        {
            cout << "Sign up failed: Account existed" << endl;
        }
        else if (code == RS_LOG_IN_S)
        {
            cout << "Log in successfully" << endl;
        }
        else if (code == RS_LOG_IN_F_WRONG_PASSWORD)
        {
            cout << "Log in failed: Wrong password" << endl;
        }
        else if (code == RS_LOG_IN_F_ACCOUNT_NOT_EXISTED)
        {
            cout << "Log in failed: Account not existed" << endl;
        }
        // else if (code == RS_UPDATE_ROOM_LIST)
        // { 
        //     ClearScreen();

        //     auto rooms = DeserializeLobby(data);
        //     PrintRoomList(rooms);            
        // }

        // if (atoi(msg.c_str()) == NETWORK_CONNECTED)
        // {
        //     Lobby lobby;
        //     string playerName = to_string(clientFD);

        //     int myFD = clientFD;
        //     // Add some sample rooms
        //     CreateRoom(lobby, myFD, "RoomA");
        //     CreateRoom(lobby, myFD, "RoomB");
        //     CreateRoom(lobby, myFD, "RoomC");
        //     CreateRoom(lobby, myFD, "RoomD");
        //     CreateRoom(lobby, myFD, "RoomE");
        //     CreateRoom(lobby, myFD, "RoomF");
        //     CreateRoom(lobby, myFD, "RoomG");

        //     int page = 0;

        //     vector<UIRoom> rooms = ConvertLobbyRooms(lobby);
        //     int totalPages = ((int)rooms.size() + 14)/15;

        //     DrawUI(rooms, playerName, page, totalPages);
        // }

        //cout << ClientFD << " - " << msg << endl;
    }
}

int main()
{
    ClearScreen();

    int clientFD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFD < 0) 
    { 
        perror("socket"); 
        return 0; 
    }

    sockaddr_in serv = {};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, CLIENT_IP, &serv.sin_addr);

    if (connect(clientFD, (sockaddr*)&serv, sizeof(serv)) < 0)
    {
        perror("connect");
        return 1;
    }

    thread(ReceiveThread, clientFD).detach();

    while (true)
    {
        string command, message;
        getline(cin, command);

        if (command == "/quit") break;

        SendMessage(clientFD, command);
    }

    close(clientFD);
    return 1;
}