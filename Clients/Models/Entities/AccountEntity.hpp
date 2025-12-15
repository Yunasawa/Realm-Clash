#ifndef CLIENT_MODEL_ACCOUT_ENTITY
#define CLIENT_MODEL_ACCOUT_ENTITY

struct AccountEntity /*Tài khoản để đăng nhập*/
{
    string Email;
    string Password;

    string Serialize() const
    {
        json j;
        j["Email"] = Email;
        j["Password"] = Password;

        return j.dump();
        /*
        Email = "quang@gmail.com"
        Password = "123456"
        -> j = {
            "Email" : "quang@gmail.com",
            "Password" : "123456"
        }
        */
    }
};

#endif