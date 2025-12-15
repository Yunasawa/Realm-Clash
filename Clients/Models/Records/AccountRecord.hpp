#ifndef SERVER_MODEL_ACCOUT_RECORD
#define SERVER_MODEL_ACCOUT_RECORD

struct AccountRecord /*Account <=> Member*/
{
    int ID;
    string Name;

    static AccountRecord Deserialize(const string& msg)
    {
        json j = json::parse(msg);

        AccountRecord r;
        r.ID = j.value("ID", 0); /*Đọc trường ID nếu không có thì mặc định = 0*/
        r.Name = j.value("Name", "");
        return r;
    }
};

#endif