#ifndef IN_GAME_ENTITY
#define IN_GAME_ENTITY



struct CastleEntity
{
    int Id;
    int OwnerTeam = -1;
    int Defense;
};

enum class DifficultyClient { EASY, DIFFICULT };

struct QuestionEntity{
    string content;
    vector<string> answers;
    int correctAnswer;
    DifficultyClient difficulty;

    void display() const {
        cout << "Question: " << content << endl;
        for (size_t i = 0; i < answers.size(); ++i) {
            cout << (i+1) << ": " << answers[i] << endl;
        }
        cout << "Correct answer: " << correctAnswer << endl;
    }
};

struct ResourceEntity{
    int TargetTeamID;
    int Wood;
    int Rock;
    int Iron;
    int Gold;
};

struct InventoryEntity{
    int Balista;
    int Catapult;
    int Canon; 
};

struct WeaponEntity
{
    int Castle;
    int Weapon;
    int Amount;

    string Serialize() const
    {
        json j;
        j["Castle"] = Castle;
        j["Weapon"] = Weapon;
        j["Amount"] = Amount;

        return j.dump();
    }
};

#endif