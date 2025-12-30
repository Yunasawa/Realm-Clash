#ifndef INVENTORY_VIEW
#define INVENTORY_VIEW

/*
┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┃		                    INVENTORY							     ┃
┣━ YOUR TEAM HAVE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • Balista : x unit(s)                                              ┃
┃                                                                    ┃
┃ • Balista : x unit(s)                                              ┃
┃                                                                    ┃
┃ • Balista : x unit(s)                                              ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1: Move to shop view             ┃ 2: Move to map view             ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

string GetInventoryOption()
{
    return
        "┃ 1: Move to shop view              | 2: Move to map view            ┃\n";
            
}

void ShowInventoryView()
{
    ClearScreen();

    cout
        << GetGameTitle()
        << GetResourceLine() <<
        "┣━ YOUR TEAM HAVE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ • Balista : " << OwnInventory.Balista << " unit(s)" << string(52 - to_string(OwnInventory.Balista).length(), ' ') << "┃\n"
        "┃                                                                    ┃\n"
        "┃ • Catapult : " << OwnInventory.Catapult << " unit(s)" << string(51 - to_string(OwnInventory.Catapult).length(), ' ') << "┃\n"
        "┃                                                                    ┃\n"
        "┃ • Canon : " << OwnInventory.Canon << " unit(s)" << string(54 - to_string(OwnInventory.Canon).length(), ' ') << "┃\n"
        "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        << GetInventoryOption() <<
        "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

void ShowInventoryLog(string log){
    Log = log;

    ShowInventoryView();
}

#endif
