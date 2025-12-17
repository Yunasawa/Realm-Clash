#ifndef SERVER_MODEL_ENTITY_ACCOUNT
#define SERVER_MODEL_ENTITY_ACCOUNT

struct SessionEntity
{
    atomic<bool> Tick = { false };
    chrono::steady_clock::time_point StartTime;

    void Initialize()
    {
        Tick.store(false);
		StartTime = chrono::steady_clock::now();
    }
};

struct AccountEntity
{
    int ID;
    string Name;

    int Team = -1;
    int PendingTeam;
    int PendingInvitation = -1;
    int InvitorID = 0;
    bool IsTeamLeader;
    bool IsRoomLeader;

    string Serialize() const
    {
        json j;
        j["ID"] = ID;
        j["Name"] = Name;

        return j.dump();
    }
    string Capture() const
    {
		return "ID: " + to_string(ID) + ", Name: " + Name;
    }
};

#endif