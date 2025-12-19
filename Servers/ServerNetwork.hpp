#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../Commons/Networks/MessageHandler.hpp"
#include "../Commons/CommonDefinition.hpp"

int CreateSocket()
{
    int serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) 
    { 
        perror("socket"); 
        return 0; 
    }

    int opt = 1;
    setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serv = {};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(SERVER_PORT);

    if (bind(serverFD, (sockaddr*)&serv, sizeof(serv)) < 0) 
    { 
        perror("bind"); 
        return 0; 
    }
    if (listen(serverFD, 10) < 0) 
    { 
        perror("listen"); 
        return 1; 
    }

    cout << "Server listening on port " << SERVER_PORT << "...\n";

    return serverFD;
}

int AcceptClient(int serverFD)
{
    sockaddr_in clientAddr = {};
    socklen_t addrlen = sizeof(clientAddr);
    int clientFD = accept(serverFD, (sockaddr*)&clientAddr, &addrlen);
    if (clientFD < 0) 
    { 
        perror("accept");
        return -1; 
    }

    cout << FG_GREEN "●" RESET " Client " << clientFD << " connected.\n";

    return clientFD;
}

void BroadcastToClient(int senderFD, const string& msg, bool includeSender = false)
{
    vector<int> fds;

    {
        lock_guard<mutex> lock(ClientsMutex);
        for (const auto& client : Clients)
        {
            if (client.first != senderFD || includeSender)
            {
                fds.push_back(client.first);
            }
        }
    }

    for (int fd : fds)
    {
        SendMessage(fd, msg);
    }
}

void BroadcastToTeam(int teamID, const string& msg)
{
    vector<int> clientFDs;

    {
        lock_guard<mutex> lock(ClientsMutex);

        if (teamID < 0 || teamID >= (int)Teams.size()) return;

        for (int memberID : Teams[teamID].Members)
        {
			auto id = GetValueByKey(Clients, memberID);
            if (id != -1)
            {
                clientFDs.push_back(id);
            }
        }
    }

    for (int fd : clientFDs)
    {
        SendMessage(fd, msg);
    }
}

#endif