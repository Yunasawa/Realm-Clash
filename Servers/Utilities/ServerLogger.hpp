#ifndef UTILITES_SERVER_LOGGER
#define UTILITES_SERVER_LOGGER

enum class LogType
{
    Request,
    Success,
    Failure
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

inline const char* LogPrefix(LogType type)
{
    switch (type)
    {
        case LogType::Request: return "❓";
        case LogType::Success: return "➕";
        case LogType::Failure: return "➖";
    }
    return "";
}

inline void WriteLog(LogType type, int clientFD, const std::string& log)
{
    static std::mutex LogMutex;
    std::lock_guard<std::mutex> lock(LogMutex);

    if (!LogFile.is_open())
    {
        std::cerr << "LogFile not open!\n";
        return;
    }

    LogFile << Now()
        << " | FD:" << std::setw(3) << clientFD
        << " | " << LogPrefix(type)
        << " | " << log
        << '\n';

    LogFile.flush();
}

#endif