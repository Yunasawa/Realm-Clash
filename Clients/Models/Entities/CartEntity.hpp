#ifndef CLIENT_MODEL_ENTITY_CART
#define CLIENT_MODEL_ENTITY_CART

struct CartEntity
{
	int Type;
	int Equipment;
	int Amount;
    int TargetCastle;

    string Serialize() const
    {
        json j;
        j["Type"] = Type;
        j["Equipment"] = Equipment;
        j["Amount"] = Amount;
        j["TargetCastle"] = TargetCastle;
        return j.dump();
    }
};

#endif