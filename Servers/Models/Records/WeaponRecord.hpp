#ifndef SERVER_MODEL_RECORD_WEAPON
#define SERVER_MODEL_RECORD_WEAPON

struct WeaponRecord
{
    int Castle;
    int Weapon;
    int Amount;

    static WeaponRecord Deserialize(const string& msg)
    {
        json j = json::parse(msg);

        WeaponRecord w;
        w.Castle = j.value("Castle", -1);
        w.Weapon = j.value("Weapon", -1);
        w.Amount = j.value("Amount", -1);
        return w;
    }
};

#endif