#ifndef VIEW_ATTACK
#define VIEW_ATTACK

/*
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ U0000001 ━┓
┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━┫
┃												 ┃	   _   |~  _     ┃
┃ 1: Balista x10								 ┃	  [_]--'--[_]    ┃
┃ 2: Catapult x10                                ┃	  | | /^\ | |    ┃
┃ 3: Cannon x10                                  ┃    |_|_|I|_|_|    ┃
┃												 ┃                   ┃
┃												 ┃	  CASTLE 1 [5]   ┃	
┃												 ┃	   DEF: 10020	 ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━┫
┃ • 1 <weapon> <amount>: Attack using weapon	                     ┃
┃ • 2 : Back to map view					                         ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/
string GetAttackCastleOption()
{
    return
        "┃ • 1 <weapon> <amount>: Attack using weapon                         ┃\n"
        "┃ • 2 : Attack with all you have                                     ┃\n"
        "┃ • 3 : Back to map view                                             ┃\n";
}

// void ShowAttackCastleView()
// { 
//     ClearScreen();

//     cout
//         << GetGameTitle()
//         << GetResourceLine() <<
//         "┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┫\n"
//         "┃ • Balista x" << setw(34) << left << OwnInventory.Balista 
//         << "┃   	   _   |~  _      ┃\n"
//         "┃                                             ┃   	  [_]--'--[_]     ┃\n"
//         "┃ • Catapult x" << setw(33) << left << OwnInventory.Catapult 
//         << "┃   	  | | /^\\ | |     ┃\n"
//         "┃                                             ┃      |_|_|I|_|_|     ┃\n"
//         "┃ • Cannon x" << setw(35) << left << OwnInventory.Canon 
//         << "┃                      ┃\n"
//         "┃                                             ┃     CASTLE " << CurrentTargetCastle.Id 
//         << "  [" << CurrentTargetCastle.OwnerTeam << "]    ┃\n"
//         "┃                                             ┃      DEF : " << setw(10) << left << CurrentTargetCastle.Defense 
//         << "┃\n"
//         "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┫\n"
//         << GetAttackCastleOption() << 
//         "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
//         "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
//         "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
// }


void ShowAttackCastleView() {
    
    ClearScreen();

    cout << GetGameTitle() << GetResourceLine()
         << "┣━ LOBBY ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━┫\n"
         << "┃ • Balista x" << left << setw(34) << OwnInventory.Balista 
         << "┃      _        |~      ┃\n"
         << "┃                                               ┃   [___]--'--[___]     ┃\n"
         << "┃ • Catapult x" << left << setw(33) << OwnInventory.Catapult 
         << "┃   |   |  /^\\ |   |    ┃\n"
         << "┃                                               ┃   |___|_|I|_|___|     ┃\n"
         << "┃ • Cannon x" << left << setw(35) << OwnInventory.Canon 
         << "┃                       ┃\n"
         << "┃                                               ┃    CASTLE " << CurrentTargetCastle.Id 
         << " [" << CurrentTargetCastle.OwnerTeam << "]      ┃\n"
         << "┃                                               ┃    DEF : " << left << setw(12) 
         << CurrentTargetCastle.Defense << "┃\n"
         << "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━┫\n"
         << GetAttackCastleOption()
         << "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
         << "┃ " << BOLD << Log << RESET 
         << string(70 - Log.length(), ' ') << "┃\n"
         << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}


void ShowAttackCastleLog(string log)
{
    Log = log;

    ShowAttackCastleView();
}
#endif