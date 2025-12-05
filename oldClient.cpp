#include "../Cores/CoreIncluding.h"
#include "../Cores/CoreDefinition.h"
#include "../Cores/Networks/MessageHandler.h"

void ReceiveThread(int client_fd)
{
    while (true)
    {
        string msg = ReceiveMessage(client_fd);
        if (msg.empty()) break;
        cout << msg << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: ./client <server-ip>\n";
        return 0;
    }

    string ip = argv[1];

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) { perror("socket"); return 1; }

    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, ip.c_str(), &serv.sin_addr);

    if (connect(client_fd, (sockaddr*)&serv, sizeof(serv)) < 0)
    {
        perror("connect");
        return 1;
    }

    thread(ReceiveThread, client_fd).detach();

    while (true)
    {
        cout << "Enter message: ";
        string msg;
        getline(cin, msg);

        if (msg == "/quit") break;

        // protocol command + json
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

            SendMessage(client_fd, "MESSAGE " + j.dump());
        }
        else
        {
            SendMessage(client_fd, msg); // other commands like SCORE
        }
    }

    close(client_fd);
    return 0;
}
