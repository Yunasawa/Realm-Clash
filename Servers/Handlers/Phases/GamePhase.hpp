#ifndef SERVER_HANDLER_PHASE_GAME
#define SERVER_HANDLER_PHASE_GAME

void HandleGamePhase(int clientFD, const string& code, const vector<string>& command)
{
	if (GamePhase == PHASE_LOBBY_IDLING)
	{
		if (code == RQ_START_GAME)
		{
			HandleStartGame(clientFD);

			GamePhase = PHASE_RESOURCE_COLLECTING;
		}
	}
	else if (GamePhase == PHASE_RESOURCE_COLLECTING)
	{
		if (code == RQ_OCCUPY_SPOT)
		{
			HandleOccupySpot(clientFD, command[1]);
		}
		else if (code == RQ_OCCUPY_ANSWER_SPOT)
		{
			HandleAnswerSpotQuestion(clientFD, command[1]);
		}
		else if (code == RQ_OCCUPY_CASTLE)
		{
			HandleOccupyCastle(clientFD, command[1]);
		}
		else if (code == RQ_OCCUPY_ANSWER_CASTLE)
		{
			HandleAnswerCastleQuestion(clientFD, command[1]);
		}
	}
	else if (GamePhase == PHASE_CASTLE_COMBATING)
	{
		if (code == RQ_BUY_EQUIPMENT)
		{
			auto cart = CartRecord::Deserialize(command[1]);

			if (cart.Type == 0)
			{
				HandleBuyWeapon(clientFD, cart);
			}
			else
			{
				HandleBuyDefense(clientFD, cart);
			}
		}
	}
}

#endif