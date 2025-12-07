#include "ServerDeclaration.hpp"
#include "ServerNetwork.hpp"
#include "ServerDefinition.hpp"
#include "../Cores/CoreDefinition.hpp"
#include "../Cores/CoreIncluding.hpp"
#include "../Cores/CoreFunction.hpp"
#include "../Cores/Networks/MessageHandler.hpp"
#include "../Cores/Models/Lobby2.hpp"

Lobby Lobby;

void HandleClient(int clientFD)
{
    {
        lock_guard<mutex> lock(ClientsMutex);
        Clients.push_back(clientFD);
    }

    SendMessage(clientFD, to_string(RS_NETWORK_CONNECTED) + " a");

    while (true)
    {
        string msg = ReceiveMessage(clientFD);
        if (msg.empty()) break;

        cout << "\033[33m■\033[0m [Client " << clientFD << "]: " << msg << endl;

        //auto [code, data] = HandleRequest(msg);
        auto parts = SplitMessage(msg);
        string code = parts[0];
        string response = msg;

        if (code == RQ_SIGN_UP)
        {
            if (FileExists(ACCOUNT_FILE_PATH))
            {
                string password;

                if (IsExistedAccount(ACCOUNT_FILE_PATH, parts[1], password))
                {
                    SendMessage(clientFD, to_string(RS_SIGN_UP_F_ACCOUNT_EXISTED));
                }
                else
                {
                    int accountID = atoi(ReadLine(STATUS_FILE_PATH, STATUS_ACCOUNT_ID).c_str());

                    stringstream ss;
                    ss << accountID << " " << parts[1] << " " << parts[2];
                    WriteAt(ACCOUNT_FILE_PATH, accountID, ss.str());
                    WriteAt(STATUS_FILE_PATH, STATUS_ACCOUNT_ID, to_string(accountID + 1));

                    SendMessage(clientFD, to_string(RS_SIGN_UP_S));
                }
            }
            else
            {
                cout << "File not exist" << endl;
            }
        }
        else if (code == RQ_LOG_IN)
        {
            if (FileExists(ACCOUNT_FILE_PATH))
            {
                string password;

                if (IsExistedAccount(ACCOUNT_FILE_PATH, parts[1], password))
                {
                    if (parts[2] == password)
                    {
                        SendMessage(clientFD, to_string(RS_LOG_IN_S));
                    }
                    else
                    {
                        SendMessage(clientFD, to_string(RS_LOG_IN_F_WRONG_PASSWORD));
                    }
                }
                else
                {
                    SendMessage(clientFD, to_string(RS_LOG_IN_F_ACCOUNT_NOT_EXISTED));
                }
            }
            else
            {
                cout << "File not exist" << endl;
            }
        }
        // else if (code == RQ_CREATE_ROOM)
        // {
        //     cout << "Created room name: " << data << endl;
        //     // response = to_string(RS_CREATE_ROOM_S) + " " + data;

        //     CreateRoom(Lobby, clientFD, data);
        //     response = to_string(RS_UPDATE_ROOM_LIST) + " " + SerializeLobby(Lobby).dump();
        
        //     BroadcastMessage(response, clientFD, true);
        // }
    }

    {
        lock_guard<mutex> lock(ClientsMutex);
        Clients.erase(remove(Clients.begin(), Clients.end(), clientFD), Clients.end());
    }

    close(clientFD);
    cout << "\033[31m●\033[0m Client " << clientFD << " disconnected.\n";
}

int main()
{
    ClearScreen();

    int ServerFD = CreateSocket();

    while (true)
    {
        int clientFD = AcceptClient(ServerFD);
        if (clientFD < 0) 
        {
            continue;
        }

        thread(HandleClient, clientFD).detach();
    }

    close(ServerFD);
    return 1;
}