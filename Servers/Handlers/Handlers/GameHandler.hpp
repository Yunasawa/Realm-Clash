#ifndef SERVER_HANDLER_GAME
#define SERVER_HANDLER_GAME

void HandleStartGame(int clientFD)
{
	if (Lobby.CountTeam() < 3)
	{
		WriteLog(LogType::Failure, clientFD, "START GAME : Not enough teams.", "Current team: " + to_string(Lobby.CountTeam()));
		SendMessage(clientFD, string(RS_START_GAME_F_NOT_ENOUGH_TEAMS));
		
		return;
	}

	WriteLog(LogType::Success, clientFD, "START GAME");
	BroadcastMessage(clientFD, string(RS_UPDATE_GAME_START), true);
}

#endif