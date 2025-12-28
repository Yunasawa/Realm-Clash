// Client-side question entity used by views and handlers
#ifndef CLIENT_MODEL_ENTITY_QUESTION
#define CLIENT_MODEL_ENTITY_QUESTION

#include "../../../Commons/CommonIncluding.hpp"

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

#endif
