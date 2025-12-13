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
#include "Plugins/Nlohmann Json/json.hpp"

#include "Definitions/TextStyle.hpp"

using namespace std;
using json = nlohmann::json;

string GetTeamColor(int team)
{
    switch (team)
    {
        case 1: return FG_RED;
        case 2: return FG_GREEN;
        case 3: return FG_YELLOW;
        case 4: return FG_BLUE;
        case 5: return FG_MAGENTA;
    }

    return "";
}

#endif