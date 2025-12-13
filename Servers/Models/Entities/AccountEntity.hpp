#ifndef SERVER_MODEL_ENTITY_ACCOUNT
#define SERVER_MODEL_ENTITY_ACCOUNT

struct AccountEntity
{
    int ID;
    string Name;

    int Team;
    int PendingTeam;
    bool IsTeamLeader;
    bool IsRoomLeader;

    string Serialize() const
    {
        json j;
        j["ID"] = ID;
        j["Name"] = Name;

        return j.dump();
    }
};

#endif