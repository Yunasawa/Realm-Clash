#ifndef SERVER_MODEL_RECORD_LOBBY
#define SERVER_MODEL_RECORD_LOBBY

struct MemberRecord 
{
    int ID;
    string Name;
    bool IsRequestPending;
    bool IsTeamLeader;
    bool IsRoomLeader;
};

struct TeamRecord
{
    array<MemberRecord, 3> Members; /*Mỗi team tối đa 3 thành viên */
};

struct LobbyRecord
{
    array<TeamRecord, 5> Teams; /*Mỗi phòng tối đa 5 team */

    static LobbyRecord Deserialize(const string& data) /*Convert json -> struct LobbyRecord*/
    {
        json j = json::parse(data);

        LobbyRecord r;

        if (!j.contains("Teams") || !j["Teams"].is_array()) 
            return r;

        for (int t = 0; t < 5; t++)
        {
            if (t >= (int)j["Teams"].size())
                break;

            const auto& teamJson = j["Teams"][t];

            if (!teamJson.contains("Members") || !teamJson["Members"].is_array())
                continue;

            for (int i = 0; i < 3; i++)
            {
                MemberRecord m;

                if (i < (int)teamJson["Members"].size())
                {
                    const auto& memberJson = teamJson["Members"][i];

                    m.ID               = memberJson.value("ID", 0);
                    m.Name             = memberJson.value("Name", "");
                    m.IsRequestPending = memberJson.value("IsRequestPending", false);
                    m.IsTeamLeader     = memberJson.value("IsTeamLeader", false);
                    m.IsRoomLeader     = memberJson.value("IsRoomLeader", false);
                }
                else
                {
                    m.ID = 0;
                    m.Name = "";
                    m.IsRequestPending = false;
                    m.IsTeamLeader = false;
                    m.IsRoomLeader = false;
                }

                r.Teams[t].Members[i] = m;
            }
        }

        return r;
    }

    MemberRecord GetMember(int id)
    {
        for (const auto& team : Teams)
        {
            for (const auto& member : team.Members)
            {
                if (member.ID == id) return member;
            }
        }

        return MemberRecord{}; /*Nếu duyệt LobbyRecord hiện tại mà không tồn tại -> giá trị khởi tạo mặc định*/
    }
};

#endif