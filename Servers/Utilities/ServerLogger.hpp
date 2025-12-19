#ifndef UTILITES_SERVER_LOGGER
#define UTILITES_SERVER_LOGGER

enum class LogType
{
    Request,
    Success,
    Failure,
    Update
};

inline string Now()
{
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    ostringstream ss;
    ss << put_time(&tm, "%d/%m/%Y %H:%M:%S");
    return ss.str();
}

inline const string LogPrefix(LogType type)
{
    switch (type)
    {
        case LogType::Request: return "❓";
        case LogType::Success: return "➕";
        case LogType::Failure: return "➖";
        case LogType::Update: return "❌";
    }
    return "";
}

inline const string CodePrefix(LogType type)
{
    switch (type)
    {
        case LogType::Request: return "REQUEST | ";
        case LogType::Success: return "SUCCESS | ";
        case LogType::Failure: return "FAILURE | ";
        case LogType::Update: return "UPDATE  | ";
    }
    return "";
}

inline void WriteLog(LogType type, int clientFD, const std::string& code, const std::string& data = "")
{
    static std::mutex LogMutex;
    std::lock_guard<std::mutex> lock(LogMutex);

    if (!LogFile.is_open())
    {
        std::cerr << "LogFile not open!\n";
        return;
    }

    LogFile << Now()
        << " ┃ " << std::setw(2) << std::right << clientFD
        << " ┃ " << LogPrefix(type)
		<< " ┃ " << std::setw(70) << std::left << (CodePrefix(type) + code)
		<< " ┃ " << data
        << '\n';

    LogFile.flush();
}

#endif