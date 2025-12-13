#ifndef SERVER_HANDLER_LOBBY
#define SERVER_HANDLER_LOBBY

LobbyEntity Lobby;

void HandleUpdateLobby(int clientFD)
{
    SendMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize());
}

void HandleJoinTeam(int clientFD, string data)
{
    int team = atoi(data.c_str()) - 1;

    auto& account = Accounts[Clients[clientFD]];
    auto assignedSlot = Lobby.Teams[team].AssignFreeSlot(account.ID);

    if (assignedSlot != -1)
    {
        if (RoomLeader == 0)
        {
            RoomLeader = account.ID;
            account.IsRoomLeader = true;
        }

        if (assignedSlot == 0)
        {
            account.IsTeamLeader = true;
        }

        account.Team = team;
    }

    SendMessage(clientFD, string(RS_JOIN_TEAM_S) + " " + Lobby.Serialize());
    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
}

AccountEntity* FindAccountByName(const std::string& myName)
{
    for (auto& [id, acc] : Accounts)
    {
        if (acc.Name == myName) return &acc;
    }

    return nullptr;
}

void HandleAddMember(int clientFD, string name)
{
    if (auto* account = FindAccountByName(name))
    {
        auto clientAccount = Accounts[Clients[clientFD]];
        auto freeSlotAmount = Lobby.Teams[clientAccount.Team].CountFreeSlot();

        if (freeSlotAmount == 0)
        {
            SendMessage(clientFD, string(RS_ADD_MEMBER_F_TEAM_FULL));
        }
        else
        {
            auto assignedSlot = Lobby.Teams[clientAccount.Team].AssignFreeSlot(account->ID);

            SendMessage(clientFD, string(RS_ADD_MEMBER_S) + " " + Lobby.Serialize());
            BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
            SendMessage(GetValueByKey(Clients, account->ID), string(RS_UPDATE_TEAM_ROLE));
        }
    }
    else
    {
        SendMessage(clientFD, string(RS_ADD_MEMBER_F_NOT_FOUND) + " " + name);
    }
}

void HandleExitTeam(int clientFD)
{
    auto account = Accounts[Clients[clientFD]];

    if (account.IsRoomLeader)
    {

    }
    if (account.IsTeamLeader)
    {
        
    }

    account.Team = 0;

    Lobby.RemoveMember(account.Team, account.ID);

    SendMessage(clientFD, string(RS_EXIT_TEAM_S) + " " + Lobby.Serialize());
    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
}

#endif