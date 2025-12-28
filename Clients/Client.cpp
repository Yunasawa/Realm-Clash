#include "../Commons/Networks/MessageHandler.hpp"
#include "../Commons/CoreFunction.hpp" 
#include "../Commons/CommonIncluding.hpp" 
#include "../Commons/CommonDefinition.hpp"
#include "../Commons/Definitions/TextStyle.hpp"
#include "ClientNetwork.hpp"
#include "ClientDefinition.hpp"
#include "ClientIncluding.hpp"

void CallPhase(int phase, int clientFD, const vector<string>& args)
{
    static void (*funcs[])(int, vector<string>) = 
    { 
        HandleWelcomeInput, 
        HandleLobbyInput,
		HandleGameInput
    };

    funcs[phase](clientFD, args);
}

void ReceiveThread(int clientFD)
{
    while (true)
    {
        string msg = ReceiveMessage(clientFD);
        if (msg.empty()) break;
            
        auto split = SplitBySpace(msg);
        auto code = split[0];

        HandleWelcomeResponse(clientFD, code, split);
        HandleLobbyResponse(clientFD, code, split);
        HandleGameResponse(clientFD, code, split);
    }
}

int main(int argc, char *argv[])
{
    ClearScreen();

    int clientFD = CreateSocket((argc > 1) ? argv[1] : CLIENT_IP);

    thread(ReceiveThread, clientFD).detach();

    while (true)
    {
        string command;
        getline(cin, command);

        auto split = SplitBySpace(command);
        
        CallPhase(CurrentPhase / 100, clientFD, split);
    }

    close(clientFD);
    return 1;
}
/* Chia 2 thread để giao tiếp nhận và gửi với server */