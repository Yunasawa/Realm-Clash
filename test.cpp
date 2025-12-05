#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <iomanip>

using namespace std;

void MoveUp(int n) {
    cout << "\033[" << n << "A";
}

void ClearLine() {
    cout << "\033[2K\r";  // Erase entire line
}

int main() {
    int tick = 0;

    const int UI_LINES = 6;

    // Print empty lines first (layout)
    for (int i = 0; i < UI_LINES; i++)
        cout << "\n";

    while (true) {
        // Get time
        auto now = chrono::system_clock::now();
        time_t tt = chrono::system_clock::to_time_t(now);
        tm* local = localtime(&tt);

        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "%I:%M:%S %p", local);

        // Colors
        const char* cyan   = "\033[36m";
        const char* yellow = "\033[33m";
        const char* green  = "\033[32m";
        const char* reset  = "\033[0m";

        // Go up to UI start
        MoveUp(UI_LINES);

        // Draw stable-width UI
        ClearLine(); cout << cyan   << "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓" << reset << "\n";
        ClearLine(); cout << green  << "┃        TIMER STATUS        ┃" << reset << "\n";
        ClearLine(); cout << cyan   << "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫" << reset << "\n";

        // Pad dynamic text so line length is stable
        string t = string("┃ Current Time: ") + timeStr;
        t.resize(31, ' ');
        t += "┃";

        string k = string("┃ Current Tick: ") + to_string(tick);
        k.resize(31, ' ');
        k += "┃";

        ClearLine(); cout << yellow << t << reset << "\n";
        ClearLine(); cout << yellow << k << reset << "\n";

        ClearLine(); cout << cyan   << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛" << reset << "\n";

        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
        tick++;
    }
}
