#ifndef VIEW_LOBBY
#define VIEW_LOBBY

#include "../../Cores/CoreFunction.hpp" 
#include "../../Cores/CoreIncluding.hpp" 
#include "../../Cores/CoreDefinition.hpp"

/*
┏━ playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━ LOBBY ━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃
┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃
┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃
┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃
┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃ ● 1 | RoomA          10 / 15 ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                         <  1/2  >                                          ┃
┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1 : Go to previous page                         2 : Go to next page                        ┃
┃ 3 <room name> : Create new room                 4 <room id> : Join a room                  ┃
┃ 5 <new name> : Change player's name                                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

struct UIRoom 
{
    int id;        
    string name;
    int current;
    int max;
    bool inMatch;
};

string GetDotColor(const UIRoom& r) 
{
    if (r.inMatch)
        return "\033[31m●\033[0m"; // Red = match started
    if (r.current == r.max)
        return "\033[33m●\033[0m"; // Yellow = full
    return "\033[32m●\033[0m";     // Green = available
}

void DrawUI(const vector<UIRoom>& rooms, const string& playerName, int page, int maxPage) 
{
    ClearScreen();

    int columns = 3;
    int rowsPerPage = 5;
    int start = page * rowsPerPage * columns;

    string bar = "━";
    string title = bar + " " + playerName + " " + bar;
    const int totalWidth = 96; // wide enough for 3 columns
    int remaining = totalWidth - (int)title.size();
    if (remaining < 0) remaining = 0;

    cout << "┏" << title;
    for (int i=0; i<remaining; i++) cout << bar;
    cout << "┓\n";

    cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━ ROOM LIST ━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";

    for (int row=0; row<rowsPerPage; row++)
    {
        for (int col=0; col<columns; col++) 
        {
            int index = start + row*columns + col;
            if (index < (int)rooms.size()) 
            {
                const UIRoom& r = rooms[index];
                string dot = GetDotColor(r);
                string label = to_string(r.id) + " | " + r.name;

                cout << "┃ " << dot << " "
                     << left << setw(19) << setfill(' ') << label
                     << right << setw(2) << r.current
                     << " / "
                     << setw(2) << r.max << " ";
            } 
            else 
            {
                cout << "┃ " << string(26,' ') << "   ";
            }
        }
        cout << "┃\n";
    }

    cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
    cout << "┃ Page: " << (page+1) << " / " << maxPage << setw(totalWidth-16) << " " << "┃\n";
    cout << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
    cout << "┃ 1 : Go to previous page                         2 : Go to next page                        ┃\n";
    cout << "┃ 3 <room name> : Create new room                 4 <room id> : Join a room                  ┃\n";
    cout << "┃ 5 <new name> : Change player's name                                                        ┃\n";
    cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

#endif