#ifndef SERVER_HANDLER_PHASE_WELCOME
#define SERVER_HANDLER_PHASE_WELCOME

void HandleWelcomePhase(int clientFD, const string& code, const vector<string>& parts)
{
    if (code == RQ_SIGN_UP)
    {
        HandleSignUp(clientFD, parts[1]);
		WriteLog(LogType::Request, clientFD, "SIGN_UP " + parts[1]);
    }
    else if (code == RQ_LOG_IN)
    {
        HandleLogIn(clientFD, parts[1]);
		WriteLog(LogType::Request, clientFD, "LOG_IN " + parts[1]);
    }
}

#endif