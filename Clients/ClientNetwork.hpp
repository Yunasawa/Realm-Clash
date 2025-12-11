#ifndef CLIENT_NETWORK
#define CLIENT_NETWORK

int CreateSocket(const string& ip)
{
    int clientFD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFD < 0) 
    { 
        perror("socket"); 
        return -1; 
    }

    sockaddr_in serv = {};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ip.c_str(), &serv.sin_addr);

    if (connect(clientFD, (sockaddr*)&serv, sizeof(serv)) < 0)
    {
        perror("connect");
        return -1;
    }

    return clientFD;
}

#endif