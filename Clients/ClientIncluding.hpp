#ifndef CLIENT_INCLUDING
#define CLIENT_INCLUDING

/*Lưu các file .hpp cần được khai báo !*/

#include "Models/Records/AccountRecord.hpp"
#include "Models/Records/LobbyRecord.hpp"
#include "Models/Records/MapRecord.hpp"
#include "Models/Records/ResourceRecord.hpp"

#include "Models/Entities/AccountEntity.hpp"
#include "Models/Entities/OccupationEntity.hpp"
#include "Models/Entities/CartEntity.hpp"
#include "Models/Entities/QuestionEntity.hpp"

int CurrentPhase = 0;
AccountRecord Account;
LobbyRecord Lobby;
int JoinRequestAmount = 0;
int TeamInviteRequest = 0;
int PendingJoinTick = 0;
int PendingInviteTick = 0;
string Log = FG_GREEN "";
int Team;
int Tick;
MapRecord Map;
ResourceRecord Resource;
int CurrentQuestionSpot = -1;
bool CurrentQuestionIsCastle = false;
QuestionEntity CurrentQuestion;
int QuestionTimeOut = 30;


#include "ClientUtilities.hpp"

#include "Views/LobbyView.hpp"
#include "Views/WelcomeView.hpp"
#include "Views/GameView.hpp"
#include "Views/ShopView.hpp"
#include "Views/QuestionView.hpp"

#include "Handlers/Phases/PhaseWelcome.hpp"
#include "Handlers/Phases/PhaseLobby.hpp"
#include "Handlers/Phases/PhaseGame.hpp"

#endif