#ifndef CLIENT_HANDLER_PHASE_GAME
#define CLIENT_HANDLER_PHASE_GAME

void HandleGameInput(int clientFD, vector<string> command)
{

}

void HandleGameResponse(int clientFD, const string& code, vector<string> split)
{
	if (code == RS_UPDATE_GAME_START)
	{
		CurrentPhase = PHASE_GAME_START_GAME;

		ShowGameLog(LOG_NONE);
	}
	else if (code == RS_START_GAME_F_NOT_ENOUGH_TEAMS)
	{
		ShowLobbyLog(FG_RED "Start game failed: Not enough teams!");
	}
}

#endif