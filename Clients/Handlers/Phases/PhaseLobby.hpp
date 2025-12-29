#ifndef CLIENT_HANDLER_PHASE_LOBBY
#define CLIENT_HANDLER_PHASE_LOBBY

void HandleLobbyInput(int clientFD, vector<string> command)
{
    string message;
    int code = atoi(command[0].c_str());

    if (CurrentPhase == PHASE_LOBBY_JOINING_READY)
    {
        if (code == 1 && command.size() == 2)
        {
            auto team = stoi(command[1]);

            if (team > 0 && team < 6)
            {
                SendMessage(clientFD, string(RQ_JOIN_TEAM) + " " + command[1]);
            }
            else
            {
                ShowLobbyLog(LOG_LOBBY_WRONG_TEAM);
            }
        } /*Mặc định khi đăng nhập server sẽ chỉ hiện 1 room gồm đúng 5 team từ 1-5 
            để người chơi tham gia, không có tạo room mới, team mới.
            */
		else if (code == 2 && command.size() == 1)
		{
			SendMessage(clientFD, string(RQ_ACCEPT_INVITATION));

            TeamInviteRequest = -2;

            ShowLobbyCode("");
		}
		else goto UnknownCommand;
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINING_PENDING)
    {
        if (code == 1 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_CANCEL_JOINING));
        }
        else goto UnknownCommand;
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_MEMBER)
    {
        if (code == 1 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_INVITE_MEMBER) + " " + command[1]);
        }
        else if (code == 2 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }
		else goto UnknownCommand;
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RTLEADER)
    {
        if (code == 1 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_ADD_MEMBER) + " " + command[1]);
        }
        else if (code == 2 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        } 
        else if (code == 3 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_ACCEPT_PARTICIPATION));
        } 
        else if (code == 4 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_KICK_MEMBER) + " " + command[1]);
        } 
        else if (code == 5 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_START_GAME));
		}
		else goto UnknownCommand;
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_TLEADER)
    {
        if (code == 1 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_ADD_MEMBER) + " " + command[1]);
        }
        else if (code == 2 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }  
        else if (code == 3 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_ACCEPT_PARTICIPATION) + " " + command[1]);
        } 
        else if (code == 4 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_KICK_MEMBER) + " " + command[1]);
        }   
		else goto UnknownCommand;
    }
    else if (CurrentPhase == PHASE_LOBBY_JOINED_RLEADER)
    {
        if (code == 1 && command.size() == 2)
        {
            SendMessage(clientFD, string(RQ_INVITE_MEMBER) + " " + command[1]);
        }
        else if (code == 2 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_EXIT_TEAM));
        }  
        else if (code == 3 && command.size() == 1)
        {
            SendMessage(clientFD, string(RQ_START_GAME));
        }  
		else goto UnknownCommand;
    }

    return;

UnknownCommand:
    ShowLobbyLog(LOG_UNKNOWN_COMMAND);
}

