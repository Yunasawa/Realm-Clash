#ifndef SERVER_HANDLER_PHASE_LOBBY
#define SERVER_HANDLER_PHASE_LOBBY

void HandleLobbyPhase(int clientFD, const string& code, const vector<string>& parts)
{
    auto account = Accounts[Clients[clientFD]];

    if (code == RQ_UPDATE_LOBBY)
    {
        HandleUpdateLobby(clientFD);
    }
    else if (code == RQ_JOIN_TEAM)
    {
        if (account.Team != 0) return;

        HandleJoinTeam(clientFD, parts[1]);
    }
    else if (code == RQ_CANCEL_JOINING)
    {

    }
    else if (code == RQ_ADD_MEMBER)
    {
		if (account.IsTeamLeader == false) return;

        HandleAddMember(clientFD, parts[1]);
    }
    else if (code == RQ_EXIT_TEAM)
    {
		if (account.Team == 0) return;

        HandleExitTeam(clientFD);
    }
}

#endif