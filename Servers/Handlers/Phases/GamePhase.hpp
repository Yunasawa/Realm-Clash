#ifndef SERVER_HANDLER_PHASE_GAME
#define SERVER_HANDLER_PHASE_GAME

void HandleGamePhase(int clientFD, const string& code, const vector<string>& command)
{
	if (code == RQ_START_GAME)
	{
		WriteLog(LogType::Request, clientFD, "START GAME");
		HandleStartGame(clientFD);
	}
	else if (code == RQ_OCCUPY_SPOT)
	{
		HandleOccupySpot(clientFD, command[1]);
	}
}

#endif