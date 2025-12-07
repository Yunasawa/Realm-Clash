#ifndef MESSAGE_HANDLER
#define MESSAGE_HANDLER

#include "../CoreIncluding.hpp"

bool SendMessage(int clientFD, const string &msg)
{
    string fullMsg = msg + "\r\n";
    const char *buf = fullMsg.c_str();
    size_t len = fullMsg.size(), sent = 0;

    while (sent < len)
    {
        ssize_t n = send(clientFD, buf + sent, len - sent, 0);
        if (n <= 0) return false;
        sent += n;
    }
    return true;
}

string ReceiveMessage(int clientFD)
{
    string data;
    char c;
    while (true)
    {
        ssize_t n = recv(clientFD, &c, 1, 0);
        
        if (n <= 0) 
        {
            return "";
        }

        data.push_back(c);

        if (data.size() >= 2 && data[data.size()-2] == '\r' && data[data.size()-1] == '\n')
        {
            data.resize(data.size()-2); 
            return data;
        }
    }
}

#endif