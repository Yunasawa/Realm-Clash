#ifndef SERVER_MODEL_RECORD_MAP
#define SERVER_MODEL_RECORD_MAP

/*Hiển thị thông tin lên màn hình clients*/

struct SpotRecord
{
    int WoodSlot = -1;
    int RockSlot = -1;
    int IronSlot = -1;
};

struct CastleRecord
{
    int OwnerTeam = -1;
    int Defense;
};

struct MapRecord
{
    array<SpotRecord, 6> Spots;
    array<CastleRecord, 3> Castles;

    static MapRecord Deserialize(const string& data)
    {
        MapRecord result;

        json j = json::parse(data);

        if (j.contains("Spots") && j["Spots"].is_array())
        {
            size_t count = min(result.Spots.size(), j["Spots"].size());
            for (size_t i = 0; i < count; ++i)
            {
                const auto& js = j["Spots"][i];

                if (js.contains("Wood"))  result.Spots[i].WoodSlot = js["Wood"].get<int>();
                if (js.contains("Rock"))  result.Spots[i].RockSlot = js["Rock"].get<int>();
                if (js.contains("Iron"))  result.Spots[i].IronSlot = js["Iron"].get<int>();
            }
        }

        if (j.contains("Castles") && j["Castles"].is_array())
        {
            size_t count = min(result.Castles.size(), j["Castles"].size());
            for (size_t i = 0; i < count; ++i)
            {
                const auto& jc = j["Castles"][i];

                if (jc.contains("OwnerTeam")) result.Castles[i].OwnerTeam = jc["OwnerTeam"].get<int>();
                if (jc.contains("Defense"))   result.Castles[i].Defense = jc["Defense"].get<int>();
            }
        }

        return result;
    }

};

#endif