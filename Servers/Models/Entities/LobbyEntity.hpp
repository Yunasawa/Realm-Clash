#ifndef SERVER_MODEL_ENTITY_LOBBY
#define SERVER_MODEL_ENTITY_LOBBY

struct TeamEntity
{
    array<int, 3> Members;

    int AssignFreeSlot(int account)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Members[i] == 0)
            {
                Members[i] = account;
                return i;
            }
        }

        return -1;
    }
};

struct LobbyEntity
{
    array<TeamEntity, 5> Teams;

    string Serialize() const
    {
        json j;
        j["Teams"] = json::array();

        for (const auto& team : Teams)
        {
            json teamJson;
            teamJson["Members"] = json::array();

            for (const int& member : team.Members)
            {
                if (member == 0)
                {
                    teamJson["Members"].push_back(
                    {
                        {"ID", 0},
                        {"Name", ""},
                        {"IsTeamLeader", false},
                        {"IsRoomLeader", false}
                    });
                }
                else
                {
                    auto account = Accounts[member];
                    teamJson["Members"].push_back(
                    {
                        {"ID", member},
                        {"Name", account.Name},
                        {"IsTeamLeader", account.IsTeamLeader},
                        {"IsRoomLeader", account.IsRoomLeader}
                    });
                }
            }

            j["Teams"].push_back(teamJson);
        }

        return j.dump();
    }
};

#endif