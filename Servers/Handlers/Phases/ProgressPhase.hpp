#ifndef SERVER_HANDLER_PHASE_EXIT
#define SERVER_HANDLER_PHASE_EXIT

void HandleQuitPhase(int clientFD)
{
    auto& account = Accounts[Clients[clientFD]];
    auto& team = Lobby.Teams[account.Team];

    Lobby.RemoveMember(account.Team, account.ID);
    JoinedMembers.erase(remove(JoinedMembers.begin(), JoinedMembers.end(), account.ID), JoinedMembers.end());
    team.JoinRequests.erase(remove(team.JoinRequests.begin(), team.JoinRequests.end(), account.ID), team.JoinRequests.end());

    WriteLog(LogType::Success, clientFD, "QUIT GAME");

    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);

	auto accountID = Clients[clientFD];

	Accounts.erase(accountID);
	Clients.erase(clientFD);
}

#endif