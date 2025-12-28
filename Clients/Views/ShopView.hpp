#ifndef VIEW_SHOP
#define VIEW_SHOP

/*
┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┃ Wood : 00000000 | Rock : 00000000 | Iron : 00000000 | Gold : 00000 ┃
┣━ WEAPON SHOP ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1: Balista      | ATK: 1000 | Wood: 1500 | Rock:    0 | Iron:  200 ┃
┃ 2: Catapult     | ATK: 3000 | Wood:  400 | Rock: 1500 | Iron:  300 ┃
┃ 3: Cannon       | ATK: 8000 | Wood:  500 | Rock: 2500 | Iron: 1800 ┃
┣━ OPTIONS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1 <ID> <amount> : Buy weapon     | 2: Switch to defense shop       ┃
┃ 3: Back to main screen           | 4: Show inventory               ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┃ Wood : 00000000 | Rock : 00000000 | Iron : 00000000 | Gold : 00000 ┃
┣━ DEFENSE SHOP ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1: Fence        | DEF:  200 | Wood:  200 | Rock:   50 | Iron:    0 ┃
┃ 2: Wood Wall    | DEF: 1000 | Wood: 1000 | Rock:  100 | Iron:  100 ┃
┃ 3: Stone Wall   | DEF: 3000 | Wood:  200 | Rock: 1000 | Iron:  200 ┃
┃ 4: Legend Wall  | DEF: 8000 | Wood: 1000 | Rock: 2000 | Iron: 1000 ┃
┣━ OPTIONS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1 <ID> <amount> <castle> : Buy defense | 2: Switch to weapon shop  ┃
┃ 3: Back to main screen                 | 4: Show inventory         ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

*/

string GetShopOption()
{
    if (CurrentPhase == PHASE_GAME_SHOPING_WEAPON)
    {
        return
            "┃ • 1 <id> <amount>: Buy weapon    | • 2: Move to defense shop       ┃\n"
            "┃ • 3: Back to main screen         | • 4: Show inventory             ┃\n";
    }
    else if (CurrentPhase == PHASE_GAME_SHOPING_DEFENSE)
    {
        return
            "┃ • 1 <id> <amount> <castle> : Buy defense | • 2: Move to weapon shop┃\n"
            "┃ • 3: Back to main screen                 | • 4: Show inventory     ┃\n";
    }

    return "";
}

void ShowAttackShopView()
{
    ClearScreen();

    cout
        << GetGameTitle()
        << GetResourceLine() <<
        "┣━ WEAPON SHOP ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ 1: Balista      | ATK: 1000 | Wood: 1500 | Rock:    0 | Iron:  200 ┃\n"
        "┃ 2: Catapult     | ATK: 3000 | Wood:  400 | Rock: 1500 | Iron:  300 ┃\n"
        "┃ 3: Cannon       | ATK: 8000 | Wood:  500 | Rock: 2500 | Iron: 1800 ┃\n"
        "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        << GetShopOption() <<
        "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

void ShowAttackShopLog(string log)
{
    Log = log;

    ShowAttackShopView();
}

void ShowDefenseShopView()
{
    ClearScreen();

    cout
        << GetGameTitle()
        << GetResourceLine() <<
        "┣━ DEFENSE SHOP ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ 1: Fence        | DEF:  200 | Wood:  200 | Rock:   50 | Iron:    0 ┃\n"
        "┃ 2: Wood Wall    | DEF: 1000 | Wood: 1000 | Rock:  100 | Iron:  100 ┃\n"
        "┃ 3: Stone Wall   | DEF: 3000 | Wood:  200 | Rock: 1000 | Iron:  200 ┃\n"
        "┃ 4: Legend Wall  | DEF: 8000 | Wood: 1000 | Rock: 2000 | Iron: 1000 ┃\n"
        "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        << GetShopOption() <<
        "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

void ShowDefenseShopLog(string log)
{
    Log = log;

    ShowDefenseShopView();
}

#endif