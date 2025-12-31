#ifndef VIEW_RESULT
#define VIEW_RESULT

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>
#include <algorithm>
#include "../Models/Records/ResultRecord.hpp"
#include "../ClientDefinition.hpp"

string MakeResultTitle(const std::string& playerName)
{
    const int totalLen = 70;

    string bar = "━";

    int fillLen = totalLen - playerName.length() - 5;

    string fill;
    fill.reserve(fillLen * bar.length());

    for (int i = 0; i < fillLen; ++i)
    {
        fill += bar;
    }

    return "┏" + fill + " " + playerName + " ━┓";
}

void ShowResultView(const ResultRecord& record)
{
    ClearScreen();

    cout << MakeResultTitle(Account.Name) << "\n";
    const int inner = WINDOW_WIDTH - 4;

    cout << "┣━━ LEADER BOARD ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ POINT ━━┫\n";                                                                                 
    int rank = 1;
    for (const auto& p : record.Results)
    {
        string rankText  = "● " + to_string(rank);
        string teamText  = "Team " + to_string(p.first + 1);
        string pointText = to_string(p.second);

        const int colRank  = 6;
        const int colTeam  = inner - colRank - 8;

        cout << "┃ " << left << setw(colRank) << rankText
             << " ┃ " << left << setw(colTeam - 3) << teamText
             << " ┃ " <<setw(8)<< pointText << "┃\n";

        rank++;
    }

    cout << "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
    cout << "┃ • 1: Back to main menu                                             ┃\n";
    cout << "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n";
    string log = FG_GREEN "Game Finished. Waiting for next game...";
    cout << "┃ " << BOLD << log << RESET << string(inner - log.length() - 5, ' ') << " ┃\n";
    cout << "┗" << repeat_utf8("━", WINDOW_WIDTH - 2) << "┛\n";
}
#endif


