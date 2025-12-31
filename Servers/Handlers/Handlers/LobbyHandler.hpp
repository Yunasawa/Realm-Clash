#ifndef SERVER_HANDLER_LOBBY
#define SERVER_HANDLER_LOBBY

void HandleUpdateLobby(int clientFD)
{
    SendMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize());
}

void HandleJoinTeam(int clientFD, const string& data)
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
            account.LobbyTeam = teamID;
            
			team.Members[0].ID = account.ID;
            JoinedMembers.push_back(account.ID);

			auto serializedLobby = Lobby.Serialize();

            SendMessage(clientFD, string(RS_JOIN_TEAM_S) + " " + serializedLobby);
            BroadcastToClient(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

            WriteLog(LogType::Success, clientFD, "JOIN TEAM", Lobby.Capture());
        }
        else
        {
            if (team.CountFreeSlot(true) == 0)
            {
                WriteLog(LogType::Failure, clientFD, "JOIN TEAM : Request list is full");
				SendMessage(clientFD, string(RS_JOIN_TEAM_F_REQUEST_FULL));
            }
            else
            {
                auto teamLeaderFD = GetValueByKey(Clients, team.Members[0].ID);
                auto accountID = account.ID;

                account.PendingTeam = teamID;

                team.JoinRequests.push_back(accountID);

				team.AssignFreeSlot(accountID, true);

                SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
                WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));

				BroadcastToClient(teamLeaderFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);

                StartTickOnClient(clientFD, DURATION_JOIN_REQUEST,
                    [accountID](int clientFD, int tick)
                    {
                        SendMessage(clientFD, string(RS_UPDATE_PENDING_JOIN) + " " + to_string(tick));
                    },
                    [accountID, teamID, teamLeaderFD](int clientFD)
                    {
                        auto& account = Accounts[accountID];

                        account.PendingTeam = -1;

                        lock_guard<mutex> lock(SessionsMutex);
                        auto& team = Lobby.Teams[teamID];
                        team.JoinRequests.erase(
                            remove(team.JoinRequests.begin(), team.JoinRequests.end(), accountID),
                            team.JoinRequests.end()
                        );

                        Lobby.RemoveMember(teamID, accountID);

                        BroadcastToClient(teamLeaderFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);

                        SendMessage(clientFD, string(RS_JOIN_TEAM_F_REQUEST_EXPIRED));
                        WriteLog(LogType::Failure, clientFD, "JOIN TEAM : Request rejected");

                        SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
                        WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));
                    });
            }
        }
    }
}

void HandleCancelJoining(int clientFD)
{

}

AccountEntity* FindAccountByName(const string& myName)
{
    for (auto& [id, acc] : Accounts)
    {
        if (acc.Name == myName) return &acc;
    }

    return nullptr;
}

void HandleAddMember(int clientFD, const string& name)
{
    if (auto* account = FindAccountByName(name))
    {
        auto& adderAccount = Accounts[Clients[clientFD]];
		auto& team = Lobby.Teams[adderAccount.LobbyTeam];
        
        auto* member = team.CheckIfPending(account->ID);

        if (member == nullptr)
        {
            auto freeSlot = team.CountFreeSlot(true);

            if (freeSlot == 0)
            {
                WriteLog(LogType::Failure, clientFD, "ADD MEMBER : Team is full");
                SendMessage(clientFD, string(RS_ADD_MEMBER_F_TEAM_FULL));
            }
            else
            {
                team.AssignFreeSlot(account->ID);
            }
        }
        else
        {
            member->IsRequestPending = false;

            account->PendingTeam = -1;
        }

        account->LobbyTeam = adderAccount.LobbyTeam;
        JoinedMembers.push_back(account->ID);

        auto serializedLobby = Lobby.Serialize();

        SendMessage(clientFD, string(RS_ADD_MEMBER_S) + " " + serializedLobby);
        WriteLog(LogType::Success, clientFD, "ADD MEMBER", Lobby.Capture());

        BroadcastToClient(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + serializedLobby, false);

        auto memberFD = GetValueByKey(Clients, account->ID);
        SendMessage(memberFD, string(RS_UPDATE_TEAM_ROLE));
        WriteLog(LogType::Success, memberFD, "UPDATE TEAM ROLE");

        StopTickOnClient(memberFD,
            [memberFD](int clientFD)
            {
                SendMessage(memberFD, string(RS_JOIN_TEAM_S_REQUEST_ACCEPTED));
            });
    }
    else
    {
        SendMessage(clientFD, string(RS_ADD_MEMBER_F_MEMBER_NOT_FOUND) + " " + name);
        WriteLog(LogType::Failure, clientFD , "ADD MEMBER : Member not found", name);
    }
}

