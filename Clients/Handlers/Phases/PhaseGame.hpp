#ifndef CLIENT_HANDLER_PHASE_GAME
#define CLIENT_HANDLER_PHASE_GAME

#include "../../../Commons/CommonIncluding.hpp"

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

				CurrentPhase = PHASE_GAME_QUESTION_ANSWERING;
			}
			else
			{
				CurrentPhase = PHASE_GAME_MAP_IDLING;

				ShowGameLog(FG_RED "Trying to occupy wrong castle!");
			}
		}
		else goto UnknownCommand;
	}

	else if (CurrentPhase == PHASE_GAME_QUESTION_ANSWERING)
	{
			if (code == 1 && command.size() == 2) {
				// Answer format: 1 <answerID>
				if (CurrentQuestionIsCastle) {
					SendMessage(clientFD, string(RQ_OCCUPY_ANSWER_CASTLE) + " " + command[1]);
				} else {
					SendMessage(clientFD, string(RQ_OCCUPY_ANSWER_SPOT) + " " + command[1]);
				}
				return;
			}
			else if (code == 2 && command.size() == 1){
				CurrentPhase = PHASE_GAME_MAP_IDLING;

				ShowGameLog("");
			}

			else {
				goto UnknownCommand;
			}
	}

	else if (CurrentPhase == PHASE_GAME_MAP_COMBATING)
	{
		if (code == 1 && command.size() == 1) /*Open shop*/
		{
			CurrentPhase = PHASE_GAME_SHOPING_WEAPON;

			ShowAttackShopView();
		}
		else if (code == 2 && command.size() == 2) /*Attack castle*/
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
			CurrentPhase = PHASE_GAME_SHOPING_DEFENSE;

			ShowDefenseShopView();
		}
		else if (code == 3 && command.size() == 1)
		{
			CurrentPhase = PHASE_GAME_MAP_COMBATING;

			ShowGameView();
		}
		else if (code == 4 && command.size() == 1)
		{
			/* Bổ sung inventory view */
		}
		else goto UnknownCommand;
	}
	else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
	{
		if (code == 1 && command.size() == 4)
		{
			auto cart = CartEntity
			{
				.Type = 1,
				.Equipment = stoi(command[1]),
				.Amount = stoi(command[2]),
				.TargetCastle = stoi(command[3])
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

			ShowGameView();
		}
		else if (code == 4 && command.size() == 1)
		{
			/* Bổ sung inventory view */
		}
		else goto UnknownCommand;
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
	else if (code == RS_START_GAME_F_NOT_ENOUGH_TEAMS)
	{
		ShowLobbyLog(FG_RED "Start game failed: Not enough teams!");
	}
	else if (code == RS_UPDATE_GAME_MAP)
	{
		Map = MapRecord::Deserialize(data[1]);

		ShowGameView();
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
				// StartQuestionCountdown(CurrentQuestion, QuestionTimeOut);
			} catch(...) {
				ShowQuestionLog(CurrentQuestion,FG_RED "Failed to parse question payload");
			}
		} else {
			ShowQuestionLog(CurrentQuestion,FG_RED "Malformed question payload");
		}
	}
	
	else if (code == RS_OCCUPY_SPOT_S || code == RS_ANSWER_QUESTION_SPOT_S )
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_GREEN "Successfully occupied spot!");
	}

	else if (code == RS_OCCUPY_CASTLE_S || code == RS_ANSWER_QUESTION_CASTLE_S)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_GREEN "Successfully occupied castle!");
	}
	else if (code == RS_ANSWER_QUESTION_F_TOO_SLOW)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_RED "Too slow! Other team has occupied successfully");
	}
	else if (code == RS_OCCUPY_SPOT_F_INVALID_SPOT)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog (FG_RED "Not exist spot!");
	}
	else if (code == RS_OCCUPY_CASTLE_F_INVALID_CASTLE)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog (FG_RED "Not exist castle!");
	}
	else if (code == RS_ANSWER_QUESTION_F_WRONG_ANSWER)
	{
		ShowQuestionLog(CurrentQuestion,FG_RED "Wrong answer");
	} 
	else if (code == RS_OCCUPY_SPOT_F_SPOT_OCCUPIED)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_RED "Spot slot has been occupied!");
	}
	else if (code == RS_OCCUPY_SPOT_F_FULL_OF_SLOT)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_RED "Both slots of this resource has been occupied!");
	}
	else if (code == RS_OCCUPY_CASTLE_F_CASTLE_OCCUPIED)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_RED "Castle has been occupied!");
	}
	else if (code == RS_OCCUPY_CASTLE_F_SLOT_FULL)
	{
		CurrentPhase = PHASE_GAME_MAP_IDLING;

		ShowGameLog(FG_RED "Occupy only one castle at a time!");
	}
	else if (code == RS_UPDATE_START_COMBAT)
	{
		CurrentPhase = PHASE_GAME_MAP_COMBATING;

		ShowGameView();
		ShowGameLog(FG_YELLOW "Start combating. FIGHT!");
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
}

#endif