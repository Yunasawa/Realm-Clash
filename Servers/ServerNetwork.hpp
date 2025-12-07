#ifndef SERVER_NETWORK
#define SERVER_NETWORK

#include "../Cores/Networks/MessageHandler.hpp"
#include "../Cores/CoreDefinition.hpp"

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

    cout << "\033[32m●\033[0m Client " << clientFD << " connected.\n";

    return clientFD;
}

void BroadcastMessage(const string &msg, int senderFD, bool ignoreSender)
{
    lock_guard<mutex> lock(ClientsMutex);
    for (int fd : Clients)
    {
        if (fd != senderFD || ignoreSender)
        {
            SendMessage(fd, msg);
        }
    }
}
#endif