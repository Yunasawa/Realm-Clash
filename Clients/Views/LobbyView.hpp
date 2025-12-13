#ifndef VIEW_LOBBY
#define VIEW_LOBBY

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ U0000001 ━┓
┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ ● 1 |                    |                    |                    ┃
┃--------------------------------------------------------------------┃
┃ ● 2 |                    |                    |                    ┃
┃--------------------------------------------------------------------┃
┃ ● 3 |                    |                    |                    ┃
┃--------------------------------------------------------------------┃
┃ ● 4 |                    |                    |                    ┃
┃--------------------------------------------------------------------┃
┃ ● 5 |                    |                    |                    ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • 1 <teamID>: Create/Join team                                     ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

# Joined & Become member

┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • 1: Invite to team              ┃ • 2: Exit team                  ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫

Branch 3: Joined a team & being leader

┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • 1: Invite to team              ┃ • 2: Accept participation       ┃
┃ • 3: Exit team                   ┃                                 ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
*/

string MakeTitle(const std::string& playerName)
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

string MakeUnderlineName(int id, string name)
{
    if (id == Account.ID) return UNDERLINE + name + RESET;
    return name;
}

string MakeTeamLine(const TeamRecord& team)
{
    auto member0 = team.Members[0];
    auto isRoomLeader = team.Members[0].IsRoomLeader;

    auto tlMark = team.Members[0].IsTeamLeader ? string(BOLD) : "";
    auto rlMark = isRoomLeader ? " ⭐" : "";

    auto member0Text = member0.Name + rlMark;
    auto member0StyledText = MakeUnderlineName(team.Members[0].ID, member0.Name) + rlMark;

    return 
    " | " + tlMark + member0StyledText + RESET + string((isRoomLeader ? 20 : 19) - member0Text.length(), ' ') + 
    "| " + MakeUnderlineName(team.Members[1].ID, team.Members[1].Name) + RESET + string(19 - team.Members[1].Name.length(), ' ') +
    "| " + MakeUnderlineName(team.Members[2].ID, team.Members[2].Name) + RESET + string(19 - team.Members[2].Name.length(), ' ') + "┃\n";
}

string GetLog(string code)
{
    if (code == RS_JOIN_TEAM_S)
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

    return Log;
}

string GetOption()
{
    if (CurrentPhase == PHASE_LOBBY_JOINING_READY)
    {
        return 
        "┃ • 1 <teamID>: Create/Join team                                     ┃\n";
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
        "┃ • 3 <id>: Accept join request    | • 4 <member>: Kick member       ┃\n"   
        "┃ • 5: Remove team                 | • 6: Start game                 ┃\n";   
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_TLEADER)
    {
        return 
        "┃ • 1 <member>: Add to team        | • 2: Exit team                  ┃\n"   
        "┃ • 3 <id>: Accept join request    | • 4 <member>: Kick member       ┃\n"
        "┃ • 5: Remove team                                                   ┃\n";   
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RLEADER)
    {
        return 
        "┃ • 1 <member>: Invite to team     | • 2: Exit team                  ┃\n"   
        "┃ • 3: Start game                                                    ┃\n"; 
    }

    return "";
}

string GetSubLog()
{
    if (JoinRequestAmount == 0)
    {
        return "";
    }
    else
    {
        string text = to_string(JoinRequestAmount) + " pending joining request(s).";
        return "┃ " + text + string(67 - text.length(), ' ') + "┃\n";
    }
}

void ShowLobbyView(string code)
{
    ClearScreen();

    Log = GetLog(code);

    cout << MakeTitle(Account.Name) << "\n";
    cout <<
    "┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
    "┃ " << GetTeamColor(1) << SQUARE << RESET << " 1" << MakeTeamLine(Lobby.Teams[0]) <<
    "┃ " << GetTeamColor(2) << SQUARE << RESET << " 2" << MakeTeamLine(Lobby.Teams[1]) <<
    "┃ " << GetTeamColor(3) << SQUARE << RESET << " 3" << MakeTeamLine(Lobby.Teams[2]) <<
    "┃ " << GetTeamColor(4) << SQUARE << RESET << " 4" << MakeTeamLine(Lobby.Teams[3]) <<
    "┃ " << GetTeamColor(5) << SQUARE << RESET << " 5" << MakeTeamLine(Lobby.Teams[4]) <<
    "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        << GetOption() <<
    "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
    "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        << GetSubLog() <<
    "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

//⭐
#endif