void HandleExitTeam(int clientFD)
{
    auto& account = Accounts[Clients[clientFD]];

    auto [roomLeader, teamLeader] = Lobby.RemoveMember(account.LobbyTeam, account.ID);
    JoinedMembers.erase(remove(JoinedMembers.begin(), JoinedMembers.end(), account.ID), JoinedMembers.end());

    SendMessage(clientFD, string(RS_EXIT_TEAM_S) + " " + Lobby.Serialize());
    WriteLog(LogType::Success, clientFD, "EXIT TEAM");

    BroadcastToClient(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);

	WriteLog(LogType::Success, clientFD, "UPDATE ROOM LIST : After exit team", Lobby.Capture());

    auto notify = [&](int leaderID)
        {
            if (leaderID == 0) return;

            auto fd = GetValueByKey(Clients, leaderID);
            SendMessage(fd, string(RS_UPDATE_TEAM_ROLE));
            WriteLog(LogType::Success, fd, "UPDATE TEAM ROLE");
        };

    if (roomLeader != 0 && roomLeader == teamLeader)
    {
        notify(roomLeader);
    }
    else
    {
        notify(roomLeader);
        notify(teamLeader);
    }
}

void HandleAcceptParticipation(int clientFD)
{
    auto& leader = Accounts[Clients[clientFD]];
    auto& team = Lobby.Teams[leader.LobbyTeam];

	if (team.JoinRequests.size() == 0)
	{
        WriteLog(LogType::Failure, clientFD, "ACCEPT PARTICIPATION : There is no joining request.");
        SendMessage(clientFD, string(RS_ACCEPT_PARTICIPATION_F_NO_REQUEST));
	}
    else
    {
		int requestAmount = team.JoinRequests.size();

		team.JoinRequests.clear();

        vector<int> acceptedMembers;

		for (auto& member : team.Members)
		{
			if (member.IsRequestPending)
			{
                member.IsRequestPending = false;

				auto& account = Accounts[member.ID];
				account.LobbyTeam = leader.LobbyTeam;
				account.PendingTeam = -1;

				JoinedMembers.push_back(member.ID);
				acceptedMembers.push_back(member.ID);
			}
		}

        WriteLog(LogType::Success, clientFD, "ACCEPT PARTICIPATION", "Request: " + to_string(requestAmount));
        SendMessage(clientFD, string(RS_ACCEPT_PARTICIPATION_S) + " " + to_string(requestAmount));
        
        BroadcastToClient(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);
    
		for (auto memberID : acceptedMembers)
		{
            auto memberFD = GetValueByKey(Clients, memberID);
            SendMessage(memberFD, string(RS_UPDATE_TEAM_ROLE));
            WriteLog(LogType::Success, memberFD, "UPDATE TEAM ROLE");

            StopTickOnClient(memberFD,
                [memberFD](int clientFD)
                {
                    SendMessage(memberFD, string(RS_JOIN_TEAM_S_REQUEST_ACCEPTED));
                });
		}
    }
}

void HandleInviteMember(int clientFD, const string& name)
{
    auto& inviter = Accounts[Clients[clientFD]];
    auto& team = Lobby.Teams[inviter.LobbyTeam];

    int memberCount = team.CountMember();

    if (memberCount >= 3)
    {
        WriteLog(LogType::Failure, clientFD, "INVITE MEMBER : Team is full");
        SendMessage(clientFD, string(RS_INVITE_MEMBER_F_TEAM_FULL));
    }
    else
    {
        if (auto* account = FindAccountByName(name))
        {
            if (team.CheckIfPending(account->ID) != nullptr)
            {
                WriteLog(LogType::Failure, clientFD, "INVITE MEMBER : Member requested to join", "Name: " + name);
                SendMessage(clientFD, string(RS_INVITE_MEMBER_F_JOIN_REQUESTED));

                return;
            }
            
            if (team.CountFreeSlot(true) == 0)
            {
                WriteLog(LogType::Failure, clientFD, "INVITE MEMBER : Request list is full");
                SendMessage(clientFD, string(RS_INVITE_MEMBER_F_REQUEST_FULL));

                return;
            }

            auto memberFD = GetValueByKey(Clients, account->ID);

			account->PendingInvitation = inviter.LobbyTeam;
            account->InvitorID = inviter.ID;

            WriteLog(LogType::Update, memberFD, "INVITE REQUEST", "From: " + inviter.Name + ", Team: " + to_string(inviter.LobbyTeam));
            SendMessage(memberFD, string(RS_UPDATE_INVITE_REQUEST) + " " + to_string(inviter.LobbyTeam));

            StartTickOnClient(clientFD, DURATION_INVITE_REQUEST,
                [](int clientFD, int tick)
                {
                    SendMessage(clientFD, string(RS_UPDATE_PENDING_INVITE) + " " + to_string(tick));
                },
                [memberFD, account](int clientFD)
                {
                    account->PendingInvitation = -1;
					account->InvitorID = 0;

					WriteLog(LogType::Failure, clientFD, "INVITE MEMBER : Invite request expired");
                    SendMessage(clientFD, string(RS_INVITE_MEMBER_F_REQUEST_EXPIRED));

					WriteLog(LogType::Update, memberFD, "INVITE REQUEST : Invite request expired");
                    SendMessage(memberFD, string(RS_UPDATE_INVITE_EXPIRED));
                });
        }
        else
        {
            WriteLog(LogType::Failure, clientFD, "INVITE MEMBER : Member not found", "Name: " + name);
            SendMessage(clientFD, string(RS_INVITE_MEMBER_F_MEMBER_NOT_FOUND));
        }
    }
}