void HandleLobbyResponse(int clientFD, const string& code, vector<string> split)
{
    if (code == RS_RESET_GAME)
    {
        Tick = 0;
        JoinRequestAmount = 0;
        TeamInviteRequest = 0;
        PendingJoinTick = 0;
        PendingInviteTick = 0;
        Log = FG_GREEN "";
        Team = 0;
        Map = MapRecord();
        Resource = ResourceRecord();
        CurrentQuestionSpot = -1;
        CurrentQuestionIsCastle = false;
        CurrentQuestion = QuestionEntity();
        QuestionTimeOut = 30;
        CurrentPhase = PHASE_LOBBY_JOINING_READY;
        ShowLobbyView();
    }
    else if (code == RS_UPDATE_ROOM_LIST)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        ShowLobbyCode(code);

        // Set phase based on current lobby
        auto member = Lobby.GetMember(Account.ID);
        if (member.ID != 0)
        {
            if (member.IsRoomLeader && member.IsTeamLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RTLEADER;
            }
            else if (member.IsRoomLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RLEADER;
            }
            else if (member.IsTeamLeader)
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
        ShowLobbyView();
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
				Log = LOG_LOBBY_BEING_RTLEADER;
            }
            else if (isRoomLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RLEADER;
				Log = LOG_LOBBY_BEING_RLEADER;
            }
            else if (isTeamLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_TLEADER;
				Log = LOG_LOBBY_BEING_TLEADER;
            }
            else
            {
                CurrentPhase = PHASE_LOBBY_JOINED_MEMBER;
            }
        }

        ShowLobbyCode(code);
    }
    else if (code == RS_ADD_MEMBER_S)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        ShowLobbyCode(code);
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
                Log = LOG_LOBBY_BEING_RTLEADER;
            }
            else if (isRoomLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_RLEADER;
				Log = LOG_LOBBY_BEING_RLEADER;
            }
            else if (isTeamLeader)
            {
                CurrentPhase = PHASE_LOBBY_JOINED_TLEADER;
				Log = LOG_LOBBY_BEING_TLEADER;
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

        ShowLobbyCode(code);
    }
    else if (code == RS_EXIT_TEAM_S)
    {
        Lobby = LobbyRecord::Deserialize(split[1]);
        CurrentPhase = PHASE_LOBBY_JOINING_READY;
        
        ShowLobbyCode(code);
    }
    else if (code == RS_UPDATE_JOIN_REQUEST)
    {
        JoinRequestAmount = stoi(split[1]);

        ShowLobbyCode(code);
    }
    else if (code == RS_JOIN_TEAM_F_TEAM_FULL)
    {
		ShowLobbyCode(code);
    }
    else if (code == RS_UPDATE_PENDING_JOIN)
    {
		CurrentPhase = PHASE_LOBBY_JOINING_PENDING;
		PendingJoinTick = DURATION_JOIN_REQUEST - stoi(split[1]);

		ShowLobbyCode(code);
    }
    else if (code == RS_JOIN_TEAM_F_REQUEST_EXPIRED)
    {
		CurrentPhase = PHASE_LOBBY_JOINING_READY;
		
        ShowLobbyCode(code);
    }
    else if (code == RS_JOIN_TEAM_F_REQUEST_FULL)
    {
		CurrentPhase = PHASE_LOBBY_JOINING_READY;

		ShowLobbyLog(LOG_LOBBY_REQUEST_FULL);
    }
    else if (code == RS_JOIN_TEAM_S_REQUEST_ACCEPTED)
    {
		ShowLobbyLog(LOG_LOBBY_REQUEST_ACCEPTED);
    }
    else if (code == RS_ACCEPT_PARTICIPATION_S)
    {
        JoinRequestAmount = 0;

		ShowLobbyCode(code);
    }
    else if (code == RS_INVITE_MEMBER_F_TEAM_FULL)
    {
        ShowLobbyLog(FG_RED "Invite failed: Team is full!");
    }
    else if (code == RS_INVITE_MEMBER_F_REQUEST_FULL)
    {
        ShowLobbyLog(FG_RED "Invite failed: Request list is full!");
    }
    else if (code == RS_INVITE_MEMBER_F_MEMBER_NOT_FOUND)
    {
        ShowLobbyLog(FG_RED "Invite failed: Member not found!");
    }
    else if (code == RS_INVITE_MEMBER_F_JOIN_REQUESTED)
    {
        ShowLobbyLog(FG_RED "Invite failed: Member requested to join!");
    }
	else if (code == RS_UPDATE_PENDING_INVITE)
	{
        PendingJoinTick = DURATION_INVITE_REQUEST - stoi(split[1]);

		ShowLobbyLog(FG_YELLOW "Invite request sent. Expired in " + to_string(PendingJoinTick) + ".");
    }
    else if (code == RS_INVITE_MEMBER_F_REQUEST_EXPIRED)
    {
        ShowLobbyLog(FG_RED "Invite request expired!");
    }
    else if (code == RS_UPDATE_INVITE_REQUEST)
    {
        TeamInviteRequest = stoi(split[1]) + 1;

		ShowLobbyCode(code);
    }
    else if (code == RS_UPDATE_INVITE_EXPIRED)
    {
        TeamInviteRequest = -1;

        ShowLobbyCode(code);
    }
    else if (code == RS_INVITE_MEMBER_S)
    {
        ShowLobbyLog(FG_GREEN "Member accepted invitation!");
    }
}


#endif