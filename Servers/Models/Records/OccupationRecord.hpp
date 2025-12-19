#ifndef SERVER_MODEL_RECORD_OCCUPATION
#define SERVER_MODEL_RECORD_OCCUPATION

struct SpotOccupationRecord
{
    int Spot;
    int Type;

    static SpotOccupationRecord Deserialize(const string& msg)
    {
        json j = json::parse(msg);

        SpotOccupationRecord r;
        r.Spot = j.value("Spot", -1);
        r.Type = j.value("Type", -1);
        return r;
    }
    string Capture() const
    {
        return "Spot: " + to_string(Spot) + ", Resource: " + to_string(Type);
    }
};

#endif