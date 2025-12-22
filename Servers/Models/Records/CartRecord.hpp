#ifndef SERVER_MODEL_RECORD_CART
#define SERVER_MODEL_RECORD_CART

struct CartRecord
{
    int Type;
    int Equipment;
    int Amount;

    static CartRecord Deserialize(const string& msg)
    {
        json j = json::parse(msg);

        CartRecord r;
        r.Type = j.value("Type", -1);
        r.Equipment = j.value("Equipment", -1);
        r.Amount = j.value("Amount", 0);
        return r;
    }
    string Capture() const
    {
        string type = Type == 0 ? "Weapon" : "Defense";

        return type + ": " + to_string(Equipment) + " x" + to_string(Amount);
    }
};

#endif