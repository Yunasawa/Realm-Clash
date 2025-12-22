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
		else if (code == 2 && command.size() == 2)
		{
			auto castle = stoi(command[1]);

			if (castle > 0 && castle < 4)
			{
				SendMessage(clientFD, string(RQ_OCCUPY_CASTLE) + " " + to_string(castle - 1));
			}
			else
			{
				ShowLobbyLog(FG_RED "Trying to occupy wrong castle!");
			}
		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_MAP_COMBATING)
	{
		if (code == 1 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_SHOPING_WEAPON;

			ShowShopView();
		}
		else if (code == 2 && command.size() == 2)
		{

		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_SHOPING_WEAPON)
	{
		if (code == 1 && command.size() == 3)
		{
			auto cart = CartEntity
			{
				.Type = 0,
				.Equipment = stoi(command[1]),
				.Amount = stoi(command[2])
			};

			SendMessage(clientFD, string(RQ_BUY_EQUIPMENT) + " " + cart.Serialize());
		}
		else if (code == 2 && command.size() == 1)
		{

		}
		else if (code == 3 && command.size() == 1)
		{

		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
	{
		if (code == 1 && command.size() == 3)
		{
			auto cart = CartEntity
			{
				.Type = 1,
				.Equipment = stoi(command[1]),
				.Amount = stoi(command[2])
			};

			SendMessage(clientFD, string(RQ_BUY_EQUIPMENT) + " " + cart.Serialize());
		}
		else if (code == 2 && command.size() == 1)
		{

		}
		else if (code == 3 && command.size() == 1)
		{

		}
		else goto UnknownCommand;
	}

	return;

UnknownCommand:
	ShowGameLog(LOG_UNKNOWN_COMMAND);
}

void HandleGameResponse(int clientFD, const string& code, vector<string> data)
{
	if (code == RS_UPDATE_GAME_TICK)
	{
		Tick = stoi(data[1]);

		WriteLineAt(1, GetGameTitle());
	}

	if (code == RS_UPDATE_TEAM_RESOURCE)
	{
		Resource = ResourceRecord::Deserialize(data[1]);

		WriteLineAt(2, GetResourceLine());
	}

	if (code == RS_UPDATE_GAME_START)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(LOG_NONE);
	}
	else if (code == RS_START_GAME_F_NOT_ENOUGH_TEAMS)
	{
		ShowLobbyLog(FG_RED "Start game failed: Not enough teams!");
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
	else if (code == RS_OCCUPY_SPOT_F_SPOT_OCCUPIED)
	{
		ShowGameLog(FG_RED "Spot slot has been occupied!");
	}
	else if (code == RS_OCCUPY_SPOT_F_FULL_OF_SLOT)
	{
		ShowGameLog(FG_RED "Both slots of this resource has been occupied!");
	}
	else if (code == RS_OCCUPY_CASTLE_F_CASTLE_OCCUPIED)
	{
		ShowGameLog(FG_RED "Castle has been occupied!");
	}
	else if (code == RS_OCCUPY_CASTLE_F_SLOT_FULL)
	{
		ShowGameLog(FG_RED "Occupy only one castle at a time!");
	}
	else if (code == RS_UPDATE_START_COMBAT)
	{
		CurrentPhase = PHASE_GAME_MAP_COMBATING;

		ShowGameView();
		ShowGameLog(FG_YELLOW "Start combating. FIGHT!");
	}
}

#endif