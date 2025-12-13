#ifndef CLIENT_INCLUDING
#define CLIENT_INCLUDING

#include "Models/Records/AccountRecord.hpp"
#include "Models/Records/LobbyRecord.hpp"

#include "Models/Entities/AccountEntity.hpp"
#include "Models/Entities/LobbyEntity.hpp"

int CurrentPhase = 0;
AccountRecord Account;
LobbyRecord Lobby;
int JoinRequestAmount = 0;
string Log = FG_GREEN "";

#include "Views/LobbyView.hpp"
#include "Views/WelcomeView.hpp"

#include "Handlers/Phases/PhaseWelcome.hpp"
#include "Handlers/Phases/PhaseLobby.hpp"

#endif