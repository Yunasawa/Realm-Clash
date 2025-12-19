#ifndef CLIENT_HANDLER_PHASE_GAME
#define CLIENT_HANDLER_PHASE_GAME

void HandleGameInput(int clientFD, vector<string> command)
{
	string message;
	int code = atoi(command[0].c_str());

	if (CurrentPhase == PHASE_GAME_MAP_IDLING)
	{
		if (code == 1 && command.size() == 3)
		{
			auto spot = stoi(command[1]);
			auto type = stoi(command[2]);

			if (spot > 0 && spot < 7)
			{
				if (type > 0 && type < 4)
				{
					auto spotOccupation = SpotOccupationEntity{ spot - 1, type - 1 };
					SendMessage(clientFD, string(RQ_OCCUPY_SPOT) + " " + spotOccupation.Serialize());
				}
				else
				{
					ShowLobbyLog(LOG_GAME_WRONG_RESOURCE_TYPE);
				}
			}
			else
			{
				ShowLobbyLog(LOG_GAME_WRONG_SPOT);
			}
		}
		else goto UnknownCommand;
	}

	return;

UnknownCommand:
	ShowGameLog(LOG_UNKNOWN_COMMAND);
}

void HandleGameResponse(int clientFD, const string& code, vector<string> data)
{
	if (code == RS_UPDATE_GAME_START)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(LOG_NONE);
	}
	else if (code == RS_START_GAME_F_NOT_ENOUGH_TEAMS)
	{
		ShowLobbyLog(FG_RED "Start game failed: Not enough teams!");
	}
	else if (code == RS_UPDATE_GAME_TICK)
	{
		Tick = stoi(data[1]);

		WriteLineAt(0, GetGameTitle());
	}
	else if (code == RS_UPDATE_GAME_MAP)
	{
		Map = MapRecord::Deserialize(data[1]);

		ShowGameView();
	}
	else if (code == RS_OCCUPY_SPOT_S)
	{
		ShowGameLog(FG_GREEN "Successfully occupied spot!");
	}
}

#endif