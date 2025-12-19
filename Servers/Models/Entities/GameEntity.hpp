#ifndef SERVER_MODEL_ENTITY_GAME
#define SERVER_MODEL_ENTITY_GAME

enum class ResourceType { Wood, Stone, Iron, Gold };

struct GameTeamEntity
{
	vector<int> Members;
	pair<int, int> WoodSlot = { -1, -1 };
	pair<int, int> RockSlot = { -1, -1 };
	pair<int, int> IronSlot = { -1, -1 };
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