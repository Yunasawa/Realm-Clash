#ifndef SERVER_HANDLER_PHASE_LOBBY
#define SERVER_HANDLER_PHASE_LOBBY

void HandleLobbyPhase(int clientFD, const string& code, const vector<string>& parts)
{
    if (code == RQ_UPDATE_LOBBY)
    {
        HandleUpdateLobby(clientFD);
    }
    else if (code == RQ_JOIN_TEAM)
    {
        HandleJoinTeam(clientFD, parts[1]);
    }
    else if (code == RQ_ADD_MEMBER)
    {
        HandleAddMember(clientFD, parts[1]);
    }
    else if (code == RQ_EXIT_TEAM)
    {
        HandleExitTeam(clientFD);
    }
}

#endif