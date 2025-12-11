#ifndef CLIENT_HANDLER_PHASE_LOBBY_JOINING
#define CLIENT_HANDLER_PHASE_LOBBY_JOINING

void HandleLobbyInput(int clientFD, vector<string> split)
{
    string message;
    int code = atoi(split[0].c_str());

    cout << code << " " << split[1] << endl;
    if (code == 1)
    {
        SendMessage(clientFD, string(RQ_JOIN_TEAM) + " " + split[1]);
    }
}

void HandleLobbyResponse(int clientFD, const string& code, vector<string> split)
{
    if (code == RS_UPDATE_ROOM_LIST)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        ShowLobbyView(code);
    }
    else if (code == RS_JOIN_TEAM_S)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);

        auto member = Lobby.GetMember(Account.ID);
        
        if (member.ID != 0)
        {
            auto isTeamLeader = member.IsTeamLeader;
            auto isRoomLeader = member.IsRoomLeader;

            if (isRoomLeader && isTeamLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RTLEADER;
            }
            else if (isRoomLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RLEADER;
            }
            else if (isTeamLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_TLEADER;
            }
            else
            {
                CurrentPhase = PHASE_LOBBY_JOINED_MEMBER;
            }
        }

        ShowLobbyView(code);
    }
}


#endif