#ifndef VIEW_GAME
#define VIEW_GAMEwsl

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

string GetResourceLine()
{
    array<int, 4> resource = { 0, 0, 0, 0 };

    if (Team >= 0 && Team < (int)Resource.Resources.size())
    {
        resource = Resource.Resources[Team];
    }

    stringstream ss;

    ss << "┃ "
        << "Wood : " << PadWithGrayZeros(resource[0], 8) << " | "
        << "Rock : " << PadWithGrayZeros(resource[1], 8) << " | "
        << "Iron : " << PadWithGrayZeros(resource[2], 8) << " | "
        << "Gold : " << PadWithGrayZeros(resource[3], 5)
        << " ┃\n";

    return ss.str();
}

string GetSpotLine(int spotID)
{
    auto GetSpotText = [&](int teamID) -> string
        {
            auto color = teamID == -1 ? FG_GRAY : GetTeamColor(teamID + 1);
            auto text = teamID == -1 ? " " : to_string(teamID + 1);
            return color + "[" + text + "]" + RESET;
        };

	auto spot = Map.Spots[spotID];

    return " 🎪 Spot " + to_string(spotID + 1) + " |  " + 
        "Wood " + GetSpotText(spot.WoodSlot) + "   " +
        "Rock " + GetSpotText(spot.RockSlot) + "   " +
        "Iron " + GetSpotText(spot.IronSlot) + "";
}

string GetCastleLine(int castleID)
{
    auto GetCastleText = [&](int teamID) -> string
        {
            auto color = teamID == -1 ? FG_GRAY : GetTeamColor(teamID + 1);
            auto text = teamID == -1 ? " " : to_string(teamID + 1);
            return color + "[" + text + "]" + RESET;
        };

    auto castle = Map.Castles[castleID];

    return "🏰 Castle " + to_string(castleID + 1) + " " + GetCastleText(castle.OwnerTeam);
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
        << GetResourceLine() <<
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