void HandleKickMember(int clientFD, const string& name)
{
    auto& kicker = Accounts[Clients[clientFD]];
    
    if (kicker.IsTeamLeader == false)
    {
        return;
    }

    if (auto* account = FindAccountByName(name))
    {
		if (kicker.ID == account->ID)
		{
			WriteLog(LogType::Failure, clientFD, "KICK MEMBER : Cannot kick yourself", "Name: " + name);
			SendMessage(clientFD, string(RS_KICK_MEMBER_F_CANNOT_KICK_YOURSELF));
			return;
		}

		if (account->LobbyTeam != kicker.LobbyTeam)
		{
			WriteLog(LogType::Failure, clientFD, "KICK MEMBER : Member not in your team", "Name: " + name);
			SendMessage(clientFD, string(RS_KICK_MEMBER_F_NOT_IN_TEAM));
			return;
		}

        Lobby.RemoveMember(account->LobbyTeam, account->ID);
        JoinedMembers.erase(remove(JoinedMembers.begin(), JoinedMembers.end(), account->ID), JoinedMembers.end());

        WriteLog(LogType::Success, clientFD, "KICK MEMBER");
        SendMessage(clientFD, string(RS_KICK_MEMBER_S));

        auto kickedMemberFD = GetValueByKey(Clients, account->ID);
		SendMessage(kickedMemberFD, string(RS_UPDATE_KICK_OUT));

        BroadcastToClient(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);
    }
    else
    {
        WriteLog(LogType::Failure, clientFD, "KICK MEMBER : Member not found", "Name: " + name);
        SendMessage(clientFD, string(RS_KICK_MEMBER_F_MEMBER_NOT_FOUND));
    }
}

void HandleAcceptInvitation(int clientFD)
{
	auto& account = Accounts[Clients[clientFD]];
    auto teamID = account.PendingTeam;
    auto& team = Lobby.Teams[teamID];

    {
        auto teamLeaderFD = GetValueByKey(Clients, team.Members[0].ID);
        auto accountID = account.ID;

        account.PendingTeam = teamID;

        team.JoinRequests.push_back(accountID);

        team.AssignFreeSlot(accountID, true);

        SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
        WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));

        BroadcastToClient(teamLeaderFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);

        StartTickOnClient(clientFD, DURATION_JOIN_REQUEST,
            [accountID](int clientFD, int tick)
            {
                SendMessage(clientFD, string(RS_UPDATE_PENDING_JOIN) + " " + to_string(tick));
            },
            [accountID, teamID, teamLeaderFD](int clientFD)
            {
                auto& account = Accounts[accountID];

                account.PendingTeam = -1;

                lock_guard<mutex> lock(SessionsMutex);
                auto& team = Lobby.Teams[teamID];
                team.JoinRequests.erase(
                    remove(team.JoinRequests.begin(), team.JoinRequests.end(), accountID),
                    team.JoinRequests.end()
                );

                Lobby.RemoveMember(teamID, accountID);

                BroadcastToClient(teamLeaderFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);

                SendMessage(clientFD, string(RS_JOIN_TEAM_F_REQUEST_EXPIRED));
                WriteLog(LogType::Failure, clientFD, "JOIN TEAM : Request rejected");

                SendMessage(teamLeaderFD, string(RS_UPDATE_JOIN_REQUEST) + " " + to_string(team.JoinRequests.size()));
                WriteLog(LogType::Success, teamLeaderFD, "UPDATE JOIN REQUEST", "Request: " + to_string(team.JoinRequests.size()));
            });
    }

	auto invitorFD = GetValueByKey(Clients, account.InvitorID);

    StopTickOnClient(invitorFD,
        [invitorFD](int clientFD)
        {
			SendMessage(invitorFD, string(RS_INVITE_MEMBER_S));
		});
}

#endif