#ifndef SERVER_HANDLER_LOBBY
#define SERVER_HANDLER_LOBBY

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
        WriteLog(LogType::Failure, clientFD, "JOIN TEAM : Team is full");
	}
    else
    {
		auto& account = Accounts[Clients[clientFD]];

        if (memberCount == 0)
        {
            if (RoomLeader == 0)
            {
				RoomLeader = account.ID;
				account.IsRoomLeader = true;
            }

            account.IsTeamLeader = true;
            account.Team = teamID;
            
			team.Members[0].ID = account.ID;
            JoinedMembers.push_back(account.ID);

			auto serializedLobby = Lobby.Serialize();

            SendMessage(clientFD, string(RS_JOIN_TEAM_S) + " " + serializedLobby);
            BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

            WriteLog(LogType::Success, clientFD, "JOIN TEAM", Lobby.Capture());
        }
        else
        {
            if (team.CountFreeSlot() == (int)team.JoinRequests.size())
            {
				SendMessage(clientFD, string(RS_JOIN_TEAM_F_REQUEST_FULL));
            }
            else
            {
                auto teamLeaderFD = GetValueByKey(Clients, team.Members[0].ID);
                auto accountID = account.ID;

                team.JoinRequests.push_back(accountID);

                SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
                WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));



                StartTick(clientFD, TICK_JOIN_REQUEST,
                    [accountID](int clientFD, int tick)
                    {
                        SendMessage(clientFD, string(RS_UPDATE_PENDING_JOIN) + " " + to_string(tick));
                    },
                    [accountID, teamID, teamLeaderFD](int clientFD)
                    {
                        lock_guard<mutex> lock(SessionsMutex);
                        auto& team = Lobby.Teams[teamID];
                        team.JoinRequests.erase(
                            remove(team.JoinRequests.begin(), team.JoinRequests.end(), accountID),
                            team.JoinRequests.end()
                        );

                        SendMessage(clientFD, string(RS_JOIN_TEAM_F_REQUEST_REJECTED));
                        WriteLog(LogType::Failure, clientFD, "JOIN TEAM : Request rejected");

                        SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
                        WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));
                    });
            }
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
            WriteLog(LogType::Failure, clientFD, "ADD MEMBER : Team is full");
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
            WriteLog(LogType::Success, clientFD, "ADD MEMBER", Lobby.Capture());

            BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

            auto memberFD = GetValueByKey(Clients, account->ID);
            SendMessage(memberFD, string(RS_UPDATE_TEAM_ROLE));
            WriteLog(LogType::Success, memberFD, "UPDATE TEAM ROLE");
        }
    }
    else
    {
        SendMessage(clientFD, string(RS_ADD_MEMBER_F_NOT_FOUND) + " " + name);
        WriteLog(LogType::Failure, clientFD , "ADD MEMBER : Member not found", name);
    }
}

void HandleExitTeam(int clientFD)
{
    auto& account = Accounts[Clients[clientFD]];
	auto& team = Lobby.Teams[account.Team];

    Lobby.RemoveMember(account.Team, account.ID);
    JoinedMembers.erase(remove(JoinedMembers.begin(), JoinedMembers.end(), account.ID), JoinedMembers.end());
    team.JoinRequests.erase(remove(team.JoinRequests.begin(), team.JoinRequests.end(), account.ID), team.JoinRequests.end());

    SendMessage(clientFD, string(RS_EXIT_TEAM_S) + " " + Lobby.Serialize());
    WriteLog(LogType::Success, clientFD, "EXIT TEAM");

    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
}

#endif