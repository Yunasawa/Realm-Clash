#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// ANSI màu
string red    = "\033[31m";
string blue   = "\033[34m";
string green  = "\033[32m";
string yellow = "\033[33m";
string purple = "\033[35m";
string cyan   = "\033[36m";
string reset  = "\033[0m";

string C(string color, string s) { return color + s + reset; }

int main() {
    srand(time(NULL));

    vector<string> fiveColors = { red, blue, green, yellow, purple };

    string randomPlayerColor = fiveColors[rand() % fiveColors.size()];

    // màu dưới castle
    vector<string> castleColors = { red, blue, green };

    // màu dưới 6 spot (6 màu khác nhau)
    vector<string> spotColors = { yellow, purple, red, blue, green, cyan };

    // -------------------------------------------------------

    cout <<
    "┏━ "
    << C(red,   "■") << " "
    << C(blue,  "■") << " "
    << C(green, "■") << " "
    << C(yellow,"■") << " "
    << C(purple,"■")
    << " ━━━ "
    << C(randomPlayerColor, "■")
    << " playerA ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓" << endl;

    cout << "┣━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━┫\n";
    cout << "┃   W: 100.000 (+30)   ┃   S: 100.000 (+10)   ┃   I: 100.000 (+10)   ┃   G: 100.000 (+1)   ┃\n";
    cout << "┣━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━┫\n";

    cout << "┃           ┣╋━╋━╋━╋┫                    ┣╋━╋━╋━╋┫                     ┣╋━╋━╋━╋┫           ┃\n";
    cout << "┃           ┗╋━┻━┻━╋┛                    ┗╋━┻━┻━╋┛                     ┗╋━┻━┻━╋┛           ┃\n";

    // CASTLE window (no color)
    cout << "┃            ┃  ■  ┃                      ┃  ■  ┃                       ┃  ■  ┃            ┃\n";
    cout << "┃            ┃  ■  ┃                      ┃  ■  ┃                       ┃  ■  ┃            ┃\n";

    cout << "┃            ┗━━━━━┛                      ┗━━━━━┛                       ┗━━━━━┛            ┃\n";
    cout << "┃           CASTLE 01                    CASTLE 02                     CASTLE 03           ┃\n";

    // Colored blocks below castle
    cout << "┃              "
         << C(castleColors[0], "■■■")
         << "                          "
         << C(castleColors[1], "■■■")
         << "                           "
         << C(castleColors[2], "■■■")
         << "              ┃\n";

    cout << "┃                                                                                          ┃\n";

    // SPOT structure (windows no color)
    cout << "┃    ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫        ┣━━┻━━┫    ┃\n";
    cout << "┃    ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃        ┃ ■■■ ┃    ┃\n";
    cout << "┃    ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛        ┗━━━━━┛    ┃\n";


    cout << "┃    SPOT 01        SPOT 02        SPOT 03        SPOT 04        SPOT 05        SPOT 06    ┃\n";

    cout << "┃     ";
    for (int i = 0; i < 6; i++) {
        cout << C(spotColors[i], "■") << " "
            << C(spotColors[i], "■") << " "
            << C(spotColors[i], "■");

        if (i < 5) cout << "          ";
    }
    cout << "     ┃\n";

    cout << "┃                                                                                          ┃\n";
    cout << "┣━━━━━━━━━━━ OPTIONS ━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━ CONSOLE ━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
    cout << "┃                               ┃                                                          ┃\n";
    cout << "┃ 1: Answer spot question       ┃                                                          ┃\n";
    cout << "┃ 2: Answer castle question     ┃                                                          ┃\n";
    cout << "┃                               ┃                                                          ┃\n";
    cout << "┃ 0: Quit match                 ┃                                                          ┃\n";
    cout << "┃                               ┃                                                          ┃\n";
    cout << "┃                               ┃                                                          ┃\n";
    cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";

    return 0;
}
