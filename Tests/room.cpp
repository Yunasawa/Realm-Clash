#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "../Servers/ServerDeclaration.h"

using namespace std;

// ------------------------------------------------------------------------------------
// OLD UI STRUCT (used ONLY for drawing UI)
// ------------------------------------------------------------------------------------
struct UIRoom {
    int id;        // NEW: Show room ID
    string name;
    int current;
    int max;
    bool inMatch;
};

// ------------------------------------------------------------------------------------
// TERMINAL UI HELPERS
// ------------------------------------------------------------------------------------
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string GetDotColor(const UIRoom& r) {
    if (r.inMatch)
        return "\033[31m●\033[0m"; // Red = match started
    if (r.current == r.max)
        return "\033[33m●\033[0m"; // Yellow = full
    return "\033[32m●\033[0m";     // Green = available
}

// ------------------------------------------------------------------------------------
// UI DRAWER
// ------------------------------------------------------------------------------------
void DrawUI(const vector<UIRoom>& rooms, const string& playerName, int page, int maxPage) {
    ClearScreen();

    int start = page * 5;
    int end = min(start + 5, (int)rooms.size());

    string bar = "━";
    string title = bar + " " + playerName + " " + bar;

    const int totalWidth = 44;
    int remaining = totalWidth - (int)title.size();
    if (remaining < 0) remaining = 0;

    cout << "┏" << title;
    for (int i = 0; i < remaining; i++) cout << bar;
    cout << "┓\n";

    cout << "┣━━━━━━━━━━━━━━━ ROOM LIST ━━━━━━━━━━━━━━┫\n";

    if (rooms.empty()) {
        cout << "┃ No rooms available                     ┃\n";
    } else {
        for (int i = start; i < end; i++) {
            const UIRoom& r = rooms[i];
            string dot = GetDotColor(r);

            // FORMAT:
            // ● <ID> <RoomName> <current/max>
            string idText = to_string(r.id) + " | ";
            string label = idText + r.name;

            cout << "┃ " << dot << " "
                << left << setw(29) << setfill(' ') << label
                << right << setw(2) << r.current
                << " / "
                << setw(2) << r.max
                << " ┃\n";
        }

        for (int i = end; i < start + 5; i++)
            cout << "┃                                        ┃\n";
    }

    cout << "┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫\n";
    cout << "┃ Page: " << (page + 1) << " / " << maxPage
         << setw(30 - to_string(page + 1).size() - to_string(maxPage).size())
         << " " << "┃\n";

    cout << "┃                                        ┃\n";
    cout << "┃ Q / q : Previous Page                  ┃\n";
    cout << "┃ E / e : Next Page                      ┃\n";
    cout << "┃ 3 <room name> : Create Room            ┃\n";
    cout << "┃ 4 <room id> : Join Room                ┃\n";
    cout << "┃ 5 <new name> : Rename                  ┃\n";
    cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

// ------------------------------------------------------------------------------------
// CONVERT NEW LOBBY ROOMS → UI ROOMS (WITH SORTING!)
// ------------------------------------------------------------------------------------
vector<UIRoom> ConvertLobbyRooms(const Lobby& lobby) {
    vector<UIRoom> ui;
    ui.reserve(lobby.Rooms.size());

    // Extract and sort by room ID so they appear A→B→C
    vector<pair<int, Room>> sortedRooms(lobby.Rooms.begin(), lobby.Rooms.end());
    sort(sortedRooms.begin(), sortedRooms.end(),
         [](auto& a, auto& b) { return a.first < b.first; });

    for (const auto& pair : sortedRooms) {
        const Room& r = pair.second;

        UIRoom u;
        u.id = r.ID;
        u.name = r.Name;
        u.current = r.Members.size();
        u.max = 15;
        u.inMatch = r.InMatch;

        ui.push_back(u);
    }

    return ui;
}

// ------------------------------------------------------------------------------------
// MAIN
// ------------------------------------------------------------------------------------
int main() {
    Lobby lobby;
    string playerName = "playerA";
    int myFD = 999;

    // Sample rooms
    lobby.Rooms[1] = {1, "RoomA", 100, {1,2,3,4,5,6,7,8,9,10}, false};
    lobby.Rooms[2] = {2, "RoomB", 200, {1,2,3,4,5,6}, true};
    lobby.Rooms[3] = {3, "RoomC", 300, {1}, false};
    lobby.Rooms[4] = {4, "RoomD", 400, vector<int>(15), true};
    lobby.Rooms[5] = {5, "RoomE", 500, {1,2,3,4,5,6,7,8}, false};
    lobby.Rooms[6] = {6, "RoomF", 600, vector<int>(15), false};
    lobby.Rooms[7] = {7, "RoomG", 700, {1,2}, false};

    int page = 0;

    while (true) {
        vector<UIRoom> rooms = ConvertLobbyRooms(lobby);

        int totalPages = (rooms.size() + 4) / 5;

        DrawUI(rooms, playerName, page, totalPages);

        cout << "\nEnter option: ";
        string input;
        getline(cin, input);

        if (input == "Q" || input == "q") {
            if (page > 0) page--;
        }
        else if (input == "E" || input == "e") {
            if (page < totalPages - 1) page++;
        }
        else if (input.rfind("3 ", 0) == 0) {
            string roomName = input.substr(2);
            Room newRoom;
            newRoom.ID = lobby.NextRoomID++;
            newRoom.Name = roomName;
            newRoom.HostFD = myFD;
            newRoom.Members.push_back(myFD);
            lobby.Rooms[newRoom.ID] = newRoom;
        }
        else if (input.rfind("4 ", 0) == 0) {
            try {
                int roomID = stoi(input.substr(2));
                if (lobby.Rooms.count(roomID)) {
                    lobby.Rooms[roomID].Members.push_back(myFD);
                }
            } catch (...) {}
        }
        else if (input.rfind("5 ", 0) == 0) {
            playerName = input.substr(2);
        }
    }

    return 0;
}
/*
┏━ playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┣━━━━━━━━━━━━━━━ ROOM LIST ━━━━━━━━━━━━━━┫
┃ ● 1 ┃ RoomA                    10 / 15 ┃
┃ ● 2 ┃ RoomB                     6 / 15 ┃
┃ ● 3 ┃ RoomC                     1 / 15 ┃
┃ ● 4 ┃ RoomD                    15 / 15 ┃
┃ ● 5 ┃ RoomE                     8 / 15 ┃
┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫
┃ Page: 1 / 2                            ┃
┃                                        ┃
┃ Q / q : Previous Page                  ┃
┃ E / e : Next Page                      ┃
┃ 3 <room name> : Create Room            ┃
┃ 4 <room id> : Join Room                ┃
┃ 5 <new name> : Rename                  ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/