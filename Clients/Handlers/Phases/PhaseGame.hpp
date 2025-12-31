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

					PreviousPhase = CurrentPhase;

					CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
				}
				else
				{
					ShowGameLog(LOG_GAME_WRONG_RESOURCE_TYPE);
				}
			}
			else
			{
				ShowGameLog(LOG_GAME_WRONG_SPOT);
			}
		}
		else if (code == 2 && command.size() == 2)
		{
			auto castle = stoi(command[1]);

			if (castle > 0 && castle < 4)
			{
				SendMessage(clientFD, string(RQ_OCCUPY_CASTLE) + " " + to_string(castle - 1));

				PreviousPhase = CurrentPhase;

				CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
			}
			else
			{
				CurrentPhase = PHASE_GAME_MAP_IDLING;

				ShowGameLog(FG_RED "Trying to occupy wrong castle!");
			}
		}
		else if (code == 3 && command.size() == 2)
		{
			/*Xem sức phòng thủ của lâu đài*/
			auto castle = stoi(command[1]);

			if (castle > 0 && castle < 4)
			{
				SendMessage(clientFD, string(RQ_VIEW_CASTLE_INFORMATION) + " " + to_string(castle - 1));
			}
			else 
			{
				ShowGameLog(FG_RED "Castle not found");
			}
		}
		else if (code == 4 && command.size() == 2)
		{
			/*Xem resource của các team khác*/
			auto teamID = stoi(command[1]);

			if (teamID > 0 && teamID < 6)
			{
				SendMessage(clientFD, string(RQ_VIEW_TEAM_RESOURCE) + " " + to_string(teamID - 1));
			}
			else
			{
				ShowGameLog(FG_RED "Team not found!");
			}
		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_QUESTION_ANSWERING)
	{
			if (code == 1 && command.size() == 2) {
				// Answer format: 1 <answerID>
				if (CurrentQuestionIsCastle == 1) {
					SendMessage(clientFD, string(RQ_OCCUPY_ANSWER_CASTLE) + " " + command[1]);
				}
				else if (CurrentQuestionIsCastle == 2){
					SendMessage(clientFD, string(RQ_ATTACK_ANSWER_CASTLE) + " " + command[1]);
				}				 
				else {
					SendMessage(clientFD, string(RQ_OCCUPY_ANSWER_SPOT) + " " + command[1]);
				}
				return;
			}
			else if (code == 2 && command.size() == 1){
				// CurrentPhase = PHASE_GAME_MAP_IDLING;
				CurrentPhase = PreviousPhase;
				
				SendMessage(clientFD, string(RQ_UPDATE_MAP));

				// ShowGameLog("");
			}

			else {
				ShowQuestionLog(CurrentQuestion,FG_RED "Unknown command");
			}
	}
	else if (CurrentPhase == PHASE_GAME_MAP_COMBATING)
	{
		if (code == 1 && command.size() == 3) /*Tiếp tục đánh chiếm spot*/ 
		{
			auto spot = stoi(command[1]);
			auto type = stoi(command[2]);

			if (spot > 0 && spot < 7)
			{
				if (type > 0 && type < 4)
				{
					auto spotOccupation = SpotOccupationEntity{ spot - 1, type - 1 };
					SendMessage(clientFD, string(RQ_OCCUPY_SPOT) + " " + spotOccupation.Serialize());

					PreviousPhase = CurrentPhase;

					CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
				}
				else
				{
					ShowGameLog(LOG_GAME_WRONG_RESOURCE_TYPE);
				}
			}
			else
			{
				ShowGameLog(LOG_GAME_WRONG_SPOT);
			}
		}
		else if (code == 2 && command.size() == 2) /*Attack castle*/
		{
			auto castle = stoi(command[1]);

			if (castle > 0 && castle <4)
			{
				PreviousPhase = CurrentPhase;

				SendMessage(clientFD, string(RQ_ATTACK_CASTLE) + " " + to_string(castle - 1));
			}
			else 
			{
				ShowGameLog(FG_RED "Castle not found");
			}

		}
		else if (code == 3 && command.size() == 1)
		{
			PreviousPhase = CurrentPhase;

			// CurrentPhase = PHASE_GAME_VIEW_INVENTORY;

			SendMessage(clientFD, string(RQ_VIEW_INVENTORY));
		}
		else if (code == 4 && command.size() == 1) /*View shop*/
		{
			CurrentPhase = PHASE_GAME_SHOPING_WEAPON;

			ShowAttackShopView();
		}
		else if (code == 5 && command.size() == 2)
		{
			/*Xem sức phòng thủ của lâu đài*/

			auto castle = stoi(command[1]);

			if (castle > 0 && castle < 4)
			{
				SendMessage(clientFD, string(RQ_VIEW_CASTLE_INFORMATION) + " " + to_string(castle - 1));
			}
			else 
			{
				ShowGameLog(FG_RED "Castle not found");
			}
		}
		else if (code == 6 && command.size() == 2)
		{
			/*Xem thông tin resources của các team*/
			auto teamID = stoi(command[1]);

			if (teamID > 0 && teamID < 6)
			{
				SendMessage(clientFD, string(RQ_VIEW_TEAM_RESOURCE) + " " + to_string(teamID - 1));
			}
			else
			{
				ShowGameLog(FG_RED "Team not found!");
			}
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
				.Equipment = stoi(command[1]) - 1,
				.Amount = stoi(command[2])
			};

			SendMessage(clientFD, string(RQ_BUY_EQUIPMENT) + " " + cart.Serialize());
		}
		else if (code == 2 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_SHOPING_DEFENSE;

			ShowDefenseShopView();
		}
		else if (code == 3 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_MAP_COMBATING;

			// ShowGameView();

			SendMessage(clientFD, string(RQ_UPDATE_MAP));
		}
		else if (code == 4 && command.size() == 1)
		{
			PreviousPhase = CurrentPhase;

			// CurrentPhase = PHASE_GAME_VIEW_INVENTORY;

			SendMessage(clientFD, string(RQ_VIEW_INVENTORY));
		}
		else
		{
			ShowAttackShopLog(LOG_UNKNOWN_COMMAND);
		}
	}
	else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
	{
		if (code == 1 && command.size() == 4)
		{
			auto cart = CartEntity
			{
				.Type = 1,
				.Equipment = stoi(command[1]) -1,
				.Amount = stoi(command[2]),
				.TargetCastle = stoi(command[3]) - 1
			};

			SendMessage(clientFD, string(RQ_BUY_EQUIPMENT) + " " + cart.Serialize());
		}
		else if (code == 2 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_SHOPING_WEAPON;

			ShowAttackShopView();
		}
		else if (code == 3 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_MAP_COMBATING;

			// ShowGameView();

			SendMessage(clientFD, string(RQ_UPDATE_MAP));
		}
		else if (code == 4 && command.size() == 1)
		{
			PreviousPhase = CurrentPhase;

			// CurrentPhase = PHASE_GAME_VIEW_INVENTORY;

			SendMessage(clientFD, string(RQ_VIEW_INVENTORY));
		}
		else
		{
			ShowDefenseShopLog(LOG_UNKNOWN_COMMAND);
		}
	}
	else if (CurrentPhase == PHASE_GAME_VIEW_INVENTORY)
	{
		if (code == 1 && command.size() == 1)
		{
			if (PreviousPhase == PHASE_GAME_SHOPING_DEFENSE)
			{
				CurrentPhase = PHASE_GAME_SHOPING_DEFENSE; 

				ShowDefenseShopView();
			}
			else if (PreviousPhase == PHASE_GAME_SHOPING_WEAPON || PreviousPhase == PHASE_GAME_MAP_COMBATING)
			{
				CurrentPhase = PHASE_GAME_SHOPING_WEAPON;

				ShowAttackShopView();
			}
		}
		else if (code == 2 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_MAP_COMBATING;

			// ShowGameView();

			SendMessage(clientFD, string(RQ_UPDATE_MAP));
		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_CASTLE_ATTACKING)
	{
		if (code == 1 && command.size() == 3)
		{
			int castle = CurrentTargetCastle.Id - 1;
			int weapon = stoi(command[1]);
			int amount = stoi(command[2]);
			auto Weapon = WeaponEntity{ castle,weapon - 1,amount };

			SendMessage(clientFD, string(RQ_USING_WEAPON) + " " + Weapon.Serialize());
		}
		else if (code == 2 && command.size() == 1)
		{
			SendMessage(clientFD, string(RQ_USING_ALL_WEAPON) + " " + to_string(CurrentTargetCastle.Id - 1));
		}
		else if (code == 3 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_MAP_COMBATING;

			ShowGameLog("");
		}
		else
		{
			ShowAttackCastleLog(LOG_UNKNOWN_COMMAND);
		}
	}
	else if (CurrentPhase == PHASE_GAME_ENDING)
	{
		if (code == 1 && command.size() == 1)
		{
			CurrentPhase = PHASE_LOBBY_JOINING_READY;
			Lobby = LobbyRecord();
			ShowLobbyView();
			SendMessage(clientFD, RQ_RESET_GAME);
		}
		else {
			goto UnknownCommand;
		} ;
	}

	cout << CurrentPhase << endl;

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

	if (code == RS_UPDATE_TEAM_RESOURCE && CurrentPhase != PHASE_GAME_QUESTION_ANSWERING)
	{
		Resource = ResourceRecord::Deserialize(data[1]);

		WriteLineAt(2, GetResourceLine());
	}

	if (code == RS_UPDATE_GAME_START)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(LOG_NONE);
	}
	
	if (code == RS_RESET_GAME)
	{
		Tick = 0;
		JoinRequestAmount = 0;
		TeamInviteRequest = 0;
		PendingJoinTick = 0;
		PendingInviteTick = 0;
		Log = FG_GREEN "";
		Team = 0;
		Map = MapRecord();
		Resource = ResourceRecord();
		CurrentQuestionSpot = -1;
		CurrentQuestionIsCastle = false;
		CurrentQuestion = QuestionEntity();
		QuestionTimeOut = 30;
		CurrentPhase = PHASE_LOBBY_JOINING_READY;
		ShowLobbyView();
	}
	else if (code == RS_START_GAME_F_NOT_ENOUGH_TEAMS)
	{
		ShowLobbyLog(FG_RED "Start game failed: Not enough teams!");
	}
	else if (code == RS_UPDATE_GAME_MAP)
	{
		// Map = MapRecord::Deserialize(data[1]);

		// ShowGameView();
		if (CurrentPhase == PHASE_GAME_MAP_COMBATING || CurrentPhase == PHASE_GAME_MAP_IDLING)
		{	
			Map = MapRecord::Deserialize(data[1]);

			ShowGameView();
		}	
	}

	else if (code == RS_UPDATE_ATTACK_VIEW)
	{
		json j = json::parse(data[1]);

		CurrentTargetCastle.Id = j.value("TargetCastle", 0);
		CurrentTargetCastle.OwnerTeam = j.value("OwnerTeam", 0);
		CurrentTargetCastle.Defense = j.value("Defense", 0);
		OwnInventory.Balista = j.value("Balista", 0);
		OwnInventory.Catapult = j.value("Catapult", 0);
		OwnInventory.Canon = j.value("Canon", 0);

		ShowAttackCastleView();		

	}

	else if (code == RS_OCCUPY_REQUIRE_ANSWER_QUESTION)
	{
		if (data.size() >= 4) {
			try {
				int index = stoi(data[1]);
				CurrentQuestionSpot = index;
				CurrentQuestionIsCastle = (data[2] == "castle");
				for (size_t i = 4; i < data.size(); ++i) data[3] += string(" ") + data[i];
				json j = json::parse(data[3]);
				
				CurrentQuestion.content = j.value("content", string(""));
				CurrentQuestion.answers.clear();
				if (j.contains("answers") && j["answers"].is_array()) {
					for (const auto& a : j["answers"]) CurrentQuestion.answers.push_back(a.get<string>());
				}
				CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
				ShowQuestionView(CurrentQuestion);
			} catch(...) {
				ShowQuestionLog(CurrentQuestion,FG_RED "Failed to parse question payload");
			}
		} else {
			ShowQuestionLog(CurrentQuestion,FG_RED "Malformed question payload");
		}
	}
	
	else if (code == RS_OCCUPY_SPOT_S || code == RS_ANSWER_QUESTION_SPOT_S )
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_GREEN "Successfully occupied spot!");
	}

	else if (code == RS_OCCUPY_CASTLE_S || code == RS_ANSWER_QUESTION_CASTLE_S)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_GREEN "Successfully occupied castle!");
	}
	
	else if (code == RS_ANSWER_QUESTION_F_TOO_SLOW)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_RED "Too slow! Other team has occupied successfully");
	}
	
	else if (code == RS_OCCUPY_SPOT_F_INVALID_SPOT)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog (FG_RED "Not exist spot!");
	}
	
	else if (code == RS_OCCUPY_CASTLE_F_INVALID_CASTLE)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog (FG_RED "Not exist castle!");
	}
	
	else if (code == RS_ANSWER_QUESTION_F_WRONG_ANSWER)
	{
		ShowQuestionLog(CurrentQuestion,FG_RED "Wrong answer");
	} 
	
	else if (code == RS_OCCUPY_SPOT_F_SPOT_OCCUPIED)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_RED "Spot slot has been occupied!");
	}
	
	else if (code == RS_OCCUPY_SPOT_F_FULL_OF_SLOT)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_RED "Both slots of this resource has been occupied!");
	}
	
	else if (code == RS_OCCUPY_CASTLE_F_CASTLE_OCCUPIED)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_RED "Castle has been occupied!");
	}
	
	else if (code == RS_OCCUPY_CASTLE_F_SLOT_FULL)
	{
		CurrentPhase = PreviousPhase;

		ShowGameLog(FG_RED "Occupy only one castle at a time!");
	}
	
	else if (code == RS_UPDATE_START_COMBAT)
	{
		CurrentPhase = PHASE_GAME_MAP_COMBATING;

		ShowGameView();
		ShowGameLog(FG_YELLOW "Start combating. FIGHT!");
	}
	
	else if (code == RS_TARGET_TEAM_RESOURCE)
	{
		if (data.size() >= 2)
		{
			for (size_t i = 2; i < data.size(); ++i) data[1] += string(" ") + data[i];
			json j = json::parse(data[1]);
			CurrentTargetTeamResource.TargetTeamID = j.value("TargetTeamID", -1);
			CurrentTargetTeamResource.Wood = j.value("Wood", -1);
			CurrentTargetTeamResource.Rock = j.value("Rock", -1);
			CurrentTargetTeamResource.Iron = j.value("Iron", -1);
			CurrentTargetTeamResource.Gold = j.value("Gold", -1);

			string result = "Team " + to_string(CurrentTargetTeamResource.TargetTeamID + 1) + " have " + to_string(CurrentTargetTeamResource.Wood) + 
                            " wood unit " + to_string(CurrentTargetTeamResource.Rock) + " rock unit " + to_string(CurrentTargetTeamResource.Iron) + 
							" iron unit and " + std::to_string(CurrentTargetTeamResource.Gold) + " gold unit";  

			ShowGameLog(result);
 		}
	}

	else if (code == RS_TARGET_TEAM_NOT_FOUND)
	{
		ShowGameLog(FG_RED "Team not found");
	}
	
	else if (code == RS_TARGET_CASTLE_INFO)
	{
		if (data.size() >= 2)
		{
			for (size_t i = 2; i < data.size(); ++i) data[1] += string(" ") + data[i];
			json j = json::parse(data[1]);

			CurrentTargetCastle.Id = j.value("id", -1);
			CurrentTargetCastle.Defense = j.value("defense", 0);
			
			string result = "Castle " + to_string(CurrentTargetCastle.Id + 1) + " have " + to_string(CurrentTargetCastle.Defense) + " defense point!";   

			ShowGameLog(result);
 		}
	}
	
	else if (code == RS_ATTACK_CASTLE_REQUIRE_ANSWER_Q)
	{
		if (data.size() >= 4) {
			try {
				int index = stoi(data[1]);
				CurrentQuestionSpot = index;
				CurrentQuestionIsCastle = (data[2] == "castle") + 1;
				for (size_t i = 4; i < data.size(); ++i) data[3] += string(" ") + data[i];
				json j = json::parse(data[3]);
				
				CurrentQuestion.content = j.value("content", string(""));
				CurrentQuestion.answers.clear();
				if (j.contains("answers") && j["answers"].is_array()) {
					for (const auto& a : j["answers"]) CurrentQuestion.answers.push_back(a.get<string>());
				}
				CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
				ShowQuestionView(CurrentQuestion);
			} catch(...) {
				ShowQuestionLog(CurrentQuestion,FG_RED "Failed to parse question payload");
			}
		} else {
			ShowQuestionLog(CurrentQuestion,FG_RED "Malformed question payload");
		}
	}

	else if (code == RS_ATTACK_CASTLE_F_SELF_ATTACK)
	{
		ShowGameLog(FG_RED "Self attack!");
	}

	else if (code == RS_ATTACK_CASTLE_FULL_PERMISSION)
	{
		CurrentPhase = PHASE_GAME_CASTLE_ATTACKING;

		json j = json::parse(data[1]);

		CurrentTargetCastle.Id = j.value("TargetCastle", 0);
		CurrentTargetCastle.OwnerTeam = j.value("OwnerTeam", 0);
		CurrentTargetCastle.Defense = j.value("Defense", 0);
		OwnInventory.Balista = j.value("Balista", 0);
		OwnInventory.Catapult = j.value("Catapult", 0);
		OwnInventory.Canon = j.value("Canon", 0);

		ShowAttackCastleView();
	}

	else if (code == RS_ATTACK_CASTLE_S)
	{
		CurrentPhase = PHASE_GAME_MAP_COMBATING;

		ShowGameLog(FG_GREEN "Attack successfully");
	}

	else if (code == RS_ATTACK_CASTLE_F_NOT_ENOUGH_POWER)
	{
		ShowAttackCastleLog(FG_RED "Not enough power");
	}

	else if (code == RS_ATTACK_CASTLE_F_NOT_ENOUGH_WEAPON)
	{
		ShowAttackCastleLog(FG_RED "You don't have that many weapons");
	}

	else if (code == RS_OWN_WEAPON_ITEMS)
	{
		if (data.size() >= 2)
		{
			for (size_t i = 2; i < data.size(); ++i) data[1] += string(" ") + data[i];
			json j = json::parse(data[1]);

			OwnInventory.Balista = j.value("Balista", 0);
			OwnInventory.Catapult = j.value("Catapult", 0);
			OwnInventory.Canon = j.value("Canon", 0);   

			CurrentPhase = PHASE_GAME_VIEW_INVENTORY;
			
			ShowInventoryView();
 		}
	}

	else if (code == RS_GAME_END)
	{
		ResultRecord result = ResultRecord::Deserialize(data[1]);
		CurrentPhase = PHASE_GAME_ENDING;
		ShowResultView(result);
	}
	else if (code == RS_UPDATE_END_GAME)
	{
		CurrentPhase = PHASE_GAME_ENDING;
		ShowResultView(ResultRecord::Deserialize(data[1]));
	}
	else if (code == RS_SHOP_EQUIPMENT_F_LACK_RESOURCE )
	{
		if (CurrentPhase == PHASE_GAME_SHOPING_WEAPON)
		{
			ShowAttackShopLog(FG_RED "Not enough resoucres !");
		}
		else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
		{
			ShowDefenseShopLog(FG_RED "Not enough resoucres !");
		}
	}
	
	else if (code == RS_SHOP_EQUIPMENT_F_ZERO_CASTLE)
	{
		ShowDefenseShopLog(FG_RED "You don't have any castle");
	}
	
	else if (code == RS_SHOP_EQUIPMENT_F_NOT_OWN_CASTLE)
	{
		ShowDefenseShopLog(FG_RED "This castle belongs to another team");
	}
	
	else if (code == RS_SHOP_EQUIPMENT_S)
	{
		if (CurrentPhase == PHASE_GAME_SHOPING_WEAPON)
		{
			ShowAttackShopLog(FG_GREEN "Buy success");

			CurrentPhase = PHASE_GAME_SHOPING_WEAPON;
		}
		else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
		{
			ShowDefenseShopLog(FG_GREEN "Buy success");

			CurrentPhase = PHASE_GAME_SHOPING_DEFENSE;
		}
	}

	else if (code == RS_DEBUG)
	{
		ShowAttackCastleLog(FG_RED "Not thing happens!");
	}
}
#endif
