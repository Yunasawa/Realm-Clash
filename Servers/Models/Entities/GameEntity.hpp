#ifndef SERVER_MODEL_ENTITY_GAME
#define SERVER_MODEL_ENTITY_GAME
using namespace std;
#include <chrono> 
using Clock = chrono::steady_clock;

enum class Difficulty{
    EASY,
    DIFFICULT
};

struct QuestionEntity{
    string content; 
    vector<string> answers;
    int correctAnswer;
    Difficulty difficulty;

    void display() const {
        cout << "Question: " << content << endl;
        cout << "A. " << answers[0] << endl;
        cout << "B. " << answers[1] << endl;
        cout << "C. " << answers[2] << endl;
        cout << "D. " << answers[3] << endl;
        cout << "Correct answer: " << (char)('A' + correctAnswer) << endl;
        cout << "Difficulty: " << (difficulty == Difficulty::EASY ? "Easy" : "Difficult") << endl;
    }
};

struct QuestionBankEntity{
    vector<QuestionEntity> spot_questions;
    vector<QuestionEntity> castle_questions;
    
    bool load_data(const string& filename){
        ifstream file(filename);
        if(!file.is_open()){
            return false;
        }
        // Peek first non-space char to guess format
        char c;
        while (file.get(c)) {
            if (!isspace((unsigned char)c)) break;
        }
        file.clear();
        file.seekg(0);
        string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            // skip comments
            if (line.size() > 0 && (line[0] == '#' || line[0] == '/')) continue;

            // split by '|'
            vector<string> parts;
            size_t start = 0;
            while (true) {
                size_t pos = line.find('|', start);
                if (pos == string::npos) {
                    parts.push_back(line.substr(start));
                    break;
                }
                parts.push_back(line.substr(start, pos - start));
                start = pos + 1;
            }

            if (parts.size() < 7) continue; // malformed

            QuestionEntity q;
            q.content = parts[1];
            q.answers.clear();
            q.answers.push_back(parts[2]);
            q.answers.push_back(parts[3]);
            q.answers.push_back(parts[4]);
            q.answers.push_back(parts[5]);
            try { q.correctAnswer = stoi(parts[6]); } catch(...) { q.correctAnswer = -1; }
            
            if (parts[0][0] - '0' == 1){
                q.difficulty = Difficulty::EASY;
                spot_questions.push_back(q);
            } 
            else if (parts[0][0] - '0' == 2){
                q.difficulty = Difficulty::DIFFICULT;
                castle_questions.push_back(q);
            }
        }

        file.close();
        return true;
    }
};

extern QuestionBankEntity QuestionBank;

enum class ResourceType { 
    Wood, 
    Stone, 
    Iron, 
    Gold 
};

struct TeamEntity
{
	vector<int> Members;
    array<array<int, 2>, 3> SpotSlots = { { {-1, -1}, {-1, -1}, {-1, -1} } };
    vector<int> CastleSlot;

    array<int, 4> Resources;
    array<int, 3> Weapons; /*Inventory*/

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
            for (int type = 0; type < 3; type++)
            {
                for (int slot = 0; slot < 2; slot++)
                {
                    if (team.SpotSlots[type][slot] == -1) continue;

                    team.Resources[type] += ResourcePerTick[type] * MULTIPLIER_RESOURSE_UPDATE;
                }
            }

            if (team.CastleSlot.size() >= 1)
            {
                team.Resources[3] += ResourcePerTick[3] * MULTIPLIER_RESOURSE_UPDATE * team.CastleSlot.size();
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
    pair<int,QuestionEntity> CurrentQuestion;
    unordered_map<int,Clock::time_point> LastWrongAnswer;
};

enum class Items{
    BALLISTA, 
    CATAPULT,
    CANNON,
    FENCE,
    WOOD_WALL,
    STONE_WALL,
    LEGEND_WALL,
};

struct Item
{
    Items ItemType;
    std::unordered_map<ResourceType,int> Cost;
    int AttackPoint = 0;
    int DefensePoint = 0;
};

Item* GetItem(Items ItemType)
{
    Item* item = new Item;
    item->ItemType = ItemType;
    switch(ItemType){
        case Items::BALLISTA: 
            item->AttackPoint = 1000; 
            item->Cost = {
                {ResourceType::Wood,1500},
                {ResourceType::Stone,0},
                {ResourceType::Iron,200}
            };
            break;
        case Items::CATAPULT: 
            item->AttackPoint = 3000;
            item->Cost = {
                {ResourceType::Wood,400},
                {ResourceType::Stone,1500},
                {ResourceType::Iron,300}
            };
            break;
        case Items::CANNON: 
            item->AttackPoint = 8000;
            item->Cost = {
                {ResourceType::Wood,500},
                {ResourceType::Stone,2500},
                {ResourceType::Iron,1800}
            };
            break;
        case Items::FENCE: 
            item->DefensePoint = 200; 
            item->Cost = {
                {ResourceType::Wood,200},
                {ResourceType::Stone,50},
                {ResourceType::Iron,0}
            };
            break;
        case Items::WOOD_WALL: 
            item->DefensePoint = 1000;
            item->Cost = {
                {ResourceType::Wood,1000},
                {ResourceType::Stone,100},
                {ResourceType::Iron,100}
            }; 
            break;
        case Items::STONE_WALL: 
            item->DefensePoint = 3000; 
            item->Cost = {
                {ResourceType::Wood,200},
                {ResourceType::Stone,1000},
                {ResourceType::Iron,200}
            };
            break;
        case Items::LEGEND_WALL: 
            item->DefensePoint = 8000; 
            item->Cost = {
                {ResourceType::Wood,1000},
                {ResourceType::Stone,2000},
                {ResourceType::Iron,1000}
            };
            break;
    }
    return item;
}

struct CastleEntity
{
	int OwnerTeam = -1;
	int Defense;
    pair<int,QuestionEntity> CurrentQuestion;
    unordered_map<int,Clock::time_point> LastWrongAnswer;
    vector<int> EquippedItems;
    unordered_map<int,int> IsSuccessAnswerQuestion; /*teamID : key, boolean : value*/ 
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