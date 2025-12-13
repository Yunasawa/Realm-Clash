#ifndef CLIENT_HANDLER_PHASE_LOBBY_JOINING_READY
#define CLIENT_HANDLER_PHASE_LOBBY_JOINING_READY

void HandleLobbyInput(int clientFD, vector<string> split)
{
    string message;
    int code = atoi(split[0].c_str());

    if (CurrentPhase == PHASE_LOBBY_JOINING_READY)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_JOIN_TEAM) + " " + split[1]);
        }
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINING_PENDING)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_CANCEL_JOINING));
        }
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_MEMBER)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_INVITE_MEMBER) + " " + split[1]);
        }
        else if (code == 2)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RTLEADER)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_ADD_MEMBER) + " " + split[1]);
        }
        else if (code == 2)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        } 
        else if (code == 3)
        {
            SendMessage(clientFD, string(RQ_ACCEPT_PARTICIPATION));
        } 
        else if (code == 4)
        {
            SendMessage(clientFD, string(RQ_KICK_MEMBER) + " " + split[1]);
        } 
        else if (code == 5)
        {
            SendMessage(clientFD, string(RQ_START_GAME));
        }        
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_TLEADER)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_ADD_MEMBER) + " " + split[1]);
        }
        else if (code == 2)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }  
        else if (code == 3)
        {
            SendMessage(clientFD, string(RQ_ACCEPT_PARTICIPATION) + " " + split[1]);
        } 
        else if (code == 4)
        {
            SendMessage(clientFD, string(RQ_KICK_MEMBER) + " " + split[1]);
        }       
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RLEADER)
    {
        if (code == 1)
        {
            SendMessage(clientFD, string(RQ_INVITE_MEMBER) + " " + split[1]);
        }
        else if (code == 2)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }  
        else if (code == 3)
        {
            SendMessage(clientFD, string(RQ_START_GAME));
        }       
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
    else if (code == RS_ADD_MEMBER_S)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        ShowLobbyView(code);
    }
    else if (code == RS_UPDATE_TEAM_ROLE)
    {
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
        else
        {
            CurrentPhase = PHASE_LOBBY_JOINING_READY;
        }

        ShowLobbyView(code);
    }
    else if (code == RS_EXIT_TEAM_S)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        CurrentPhase = PHASE_LOBBY_JOINING_READY;
        
        ShowLobbyView(code);
    }
    else if (code == RS_UPDATE_JOIN_REQUEST)
    {
        JoinRequestAmount = stoi(split[1]);

        ShowLobbyView(code);
    }
    else if (code == RS_JOIN_TEAM_F_TEAM_FULL)
    {
		ShowLobbyView(code);
    }
    else if (code == RS_UPDATE_PENDING_JOIN)
    {
		cout << "Pending joining: " << split[1] << endl;
    }
}


#endif