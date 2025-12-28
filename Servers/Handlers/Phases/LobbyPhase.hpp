#ifndef SERVER_HANDLER_PHASE_LOBBY
#define SERVER_HANDLER_PHASE_LOBBY

void HandleLobbyPhase(int clientFD, const string& code, const vector<string>& command)
{
    auto account = Accounts[Clients[clientFD]]; /*key = clientFD, value = accountID*/

    if (code == RQ_UPDATE_LOBBY)
    {
        HandleUpdateLobby(clientFD);
    }
    else if (code == RQ_JOIN_TEAM)
    {
        if (account.LobbyTeam != -1) return;

        WriteLog(LogType::Request, clientFD, "JOIN TEAM", "Team: " + command[1]);
        HandleJoinTeam(clientFD, command[1]);
    }
    else if (code == RQ_CANCEL_JOINING)
    {
        /*Chưa xử lí việc client muốn gia nhập 1 team rồi lại thôi*/
    }
    else if (code == RQ_ADD_MEMBER)
    {
		if (account.IsTeamLeader == false) return;

        WriteLog(LogType::Request, clientFD, "ADD MEMBER", "Member: " + command[1]);
        HandleAddMember(clientFD, command[1]);
    }
    else if (code == RQ_EXIT_TEAM)
    {
		if (account.LobbyTeam == -1) return;

        WriteLog(LogType::Request, clientFD, "EXIT TEAM");
        HandleExitTeam(clientFD);
    }
    else if (code == RQ_ACCEPT_PARTICIPATION)
    {
        WriteLog(LogType::Request, clientFD, "ACCEPT PARTICIPATION");
        HandleAcceptParticipation(clientFD);
    }
    else if (code == RQ_INVITE_MEMBER)
    {
        if (account.IsTeamLeader == true) return;

		WriteLog(LogType::Request, clientFD, "INVITE MEMBER", "Member: " + command[1]);
        HandleInviteMember(clientFD, command[1]);
    }
    else if (code == RQ_ACCEPT_INVITATION)
    {
		WriteLog(LogType::Request, clientFD, "ACCEPT INVITATION");
		HandleAcceptInvitation(clientFD);
    }
}

#endif