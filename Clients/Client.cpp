#include "Views/LobbyView.h"
#include "../Cores/Models/Lobby.hpp"
#include "../Cores/Networks/MessageHandler.hpp"
#include "../Cores/CoreFunction.hpp" 
#include "../Cores/CoreIncluding.hpp" 
#include "../Cores/CoreDefinition.hpp"
#include "ClientDeclaration.hpp"

vector<UIRoom> ConvertLobbyRooms(const Lobby& lobby) 
{
    vector<UIRoom> ui;
    ui.reserve(lobby.Rooms.size());

    // Sort rooms by ID ascending
    vector<pair<int, Room>> sortedRooms(lobby.Rooms.begin(), lobby.Rooms.end());
    sort(sortedRooms.begin(), sortedRooms.end(),
         [](auto& a, auto& b){ return a.first < b.first; });

    for (const auto& pair : sortedRooms) {
        const Room& r = pair.second;

        UIRoom u;
        u.id = r.ID;
        u.name = r.Name;
        u.current = r.Members.size();
        u.max = 15;
        u.inMatch = r.InMatch;

        ui.push_back(u);
    }

    return ui;
}

void ReceiveThread(int clientFD)
{
    while (true)
    {
        string msg = ReceiveMessage(clientFD);
        if (msg.empty()) break;

        if (atoi(msg.c_str()) == NETWORK_CONNECTED)
        {
            Lobby lobby;
            string playerName = to_string(clientFD);

            int myFD = 999;
            // Add some sample rooms
            CreateRoom(lobby, myFD, "RoomA");
            CreateRoom(lobby, myFD, "RoomB");
            CreateRoom(lobby, myFD, "RoomC");
            CreateRoom(lobby, myFD, "RoomD");
            CreateRoom(lobby, myFD, "RoomE");
            CreateRoom(lobby, myFD, "RoomF");
            CreateRoom(lobby, myFD, "RoomG");

            int page = 0;

            vector<UIRoom> rooms = ConvertLobbyRooms(lobby);
            int totalPages = ((int)rooms.size() + 14)/15;

            DrawUI(rooms, playerName, page, totalPages);
        }
    }
}

int main()
{
    int ClientFD = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientFD < 0) 
    { 
        perror("socket"); 
        return 0; 
    }

    sockaddr_in serv = {};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, CLIENT_IP, &serv.sin_addr);

    if (connect(ClientFD, (sockaddr*)&serv, sizeof(serv)) < 0)
    {
        perror("connect");
        return 1;
    }

    thread(ReceiveThread, ClientFD).detach();

    while (true)
    {
        string msg;
        getline(cin, msg);

        if (msg == "/quit") break;

        if (msg.rfind("MESSAGE ", 0) == 0)
        {
            string text = msg.substr(8);

            MessageData data;
            data.text = text;

            // generate time
            time_t now = time(nullptr);
            data.time = ctime(&now);
            if (!data.time.empty() && data.time.back() == '\n')
                data.time.pop_back();

            json j = data;

            SendMessage(ClientFD, "MESSAGE " + j.dump());
        }
        else
        {
            SendMessage(ClientFD, msg);
        }
    }

    close(ClientFD);
    return 1;
}