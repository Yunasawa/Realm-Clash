#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <ctype.h>
#include <fstream>
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

int main(){
    string part = "/home/quang28/Realm-Clash/Datas/Question.ynl";
    QuestionBankEntity bank;
    bank.load_data(part);
    bank.spot_questions[0].display();
}