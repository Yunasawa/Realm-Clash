#include "../Commons/CommonDefinition.hpp"
#include "../Commons/CommonIncluding.hpp"
#include "../Commons/CoreFunction.hpp"
#include "../Commons/Networks/MessageHandler.hpp"
#include "ServerIncluding.hpp"

void HandleClient(int clientFD)
{
    {
        lock_guard<mutex> lock(ClientsMutex);
    }

    SendMessage(clientFD, RS_NETWORK_CONNECTED);

    while (true)
    {
        string msg = ReceiveMessage(clientFD);
        if (msg.empty()) break;

        cout << FG_YELLOW "■" RESET " Client " << clientFD << ": " << msg << endl;

        auto command = SplitBySpace(msg);
        string code = command[0];
        string response = msg;

        HandleWelcomePhase(clientFD, code, command);
        HandleLobbyPhase(clientFD, code, command);
		HandleGamePhase(clientFD, code, command);
    }

    {
        lock_guard<mutex> lock(ClientsMutex);
    }

    HandleQuitPhase(clientFD);
    close(clientFD);
    cout << FG_RED "●" RESET " Client " << clientFD << " disconnected.\n";
}

int main()
{
    //Accounts[1] = AccountEntity{1, "U0000001", 1, 0, true, true };
    //Accounts[2] = AccountEntity{2, "U0000002", 1, 0, false, false };
    //Accounts[3] = AccountEntity{3, "U0000003", 1, 0, false, false };

    //Lobby.Teams[0].Members = { 1, 2, 3 };

    ClearScreen();

    LogFile.open(LOG_PATH, std::ios::out | std::ios::trunc);
    if (!LogFile)
    {
        cout << "Failed to open " << LOG_PATH << endl;
    }
    else
    {
        LogFile << "        TIME        ┃ ID ┃ ➖ ┃                               ACTION                                   ┃                 DATA                   \n";
        LogFile << "━━━━━━━━━━━━━━━━━━━━╋━━━━╋━➖━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╋━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    }

    int serverFD = CreateSocket();

    while (true)
    {
        int clientFD = AcceptClient(serverFD);
        if (clientFD < 0) 
        {
            continue;
        }

        thread(HandleClient, clientFD).detach();
    }

    LogFile.close();
    close(serverFD);
    return 1;
}