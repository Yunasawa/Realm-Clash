#ifndef SERVER_MODEL_ACCOUT_RECORD
#define SERVER_MODEL_ACCOUT_RECORD

struct AccountRecord
{
    string Email;
    string Password;

    static AccountRecord Deserialize(const string& msg)
    {
        json j = json::parse(msg);

        AccountRecord r;
        r.Email    = j.value("Email", "");
        r.Password = j.value("Password", "");
        return r;
    }
	string Capture() const
	{
		return "Email: " + Email + ", Password: " + Password;
	}
};

#endif