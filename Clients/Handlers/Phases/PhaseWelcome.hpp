#ifndef CLIENT_HANDLER_PHASE_WELCOME
#define CLIENT_HANDLER_PHASE_WELCOME

void HandleWelcomeInput(int clientFD, vector<string> command)
{
    string message;
    int code = atoi(command[0].c_str());

    if (code == 1 && command.size() == 3)
    {
        AccountEntity acc{ command[1], command[2] };
        SendMessage(clientFD, string(RQ_SIGN_UP) + " " + acc.Serialize());
    }
    else if (code == 2 && command.size() == 3)
    {
        AccountEntity acc{ command[1], command[2] };
        SendMessage(clientFD, string(RQ_LOG_IN) + " " + acc.Serialize());
    }
    else
    {
        ShowWelcomeLog(LOG_UNKNOWN_COMMAND); /*Log là global scope variable*/
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
        ShowWelcomeCode(code);
    }
    else if (code == RS_SIGN_UP_S ||
             code == RS_LOG_IN_S)
    {
        Account = AccountRecord::Deserialize(split[1]);
        CurrentPhase = PHASE_LOBBY_JOINING_READY; 
        /*Đăng nhập thành công thì chuyển trạng thái của CurrentPhase*/
        ShowLobbyCode(code);
        SendMessage(clientFD, string(RQ_UPDATE_LOBBY));
    }
}

#endif