#ifndef QUESTION
#define QUESTION 
#include "../../Commons/CommonIncluding.hpp"
#include <chrono>
using Clock = chrono::steady_clock;
using namespace std;

#define TIME_OUT 60
#define TIME_PENALTY 5
enum Difficulty{
    EASY,
    DIFFICULT
};

struct Question{
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
        cout << "Difficulty: " << (difficulty == EASY ? "Easy" : "Difficult") << endl;
    }
};

struct QuestionBank{
    vector<Question> questions;

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

        if (c == '{' || c == '[') {
            // JSON format (existing behavior)
            json j;
            try {
                file >> j;
            } catch (...) {
                file.close();
                return false;
            }
            file.close();

            if (!j.contains("questions") || !j["questions"].is_array()) return false;

            for (const auto& item : j["questions"]) {
                Question q;
                q.content = item.value("content", string(""));
                q.answers = item.value("answers", vector<string>{});
                q.correctAnswer = item.value("correctAnswer", 0);
                string diff = item.value("difficulty", string("Easy"));
                q.difficulty = (diff == "Easy") ? EASY : DIFFICULT;
                questions.push_back(q);
            }

            return true;
        } else {
            // Assume pipe-separated .ynl format: id|content|a|b|c|d|correct
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

                Question q;
                // parts[0] is id; use to determine difficulty if desired
                int id = 0;
                try { id = stoi(parts[0]); } catch(...) { id = 0; }
                q.content = parts[1];
                q.answers.clear();
                q.answers.push_back(parts[2]);
                q.answers.push_back(parts[3]);
                q.answers.push_back(parts[4]);
                q.answers.push_back(parts[5]);
                try { q.correctAnswer = stoi(parts[6]); } catch(...) { q.correctAnswer = 0; }
                // simple rule: id < 200 -> EASY, else DIFFICULT
                q.difficulty = (id < 200) ? EASY : DIFFICULT;

                questions.push_back(q);
            }

            file.close();
            return true;
        }
    }

    void display_all() const {
        for (size_t i = 0; i < questions.size(); i++) {
            cout << "\n===== Question " << (i + 1) << " =====" << endl;
            questions[i].display();
        }
    }
};

struct Spot
{
    int spotID;
    int ownerTeamID; 
    Resources type;
};

struct Castle
{
    int castleID; 
    int ownerTeamID = -1; 
    int defensePoints;
    pair<int,Question> currentQuestion;
    unordered_map<int,Clock::time_point> lastWrongAnswer;
    std::vector<int> equippedItems;
};

struct Building
{
    std::unordered_map<int, Castle> Castles;
    std::unordered_map<int, Spot> Spots;
};

enum Items{
    BALLISTA, 
    CATAPULT,
    CANNON,
    FENCE,
    WOOD_WALL,
    STONE_WALL,
    LEGEND_WALL,
};

enum Resources
{
    Wood,
    Stone,
    Iron,
    Gold
};

struct Item
{
    Items ItemType;
    std::unordered_map<Resources,int> Cost;
    int AttackPoint = 0;
    int DefensePoint = 0;
};

QuestionBank questionBank; 


#endif