#ifndef CLIENT_HANDLER_PHASE_WELCOME
#define CLIENT_HANDLER_PHASE_WELCOME

void HandleWelcomeInput(int clientFD, vector<string> split)
{
    string message;
    int code = atoi(split[0].c_str());

    if (code == 1)
    {
        AccountEntity acc { split[1], split[2] };
        SendMessage(clientFD, string(RQ_SIGN_UP) + " " + acc.Serialize());
    }
    else if (code == 2)
    {
        AccountEntity acc { split[1], split[2] };
        SendMessage(clientFD, string(RQ_LOG_IN) + " " + acc.Serialize());
    }
}

void HandleWelcomeResponse(int clientFD, const string& code, vector<string> split)
{
    if (code == RS_NETWORK_CONNECTED ||
        code == RS_SIGN_UP_F_ACCOUNT_EXISTED ||
        code == RS_LOG_IN_F_WRONG_PASSWORD ||
        code == RS_LOG_IN_F_ACCOUNT_NOT_EXISTED ||
        code == RS_LOG_IN_F_ACCOUNT_HAS_BEEN_USED)
    {
        ShowWelcomeView(code);
    }
    else if (code == RS_SIGN_UP_S ||
             code == RS_LOG_IN_S)
    {
        Account = AccountRecord::Deserialize(split[1]);
        CurrentPhase = PHASE_LOBBY_JOINING_READY;

        ShowLobbyView(code);
        SendMessage(clientFD, string(RQ_UPDATE_LOBBY));
    }
}

#endif