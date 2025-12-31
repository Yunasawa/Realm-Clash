#ifndef SERVER_HANDLER_PHASE_GAME
#define SERVER_HANDLER_PHASE_GAME

void HandleGamePhase(int clientFD, const string& code, const vector<string>& command)
{
	if (code == RQ_RESET_GAME)
	{
		cout << "HELO WORLD" << endl;

		WriteLog(LogType::Request, -1, "RESET GAME");
		ResetGame();
		return;
	}

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
			HandleAnswerCastleQuestionPhaseCollect(clientFD, command[1]);
		}
		else if (code == RQ_VIEW_TEAM_RESOURCE)
		{
			HandleViewTeamResource(clientFD, command[1]);
		}
		else if (code == RQ_VIEW_CASTLE_INFORMATION)
		{
			HandleViewCastleInformation(clientFD,command[1]);
		}
		else if (code == RQ_UPDATE_MAP)
		{
			BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);
		}
	}
	else if (GamePhase == PHASE_CASTLE_COMBATING)
	{
		if (code == RQ_OCCUPY_SPOT)
		{
			HandleOccupySpot(clientFD, command[1]);
		}
		else if (code == RQ_OCCUPY_ANSWER_SPOT)
		{
			HandleAnswerSpotQuestion(clientFD, command[1]);
		}
		else if (code == RQ_BUY_EQUIPMENT)
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
		else if (code == RQ_VIEW_TEAM_RESOURCE)
		{
			HandleViewTeamResource(clientFD, command[1]);
		}
		else if (code == RQ_VIEW_INVENTORY)
		{
			HandleViewInventory(clientFD);
		}
		else if (code == RQ_VIEW_CASTLE_INFORMATION)
		{
			HandleViewCastleInformation(clientFD,command[1]);
		}
		else if (code == RQ_UPDATE_MAP)
		{
			BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);
		}
		else if (code == RQ_ATTACK_CASTLE)
		{
			HandleAttackCastle(clientFD,command[1]);
		}
		else if (code == RQ_OCCUPY_ANSWER_CASTLE)
		{
			HandleAnswerCastleQuestionPhaseCollect(clientFD, command[1]);
		}
		else if (code == RQ_ATTACK_ANSWER_CASTLE)
		{
			HandleAnswerCastleQuestionPhaseAttack(clientFD,command[1]);
		}
		else if (code == RQ_USING_WEAPON)
		{
			auto weapon = WeaponRecord::Deserialize(command[1]);

			HandleUsingWeaponToAttack(clientFD, weapon);
		}
		else if (code == RQ_USING_ALL_WEAPON)
		{
			HandleAllInAttack(clientFD, command[1]);
		}
	}


}

#endif