#ifndef SERVER_HANDLER_ACCOUNT
#define SERVER_HANDLER_ACCOUNT

string ToUID(int num)
{
    ostringstream ss;
    ss << "U" << setw(9) << setfill('0') << num;
    return ss.str();
}

void HandleSignUp(int clientFD, string data)
{
    AccountRecord account = AccountRecord::Deserialize(data);

    if (FileExists(ACCOUNT_PATH))
    {
        string id, name, password;

        if (IsExistedAccount(ACCOUNT_PATH, account.Email, password, id, name))
        {
            SendMessage(clientFD, RS_SIGN_UP_F_ACCOUNT_EXISTED);
        }
        else
        {
            int accountID = atoi(ReadLine(STATUS_PATH, STATUS_LINE_ACCOUNT_ID).c_str());

            string accountName = ToUID(accountID);

            AccountEntity acc 
            {
                .ID = accountID,
                .Name = accountName,
            };
            Accounts[acc.ID] = acc;
            Clients[clientFD] = acc.ID;
            SendMessage(clientFD, string(RS_SIGN_UP_S) + " " + acc.Serialize());

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

void HandleLogIn(int clientFD, string data)
{
    AccountRecord account = AccountRecord::Deserialize(data);

    if (FileExists(ACCOUNT_PATH))
    {
        string sid, name, password;

        if (IsExistedAccount(ACCOUNT_PATH, account.Email, password, sid, name))
        {
            int id = atoi(sid.c_str());

            if (Accounts.find(id) != Accounts.end())
            {
                cout << "AAAAA" << endl;
                SendMessage(clientFD, RS_LOG_IN_F_ACCOUNT_HAS_BEEN_USED);
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
            }
            else
            {
                SendMessage(clientFD, RS_LOG_IN_F_WRONG_PASSWORD);
            }
        }
        else
        {
            SendMessage(clientFD, RS_LOG_IN_F_ACCOUNT_NOT_EXISTED);
        }
    }
    else
    {
        cout << "File not exist" << endl;
    }
}

#endif