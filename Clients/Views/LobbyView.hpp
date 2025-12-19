#ifndef VIEW_LOBBY
#define VIEW_LOBBY

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ U0000001 ━┓
┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ ● 1 |                    |                    |                    ┃
┃ ● 2 |                    |                    |                    ┃
┃ ● 3 |                    |                    |                    ┃
┃ ● 4 |                    |                    |                    ┃
┃ ● 5 |                    |                    |                    ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • 1 <teamID>: Create/Join team                                     ┃
┃ • 1 <teamID>: Create/Join team                                     ┃
┃ • 1 <teamID>: Create/Join team                                     ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

string MakeLobbyTitle(const std::string& playerName)
{
    const int totalLen = 70;

    string bar = "━";

    int fillLen = totalLen - playerName.length() - 5;

    string fill;
    fill.reserve(fillLen * bar.length());

    for (int i = 0; i < fillLen; ++i)
    {
        fill += bar;
    }

    return "┏" + fill + " " + playerName + " ━┓";
}

string MakeLobbyMemberName(const MemberRecord& member)
{
	auto name = member.Name;
    if (member.ID == Account.ID) name = UNDERLINE + name + RESET;
    if (member.IsRequestPending) name = FG_GRAY + name;
    return name;
}

string MakeLobbyTeamLine(const TeamRecord& team, int teamID)
{
    for (int i = 0; i < 3; i++)
    {
        if (team.Members[i].ID == Account.ID)
        {
            Team = teamID;
        }
    }

    auto member0 = team.Members[0];
    auto isRoomLeader = team.Members[0].IsRoomLeader;

    auto rlMark = isRoomLeader ? " ⭐" : "";

    auto member0Text = member0.Name + rlMark;
    auto member0StyledText = MakeLobbyMemberName(member0) + rlMark;

    return 
    " | " + string(BOLD) + member0StyledText + RESET + string((isRoomLeader ? 20 : 19) - member0Text.length(), ' ') +
    "| " + MakeLobbyMemberName(team.Members[1]) + string(19 - team.Members[1].Name.length(), ' ') +
    "| " + MakeLobbyMemberName(team.Members[2]) + string(19 - team.Members[2].Name.length(), ' ') + "┃\n";
}

string LogLobbyCode(string code)
{
    if (code == RS_SIGN_UP_S)
    {
		return FG_GREEN "Sign up successfully!";
    }
    else if (code == RS_LOG_IN_S)
    {
		return FG_GREEN "Log in successfully!";
    }
    else if (code == RS_JOIN_TEAM_S)
    {
        return FG_GREEN "You joined the team!";
    }
    else if (code == RS_ADD_MEMBER_S)
    {
        return FG_GREEN "Added member to the team!";
    }
    else if (code == RS_JOIN_TEAM_F_TEAM_FULL)
    {
		return FG_RED "Join team failed: Team is full!";
    }
    else if (code == RS_UPDATE_JOIN_REQUEST)
    {
        return LOG_NONE;
    }
    else if (code == RS_UPDATE_PENDING_JOIN)
    {
		return FG_YELLOW "Joining request sent. Expired in " + to_string(PendingJoinTick) + ".";
    }
    else if (code == RS_JOIN_TEAM_F_REQUEST_EXPIRED)
    {
		return FG_RED "Joining request expired!";
    }
    else if (code == RS_EXIT_TEAM_S)
    {
		return FG_GREEN "You exited the team!";
    }
    else if (code == RS_ACCEPT_PARTICIPATION_S)
    {
		return FG_GREEN "You accepted all joining requests!";
    }
    else if (code == RS_UPDATE_INVITE_REQUEST)
    {
        return LOG_NONE;
    }
    else if (code == RS_UPDATE_INVITE_EXPIRED)
    {
        return LOG_NONE;
    }

    return Log;
}

string GetLobbyOption()
{
    if (CurrentPhase == PHASE_LOBBY_JOINING_READY)
    {
        return 
        "┃ • 1 <teamID>: Create/Join team   | • 2: Accept invitation          ┃\n";
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINING_PENDING)
    {
        return
        "┃ • 1: Cancel joining request                                        ┃\n";
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_MEMBER)
    {
        return 
        "┃ • 1 <member>: Invite to team     | • 2: Exit team                  ┃\n";    
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RTLEADER)
    {
        return 
        "┃ • 1 <member>: Add to team        | • 2: Exit team                  ┃\n"   
        "┃ • 3: Accept join request         | • 4 <member>: Kick member       ┃\n"   
        "┃ • 5: Start game                                                    ┃\n";   
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_TLEADER)
    {
        return 
        "┃ • 1 <member>: Add to team        | • 2: Exit team                  ┃\n"   
        "┃ • 3 <id>: Accept join request    | • 4 <member>: Kick member       ┃\n";
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RLEADER)
    {
        return 
        "┃ • 1 <member>: Invite to team     | • 2: Exit team                  ┃\n"   
        "┃ • 3: Start game                                                    ┃\n"; 
    }

    return "";
}

string GetLobbySubLog()
{
    if (JoinRequestAmount != 0)
    {
        string text = to_string(JoinRequestAmount) + " pending joining request(s).";
        return "┃ " + text + string(67 - text.length(), ' ') + "┃\n";
    }
   
    if (TeamInviteRequest == -1)
    {
        TeamInviteRequest = 0;

        string text = FG_RED "Invite request expired." RESET;
        return "┃ " + text + string(76 - text.length(), ' ') + "┃\n";
    }

    if (TeamInviteRequest == -2)
    {
		TeamInviteRequest = 0;

		string text = FG_GREEN "Invite request accepted." RESET;
		return "┃ " + text + string(76 - text.length(), ' ') + "┃\n";
    }

    if (TeamInviteRequest != 0)
    {
        string text = FG_YELLOW "Invite request to team " + to_string(TeamInviteRequest) + "." RESET;
        return "┃ " + text + string(76 - text.length(), ' ') + "┃\n";
    }

    return "";
}

void ShowLobbyView()
{
    ClearScreen();

    cout << MakeLobbyTitle(Account.Name) << "\n";
    cout <<
    "┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
    "┃ " << GetTeamColor(1) << SQUARE << RESET << " 1" << MakeLobbyTeamLine(Lobby.Teams[0], 0) <<
    "┃ " << GetTeamColor(2) << SQUARE << RESET << " 2" << MakeLobbyTeamLine(Lobby.Teams[1], 1) <<
    "┃ " << GetTeamColor(3) << SQUARE << RESET << " 3" << MakeLobbyTeamLine(Lobby.Teams[2], 2) <<
    "┃ " << GetTeamColor(4) << SQUARE << RESET << " 4" << MakeLobbyTeamLine(Lobby.Teams[3], 3) <<
    "┃ " << GetTeamColor(5) << SQUARE << RESET << " 5" << MakeLobbyTeamLine(Lobby.Teams[4], 4) <<
    "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        << GetLobbyOption() <<
    "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
    "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        << GetLobbySubLog() <<
    "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

void ShowLobbyLog(string log)
{
    Log = log;

    ShowLobbyView();
}

void ShowLobbyCode(string code)
{
    Log = LogLobbyCode(code);

    ShowLobbyView();
}

#endif