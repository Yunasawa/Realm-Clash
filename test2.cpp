#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct Room {
    string name;
    int current;
    int max;
    bool inMatch;
};

string GetDotColor(const Room& r) {
    if (r.inMatch)
        return "\033[31m●\033[0m"; // Red = match started
    if (r.current == r.max)
        return "\033[33m●\033[0m"; // Yellow = full
    return "\033[32m●\033[0m";     // Green = available
}

void DrawUI(const vector<Room>& rooms, const string& playerName, int page, int maxPage) {
    ClearScreen();

    int start = page * 5;
    int end = min(start + 5, (int)rooms.size());

    cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    cout << "┃ Name: " << left << setw(32) << playerName << " ┃\n";
    cout << "┣━━━━━━━━━━━━━━━ ROOM LIST ━━━━━━━━━━━━━━┫\n";

    if (rooms.empty()) {
        cout << "┃ No rooms available                     ┃\n";
    } else {
        for (int i = start; i < end; i++) {
            const Room& r = rooms[i];
            string dot = GetDotColor(r);

            cout << "┃ " << dot << " "
                << left << setw(29) << setfill(' ') << r.name
                << right << setw(2) << r.current
                << " / "
                << setw(2) << r.max
                << " ┃\n";
        }

        // Fill empty slots if less than 5 rooms on page
        for (int i = end; i < start + 5; i++)
            cout << "┃                                        ┃\n";
    }

    cout << "┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫\n";
    cout << "┃ Q = Previous Page                      ┃\n";
    cout << "┃ E = Next Page                          ┃\n";
    cout << "┃                                        ┃\n";

    // Page info
    cout << "┃ Page: " << (page + 1) << " / " << maxPage << setw(30 - to_string(page + 1).size() - to_string(maxPage).size()) << " " << "┃\n";
    cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

int main() {
    string playerName = "playerA";

    vector<Room> rooms = {
        {"RoomA", 10, 15, false},
        {"RoomB", 6, 15, false},
        {"RoomC", 1, 15, false},
        {"RoomD", 15, 15, false},
        {"RoomE", 8, 15, true},
        {"RoomF", 15, 15, true},
        {"RoomG", 2, 15, false}
    };

    int totalPages = (rooms.size() + 4) / 5; // each page = 5 rooms
    int page = 0;

    while (true) {
        DrawUI(rooms, playerName, page, totalPages);

        cout << "\nEnter choice (Q/E): ";
        string input;
        getline(cin, input);

        if (input == "Q" || input == "q") {
            if (page > 0) page--;
        } else if (input == "E" || input == "e") {
            if (page < totalPages - 1) page++;
        }
    }

    return 0;
}
