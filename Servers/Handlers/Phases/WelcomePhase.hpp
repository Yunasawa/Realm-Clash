#ifndef SERVER_HANDLER_PHASE_WELCOME
#define SERVER_HANDLER_PHASE_WELCOME

void HandleWelcomePhase(int clientFD, const string& code, const vector<string>& parts)
{
    if (code == RQ_SIGN_UP)
    {
        AccountRecord account = AccountRecord::Deserialize(parts[1]);
		WriteLog(LogType::Request, clientFD, "SIGN UP", account.Capture()); /*Server console*/

        HandleSignUp(clientFD, account);
    }
    else if (code == RQ_LOG_IN)
    {
        AccountRecord account = AccountRecord::Deserialize(parts[1]);
		WriteLog(LogType::Request, clientFD, "LOG IN ", account.Capture());

        HandleLogIn(clientFD, account);
    }
}

#endif