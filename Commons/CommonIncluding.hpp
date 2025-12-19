#ifndef CORE_INCLUDING
#define CORE_INCLUDING

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <atomic>
#include <functional>
#include <utility>
#include <array>
#include "Plugins/Nlohmann Json/json.hpp"

#include "Definitions/TextStyle.hpp"

#include "CommonDefinition.hpp"

using namespace std;
using json = nlohmann::json;

string GetTeamColor(int team)
{
    switch (team)
    {
        case 1: return FG_BRIGHT_RED;
        case 2: return FG_BRIGHT_GREEN;
        case 3: return FG_BRIGHT_YELLOW;
        case 4: return FG_BRIGHT_BLUE;
        case 5: return FG_BRIGHT_MAGENTA;
    }

    return "";
}

#endif