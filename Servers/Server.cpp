#include "../Commons/CommonDefinition.hpp"
#include "../Commons/CommonIncluding.hpp"
#include "../Commons/CoreFunction.hpp"
#include "../Commons/Networks/MessageHandler.hpp"
#include "../Commons/Models/Lobby2.hpp"
#include "ServerDefinition.hpp"
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

        auto parts = SplitBySpace(msg);
        string code = parts[0];
        string response = msg;

        HandleWelcomePhase(clientFD, code, parts);
        HandleLobbyPhase(clientFD, code, parts);
    }

    {
        lock_guard<mutex> lock(ClientsMutex);
    }

    close(clientFD);
    cout << FG_RED "●" RESET " Client " << clientFD << " disconnected.\n";
}

int main()
{
    // Accounts.clear();

    // Accounts[1] = AccountEntity{1, "U0000001", false, false};
    // Accounts[2] = AccountEntity{2, "U0000002", true, false};
    // Accounts[3] = AccountEntity{3, "U0000003", true, true};
    // Accounts[4] = AccountEntity{4, "U0000004", true, false};
    // Accounts[5] = AccountEntity{5, "U0000005", false, false};

    ClearScreen();

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

    close(serverFD);
    return 1;
}