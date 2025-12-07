#ifndef CORE_FUNCTION
#define CORE_FUNCTION

#include "CoreIncluding.hpp"

void ClearScreen() 
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
struct MessageData
{
    string text;
    string time;
};

void to_json(json& j, const MessageData& m)
{
    j = json{
        {"text", m.text},
        {"time", m.time}
    };
}

void from_json(const json& j, MessageData& m)
{
    m.text = j.at("text").get<string>();
    m.time = j.at("time").get<string>();
}

tuple<int, string> HandleResponse(const string& input) 
{
    size_t spacePos = input.find(' ');

    if (spacePos == string::npos) 
    {
        throw invalid_argument("Input must contain a space separating number and text.");
    }

    int number = stoi(input.substr(0, spacePos));
    string text = input.substr(spacePos + 1);
    return {number, text};
}

tuple<string, string> HandleRequest(const string& input) 
{
    size_t spacePos = input.find(' ');

    if (spacePos == string::npos) 
    {
        return {input, ""};
    }

    string part1 = input.substr(0, spacePos);
    string part2 = input.substr(spacePos + 1);
    return {part1, part2};
}

vector<string> SplitMessage(const string& str) 
{
    vector<string> result;
    istringstream iss(str);
    string token;
    while (getline(iss, token, '/')) 
    {
        result.push_back(token);
    }

    return result;
}

bool IsExistedAccount(const string& path, const string& email, string& password) 
{
    ifstream fin(path);
    
    if (!fin.is_open()) 
    {
        cerr << "Cannot open account file\n";
        return false;
    }

    string id, fileEmail;
    while (fin >> id >> fileEmail >> password) 
    {
        if (fileEmail == email) 
        {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

void WriteAt(const string& path, int lineNumber, const string& text) 
{
    ifstream fin(path);

    vector<string> lines;
    string temp;

    while (getline(fin, temp)) 
    {
        lines.push_back(temp);
    }
    fin.close();

    while ((int)lines.size() < lineNumber) 
    {
        lines.push_back("");
    }

    lines[lineNumber - 1] = text;

    ofstream fout(path);
    for (size_t i = 0; i < lines.size(); i++) 
    {
        fout << lines[i];
        if (i != lines.size() - 1) fout << "\n";
    }
    fout.close();
}

bool FileExists(const string& path) 
{
    ifstream fin(path);
    return fin.is_open();
}

string ReadLine(const string& path, size_t lineNumber) 
{
    ifstream file(path);
    if (!file.is_open()) 
    {
        throw runtime_error("Cannot open file: " + path);
    }

    string line;
    size_t currentLine = 1;
    while (getline(file, line)) 
    {
        if (currentLine == lineNumber) 
        {
            return line;
        }
        ++currentLine;
    }

    throw out_of_range("File has fewer than " + to_string(lineNumber) + " lines");
}

#endif