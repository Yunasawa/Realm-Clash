#ifndef SERVER_HANDLER_GAME
#define SERVER_HANDLER_GAME

void RequestStopServerTick()
{
	ServerTicking.store(false);
}

void HandleStartGame(int clientFD)
{
	//if (Lobby.CountTeam() < 3)
	//{
	//	WriteLog(LogType::Failure, clientFD, "START GAME : Not enough teams.", "Current team: " + to_string(Lobby.CountTeam()));
	//	SendMessage(clientFD, string(RS_START_GAME_F_NOT_ENOUGH_TEAMS));
	//	
	//	return;
	//}

    Group.CreateTeam();

    WriteLog(LogType::Success, clientFD, "START GAME");
	BroadcastToClient(clientFD, string(RS_UPDATE_GAME_START), true);

    StartTickOnServer(
        [](int tick)
        {
            if (tick % RESOURCE_UPDATE_TICK == 0)
            {
                Group.UpdateResource();

                auto json = Group.SerializeResource();

                WriteLog(LogType::Update, -1, "UPDATE RESOURCE", json);
                BroadcastToClient(-1, string(RS_UPDATE_TEAM_RESOURCE) + " " + json);
            }

			BroadcastToClient(-1, string(RS_UPDATE_GAME_TICK) + " " + to_string(tick));
        },
        []()
        {
            cout << "Tick end" << endl;
        }
    );

}

void HandleOccupySpot(int clientFD, const string& data)
{
    auto request = SpotOccupationRecord::Deserialize(data);

    WriteLog(LogType::Request, clientFD, "OCCUPY SPOT", request.Capture());

    auto account = Accounts[Clients[clientFD]];
    auto& spot = Map.Spots[request.Spot];
    auto& slot = spot.Slots[request.Type];

    if (slot != -1)
    {
        WriteLog(LogType::Failure, clientFD, "OCCUPY SLOT : Slot occupied", request.Capture());
        SendMessage(clientFD, string(RS_OCCUPY_SPOT_F_SPOT_OCCUPIED));

        return;
    }

    auto& team = Group.Teams[account.GameTeam];
    auto freeSlot = team.GetFreeSlot(request.Type);

    if (freeSlot == -1)
    {
        WriteLog(LogType::Failure, clientFD, "OCCUPY SLOT : Both slots are occupied", request.Capture());
        SendMessage(clientFD, string(RS_OCCUPY_SPOT_F_FULL_OF_SLOT));

        return;
    }

    team.SpotSlots[request.Type][freeSlot] = request.Spot;
    Map.Spots[request.Spot].Slots[request.Type] = account.GameTeam;

    WriteLog(LogType::Success, clientFD, "OCCUPY SPOT", request.Capture());
    SendMessage(clientFD, string(RS_OCCUPY_SPOT_S));
    BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);
}

#endif