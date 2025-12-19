#ifndef SERVER_HANDLER_ACCOUNT
#define SERVER_HANDLER_ACCOUNT

string ToUID(int num)
{
    ostringstream ss;
    ss << "U" << setw(7) << setfill('0') << num;
    return ss.str();
}

int ToID(const string& s)
{
    return stoi(s.substr(1));
}

void HandleSignUp(int clientFD, const AccountRecord& account)
{
    if (FileExists(ACCOUNT_PATH))
    {
        string id, name, password;

        if (IsExistedAccount(ACCOUNT_PATH, account.Email, password, id, name))
        {
            SendMessage(clientFD, RS_SIGN_UP_F_ACCOUNT_EXISTED);
            WriteLog(LogType::Failure, clientFD, "SIGN UP : Account existed");
        }
        else
        {
            int accountID = atoi(ReadLine(STATUS_PATH, STATUS_LINE_ACCOUNT_ID).c_str());

            string accountName = ToUID(accountID);

            lock_guard<mutex> lock(SessionsMutex);
            Sessions[accountID].Initialize();

            AccountEntity acc
            {
                .ID = accountID,
                .Name = accountName,
            };
            Accounts[accountID] = acc;
            Clients[clientFD] = accountID;

            SendMessage(clientFD, string(RS_SIGN_UP_S) + " " + acc.Serialize());
            WriteLog(LogType::Success, clientFD, "SIGN UP", acc.Capture());

            stringstream ss;
            ss << accountID << " " << account.Email << " " << account.Password << " " << accountName;
            WriteAt(ACCOUNT_PATH, accountID, ss.str());
            WriteAt(STATUS_PATH, STATUS_LINE_ACCOUNT_ID, to_string(accountID + 1));
        }
    }
    else
    {
        cout << "File not exist" << endl;
    }
}

void HandleLogIn(int clientFD, const AccountRecord& account)
{
    if (FileExists(ACCOUNT_PATH))
    {
        string sid, name, password;

        if (IsExistedAccount(ACCOUNT_PATH, account.Email, password, sid, name))
        {
            int id = atoi(sid.c_str());

            if (Accounts.find(id) != Accounts.end())
            {
                SendMessage(clientFD, RS_LOG_IN_F_ACCOUNT_HAS_BEEN_USED);
                WriteLog(LogType::Failure, clientFD, "LOG IN : Account has been used");
            }
            else if (account.Password == password)
            {
                AccountEntity acc 
                {
                    .ID = id,
                    .Name = name,
                };
                Accounts[acc.ID] = acc;
                Clients[clientFD] = acc.ID;
                SendMessage(clientFD, string(RS_LOG_IN_S) + " " + acc.Serialize());
                WriteLog(LogType::Success, clientFD, "LOG IN", acc.Capture());
            }
            else
            {
                SendMessage(clientFD, RS_LOG_IN_F_WRONG_PASSWORD);
                WriteLog(LogType::Failure, clientFD, "LOG IN : Wrong password");
            }
        }
        else
        {
            SendMessage(clientFD, RS_LOG_IN_F_ACCOUNT_NOT_EXISTED);
            WriteLog(LogType::Failure, clientFD, "LOG IN : Account not existed");
        }
    }
    else
    {
        cout << "File not exist" << endl;
    }
}

#endif