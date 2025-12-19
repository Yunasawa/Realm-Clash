#ifndef CLIENT_MODEL_ENTITY_OCCUPATION
#define CLIENT_MODEL_ENTITY_OCCUPATION

struct SpotOccupationEntity
{
    int Spot;
    int Type;

    string Serialize() const
    {
        json j;
        j["Spot"] = Spot;
        j["Type"] = Type;

        return j.dump();
    }
};

#endif