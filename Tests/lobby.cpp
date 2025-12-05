#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

struct RoomInfo {
    string roomName;
    int current;
    int max;
    bool isHost;
};

void DrawRoomDetail(const string& playerName, const RoomInfo& info) {
    ClearScreen();

    cout << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    cout << "┃ Name: " << left << setw(32) << playerName << " ┃\n";
    cout << "┣━━━━━━━━━━━━━━━━━ STATUS ━━━━━━━━━━━━━━━┫\n";

    // Room line
    cout << "┃ Room: " 
         << left << setw(9) << info.roomName
         << right << setw(2) << info.current
         << "/"
         << setw(2) << info.max
         << "                   ┃\n";

    // Role line
    cout << "┃ Role: " << (info.isHost ? "Host" : "Member");
    cout << setw(37 - string(info.isHost ? "Host" : "Member").size()) << " ┃\n";

    // Options
    cout << "┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫\n";
    cout << "┃ 1 : Exit room ♖                        ┃\n";

    if (info.isHost)
        cout << "┃ 2 : Start match                        ┃\n";

    cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

int main(int argc, char *argv[]) {
    string playerName = "playerA";

    RoomInfo memberUI = {"RoomA", 6, 15, false};
    RoomInfo hostUI   = {"RoomA", 6, 15, true};

    int type = atoi(argv[1]);
    if (type == 0)
    {
        cout << "\n--- Member UI ---\n\n";
        DrawRoomDetail(playerName, memberUI);
        cin.get();
    }
    else
    {
        cout << "\n--- Host UI ---\n\n";
        DrawRoomDetail(playerName, hostUI);
        cin.get();
    }

    return 0;
}

/*
┏━ playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃
┣━━━━━━━━━━━━━━━━━ STATUS ━━━━━━━━━━━━━━━┫
┃ Room: RoomA     6/15                   ┃
┃ Role: Member                           ┃
┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫
┃ 1 : Exit room                          ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

┏━ playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃
┣━━━━━━━━━━━━━━━━━ STATUS ━━━━━━━━━━━━━━━┫
┃ Room: RoomA     6/15                   ┃
┃ Role: Host                             ┃
┣━━━━━━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━━━━━┫
┃ 1 : Exit room                          ┃
┃ 2 : Start match                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

┏━ ■ ■ ■ ■ ■ ━━━ ■ playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┣━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━┫
┃   W: 100.000 (+30)   ┃   S: 100.000 (+10)   ┃   I: 100.000 (+10)   ┃   G: 100.000 (+1)   ┃
┣━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━┫
┃           ┣╋━╋━╋━╋┫                    ┣╋━╋━╋━╋┫                     ┣╋━╋━╋━╋┫           ┃
┃           ┗╋━┻━┻━╋┛                    ┗╋━┻━┻━╋┛                     ┗╋━┻━┻━╋┛           ┃
┃            ┃  ■  ┃                      ┃  ■  ┃                       ┃  ■  ┃            ┃
┃            ┃  ■  ┃                      ┃  ■  ┃                       ┃  ■  ┃            ┃
┃            ┗━━━━━┛                      ┗━━━━━┛                       ┗━━━━━┛            ┃
┃           CASTLE 01                    CASTLE 02                     CASTLE 03           ┃
┃              ■■■                          ■■■                           ■■■              ┃
┃                                                                                          ┃
┃    ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫    ┃
┃    ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃    ┃
┃    ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛    ┃
┃    SPOT 01        SPOT 02        SPOT 03        SPOT 04        SPOT 05        SPOT 06    ┃
┃     ■ ■ ■          ■ ■ ■          ■ ■ ■          ■ ■ ■          ■ ■ ■          ■ ■ ■     ┃
┃                                                                                          ┃
┣━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━ CONSOLE ━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                               ┃                                                          ┃ 
┃ 1: Answer spot question       ┃                                                          ┃
┃ 2: Answer castle question     ┃                                                          ┃
┃                               ┃                                                          ┃
┃ 0: Quit match                 ┃                                                          ┃
┃                               ┃                                                          ┃ 
┃                               ┃                                                          ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛



*/