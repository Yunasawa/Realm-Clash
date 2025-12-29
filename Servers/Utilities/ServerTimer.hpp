#ifndef UTILITIES_SERVER_TIMER
#define UTILITIES_SERVER_TIMER

void StartTickOnClient(int clientFD, int duration,
    const function<void(int, int)>& onTick = nullptr,
    const function<void(int)>& onEnd = nullptr)
{
    {
        lock_guard<mutex> lock(SessionsMutex);
        Sessions[clientFD].Initialize();
        Sessions[clientFD].Tick.store(true);
    }

    thread([clientFD, duration, onTick, onEnd]()
        {
            for (int i = 0; i <= duration; ++i)
            {
                bool counting;
                {
                    lock_guard<mutex> lock(SessionsMutex);
                    counting = Sessions[clientFD].Tick.load();
                }

                if (!counting) break;

                if (onTick) onTick(clientFD, i);

                this_thread::sleep_for(chrono::seconds(1));
            }

            bool stillTicking;
            {
                lock_guard<mutex> lock(SessionsMutex);
                stillTicking = Sessions[clientFD].Tick.load();
                Sessions[clientFD].Tick.store(false);
            }

            if (onEnd && stillTicking) onEnd(clientFD);

        }).detach();
}

void StopTickOnClient(int clientFD, const function<void(int)>& onStop = nullptr)
{
    lock_guard<mutex> lock(SessionsMutex);

    if (Sessions[clientFD].Tick.load() == false) return;

    Sessions[clientFD].Tick.store(false);

    if (onStop) onStop(clientFD);
}

void StartTickOnServer(
    const function<void(int)>& onTick = nullptr,
    const function<void()>& onEnd = nullptr)
{
    bool expected = false;
    if (!ServerTicking.compare_exchange_strong(expected, true))
    {
        return;
    }

    ServerTickThread = thread([onTick, onEnd]()
        {
            int tick = 0;

            while (ServerTicking.load())
            {
                if (onTick) onTick(tick);

                tick++;
                this_thread::sleep_for(chrono::seconds(1));
            }

            if (onEnd) onEnd();
        });

    ServerTickThread.detach();
}

void StopTickOnServer()
{
    ServerTicking.store(false);

    if (ServerTickThread.joinable())
    {
        ServerTickThread.join();
    }
}

#endif
