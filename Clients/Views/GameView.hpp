#ifndef VIEW_GAME
#define VIEW_GAME

/*
┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┃ Wood : 00000000 | Rock : 00000000 | Iron : 00000000 | Gold : 00000 ┃
┣━ GAME ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┫
┃ 🎪 Spot 1 |  Wood [ ]   Rock [ ]    Iron [ ]	┃  🏰 Castle 1 [ ]	 ┃
┃ 🎪 Spot 2 |  Wood [ ]   Rock [ ]    Iron [ ]	┃					 ┃
┃ 🎪 Spot 1 |  Wood [ ]   Rock [ ]    Iron [ ]	┃  🏰 Castle 1 [ ]	 ┃
┃ 🎪 Spot 3 |  Wood [ ]   Rock [ ]    Iron [ ]	┃					 ┃
┃ 🎪 Spot 1 |  Wood [ ]   Rock [ ]    Iron [ ]	┃  🏰 Castle 1 [ ]	 ┃
┃ 🎪 Spot 4 |  Wood [ ]   Rock [ ]    Iron [ ]	┃					 ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┫
┃ • 1 <teamID>: Create/Join team                                     ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

string ToTimeFormat(int tick)
{
	int hours = tick / 3600;
	int minutes = (tick % 3600) / 60;
	int seconds = tick % 60;
	stringstream ss;
	ss << setfill('0') << setw(2) << hours << ":"
		<< setfill('0') << setw(2) << minutes << ":"
		<< setfill('0') << setw(2) << seconds;
	return ss.str();
}

string GetGameTitle()
{
    string playerName = Account.Name;
	int teamID = Team + 1;
    string teamDot = GetTeamColor(teamID) + SQUARE + RESET;
	string teamBadge = GetTeamColor(teamID) + " Team " + to_string(teamID) + RESET + " ━ " + playerName + " ";
	string timeStamp = FG_BRIGHT_YELLOW " " + ToTimeFormat(Tick) + RESET;

    int fillLength = WINDOW_WIDTH - 35;
    stringstream ss;

	for (int i = 0; i < fillLength; ++i) ss << "━";

	return "┏━ " + teamDot + teamBadge + ss.str() + timeStamp + " ━┓\n";
}

string PadWithGrayZeros(int value, int width)
{
    string s = to_string(value);

    if ((int)s.size() >= width) return s;

    int zeroCount = width - s.size();

    return string(FG_GRAY) +
        string(zeroCount, '0') +
        RESET +
        s;
}

string GetResourceLine(int wood, int rock, int iron, int gold)
{
    stringstream ss;

    ss << "┃ "
        << "Wood : " << PadWithGrayZeros(wood, 8) << " | "
        << "Rock : " << PadWithGrayZeros(rock, 8) << " | "
        << "Iron : " << PadWithGrayZeros(iron, 8) << " | "
        << "Gold : " << PadWithGrayZeros(gold, 5)
        << " ┃\n";

    return ss.str();
}

string GetSpotLine(int spotID)
{
	auto spot = Map.Spots[spotID];
    return " 🎪 Spot " + to_string(spotID + 1) + " |  " + 
        "Wood [" + (spot.WoodSlot == -1 ? " " : GetTeamColor(spot.WoodSlot + 1) + to_string(spot.WoodSlot + 1)) + RESET + "]   " +
        "Rock [" + (spot.RockSlot == -1 ? " " : GetTeamColor(spot.RockSlot + 1) + to_string(spot.RockSlot + 1)) + RESET + "]   " +
        "Iron [" + (spot.IronSlot == -1 ? " " : GetTeamColor(spot.IronSlot + 1) + to_string(spot.IronSlot + 1)) + RESET + "]";
}

string GetCastleLine(int castleID)
{
    return "🏰 Castle " + to_string(castleID + 1) + " [ ]";
}

string GetGameOption()
{
    if (CurrentPhase == PHASE_GAME_MAP_IDLING)
    {
        return
            "┃ • 1 <spot> <type>: Occupy spot   | • 2 <castle>: Occupy castle     ┃\n";
    }

    return "";
}

void ShowGameView()
{
	ClearScreen();

	cout 
        << GetGameTitle()
        //"┃ Wood : 00000000 | Rock : 00000000 | Iron : 00000000 | Gold : 00000 ┃\n"
        << GetResourceLine(1233, 1231, 1231, 11) <<
        "┣━ GAME ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ " << GetSpotLine(0) << "  ┃  " << GetCastleLine(0) << "   ┃\n"
        "┃ " << GetSpotLine(1) << "  ┃                    ┃\n"
        "┃ " << GetSpotLine(2) << "  ┃  " << GetCastleLine(1) << "   ┃\n"
        "┃ " << GetSpotLine(3) << "  ┃                    ┃\n"
        "┃ " << GetSpotLine(4) << "  ┃  " << GetCastleLine(2) << "   ┃\n"
        "┃ " << GetSpotLine(5) << "  ┃                    ┃\n"
        "┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━┫\n"
            << GetGameOption() <<
        "┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n"
        "┃ " << BOLD << Log << RESET << string(72 - Log.length(), ' ') << "┃\n"
        "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";
}

void ShowGameLog(string log)
{
	Log = log;

	ShowGameView();
}
#endif