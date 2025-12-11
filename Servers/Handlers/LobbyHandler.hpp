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

    if (RoomLeader == 0)
    {
        RoomLeader = account.ID;
        account.IsRoomLeader = true;
    }

    if (assignedSlot == 0)
    {
        account.IsTeamLeader = true;
    }

    SendMessage(clientFD, string(RS_JOIN_TEAM_S) + " " + Lobby.Serialize());
    BroadcastMessage(clientFD, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), false);
}

#endif