#ifndef SERVER_MODEL_ENTITY_GAME
#define SERVER_MODEL_ENTITY_GAME

enum class ResourceType { Wood, Stone, Iron, Gold };

struct TeamEntity
{
	vector<int> Members;
    array<array<int, 2>, 3> SpotSlots = { { {-1, -1}, {-1, -1}, {-1, -1} } };
    vector<int> CastleSlots;

    array<int, 4> Resources;

    int GetFreeSlot(int type)
    {
        auto& spot = SpotSlots[type];

        if (spot[0] == -1) return 0;
        else if (spot[1] == -1) return 1;

        return -1;
    }
};

struct GroupEntity
{
    vector<TeamEntity> Teams;

    void CreateTeam()
    {
        for (auto& team : Lobby.Teams)
        {
            if (team.CountMember() == 0) continue;

            int teamIndex = (int)Teams.size();
            Teams.emplace_back();
            auto& gameTeam = Teams.back();

            for (auto& member : team.Members)
            {
                if (member.ID == 0) continue;

                Accounts[member.ID].GameTeam = teamIndex;
                gameTeam.Members.push_back(member.ID);

            }
        }
    }
    void UpdateResource()
    {
        for (auto& team : Teams)
        {
            for (int t = 0; t < 3; t++)
            {
                for (int s = 0; s < 2; s++)
                {
                    if (team.SpotSlots[t][s] == -1) continue;

                    team.Resources[t] += ResourcePerTick[t];
                }
            }
        }
    }

    string SerializeResource() const
    {
        json j = json::array();

        for (const auto& team : Teams)
        {
            j.push_back(team.Resources);
        }

        return j.dump();
    }
};

struct SpotEntity
{
    array<int, 3> Slots = { -1, -1, -1 };
};

struct CastleEntity
{
	int OwnerTeam = -1;
	int Defense;
};

struct MapEntity
{
	array<SpotEntity, 6> Spots;
	array<CastleEntity, 3> Castles;

	string Serialize() const
	{
        json j;

        j["Spots"] = json::array();
        for (const auto& spot : Spots)
        {
            j["Spots"].push_back({
                { "Wood",  spot.Slots[0] },
                { "Rock",  spot.Slots[1] },
                { "Iron",  spot.Slots[2] }
                });
        }

        j["Castles"] = json::array();
        for (const auto& castle : Castles)
        {
            j["Castles"].push_back({
                { "OwnerTeam", castle.OwnerTeam },
                { "Defense",   castle.Defense   }
                });
        }

        return j.dump();
	}
};

#endif