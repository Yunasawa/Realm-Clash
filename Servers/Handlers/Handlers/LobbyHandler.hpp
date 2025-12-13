#ifndef SERVER_HANDLER_LOBBY
#define SERVER_HANDLER_LOBBY

void StartTick(int clientFD, int duration, const function<void(int, int)>& onTick)
{
    {
        lock_guard<mutex> lock(SessionsMutex);
        Sessions[clientFD].Initialize();
        Sessions[clientFD].Counting.store(true);
    }

    thread([clientFD, duration, onTick]()
    {
        for (int i = 0; i < duration; i++)
        {
            bool counting = false;
            {
                lock_guard<mutex> lock(SessionsMutex);
                counting = Sessions[clientFD].Counting.load();
            }

            if (!counting) break;

            if (onTick) onTick(clientFD, i);

            this_thread::sleep_for(chrono::seconds(1));
        }

        {
            lock_guard<mutex> lock(SessionsMutex);
            Sessions[clientFD].Counting.store(false);
        }

    }).detach();
}


void HandleUpdateLobby(int clientFD)
{
    SendMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize());
}

void HandleJoinTeam(int clientFD, string data)
{
    auto teamID = atoi(data.c_str()) - 1;
	auto& team = Lobby.Teams[teamID];

	int memberCount = team.CountMember();

	if (memberCount >= 3)
	{
		SendMessage(clientFD, string(RS_JOIN_TEAM_F_TEAM_FULL));
        WriteLog(LogType::Failure, clientFD, "JOIN_TEAM_F_TEAM_FULL");
	}
    else
    {
		auto& account = Accounts[Clients[clientFD]];

        if (memberCount == 0)
        {
            auto assignedSlot = team.AssignFreeSlot(account.ID);

            if (assignedSlot != -1)
            {
                if (RoomLeader == 0)
                {
                    RoomLeader = account.ID;
                    account.IsRoomLeader = true;
                }

                if (assignedSlot == 0)
                {
                    account.IsTeamLeader = true;
                }

                account.Team = teamID;
                JoinedMembers.push_back(account.ID);

            }

			auto serializedLobby = Lobby.Serialize();

            SendMessage(clientFD, string(RS_JOIN_TEAM_S) + " " + serializedLobby);
            BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

            WriteLog(LogType::Success, clientFD, "JOIN_TEAM_S " + serializedLobby);
        }
        else
        {
			auto teamLeaderID = team.Members[0];
			auto teamLeaderFD = GetValueByKey(Clients, teamLeaderID);

			team.JoinRequests.push_back(account.ID);
			
            SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
            WriteLog(LogType::Success, teamLeaderFD, "UPDATE_JOIN_REQUEST " + to_string(team.JoinRequests.size()));
        
            StartTick(clientFD, 10, [](int clientFD, int tick)
            {
			    SendMessage(clientFD, string(RS_UPDATE_PENDING_JOIN) + " " + to_string(tick));
            });
        }
    }
}

AccountEntity* FindAccountByName(const std::string& myName)
{
    for (auto& [id, acc] : Accounts)
    {
        if (acc.Name == myName) return &acc;
    }

    return nullptr;
}

void HandleAddMember(int clientFD, string name)
{
    if (auto* account = FindAccountByName(name))
    {
        auto clientAccount = Accounts[Clients[clientFD]];
		auto& team = Lobby.Teams[clientAccount.Team];
        auto freeSlotAmount = team.CountFreeSlot();

        if (freeSlotAmount == 0)
        {
            SendMessage(clientFD, string(RS_ADD_MEMBER_F_TEAM_FULL));
            WriteLog(LogType::Failure, clientFD, "ADD_MEMBER_F_TEAM_FULL");
        }
        else
        {
            // auto assignedSlot = team.AssignFreeSlot(account->ID);
            team.AssignFreeSlot(account->ID);

            account->Team = clientAccount.Team;
            JoinedMembers.push_back(account->ID);
            team.JoinRequests.push_back(account->ID);

			auto serializedLobby = Lobby.Serialize();

            SendMessage(clientFD, string(RS_ADD_MEMBER_S) + " " + serializedLobby);
            WriteLog(LogType::Success, clientFD, "ADD_MEMBER_S " + serializedLobby);

            BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

            auto memberFD = GetValueByKey(Clients, account->ID);
            SendMessage(memberFD, string(RS_UPDATE_TEAM_ROLE));
            WriteLog(LogType::Success, memberFD, "UPDATE_TEAM_ROLE");
        }
    }
    else
    {
        SendMessage(clientFD, string(RS_ADD_MEMBER_F_NOT_FOUND) + " " + name);
        WriteLog(LogType::Failure, clientFD , "ADD_MEMBER_F_NOT_FOUND " + name);
    }
}

void HandleExitTeam(int clientFD)
{
    auto account = Accounts[Clients[clientFD]];
	auto& team = Lobby.Teams[account.Team];

    Lobby.RemoveMember(account.Team, account.ID);
    JoinedMembers.erase(remove(JoinedMembers.begin(), JoinedMembers.end(), account.ID), JoinedMembers.end());
    team.JoinRequests.erase(remove(team.JoinRequests.begin(), team.JoinRequests.end(), account.ID), team.JoinRequests.end());

    SendMessage(clientFD, string(RS_EXIT_TEAM_S) + " " + Lobby.Serialize());
    WriteLog(LogType::Success, clientFD, "EXIT_TEAM_S");

    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
}

#endif