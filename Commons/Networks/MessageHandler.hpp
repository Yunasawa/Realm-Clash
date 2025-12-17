#ifndef MESSAGE_HANDLER
#define MESSAGE_HANDLER

#include "../CommonIncluding.hpp"

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
    static thread_local string buffer;
    char temp[RECEIVE_CHUNK_SIZE];

    while (true)
    {
        size_t pos = buffer.find("\r\n");
        if (pos != string::npos)
        {
            string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);
            return line;
        }

        ssize_t n = recv(clientFD, temp, sizeof(temp), 0);
        if (n <= 0)
        {
            return "";
        }

        buffer.append(temp, n);

        if (buffer.size() > BUFFER_LIMIT_LENGTH)
        {
            return "";
        }
    }
}

#endif