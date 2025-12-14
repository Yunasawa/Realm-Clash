#ifndef SERVER_MODEL_ENTITY_LOBBY
#define SERVER_MODEL_ENTITY_LOBBY

#include <array>

struct MemberEntity
{
    int ID; // AccountID

    bool IsRequestPending;

    void Reset()
    {
		ID = 0;
		IsRequestPending = false;
    }
};

struct TeamEntity
{
    array<MemberEntity, 3> Members;
    vector<int> JoinRequests;

    int CountFreeSlot() const
    {
        return count_if(Members.begin(), Members.end(),
            [](const MemberEntity& m)
            {
                return m.ID == 0;
            });
    }
    int CountMember() const
    {
        return 3 - CountFreeSlot();
    }
    int AssignFreeSlot(int account)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Members[i].ID == 0)
            {
                Members[i].ID = account;
                Members[i].IsRequestPending = false;
                return i;
            }
        }

        return -1;
    }
};

struct LobbyEntity
{
    array<TeamEntity, 5> Teams;

    string Serialize() const
    {
        json j;
        j["Teams"] = json::array();

        for (const auto& team : Teams)
        {
            json teamJson;
            teamJson["Members"] = json::array();

            for (const auto& member : team.Members)
            {
                if (member.ID == 0)
                {
                    teamJson["Members"].push_back(
                    {
                        {"ID", 0},
                        {"IsRequestPending", false},
                        {"Name", ""},
                        {"IsTeamLeader", false},
                        {"IsRoomLeader", false}
                    });
                }
                else
                {
                    auto account = Accounts[member.ID];
                    teamJson["Members"].push_back(
                    {
                        {"ID", member.ID},
                        {"IsRequestPending", member.IsRequestPending},
                        {"Name", account.Name},
                        {"IsTeamLeader", account.IsTeamLeader},
                        {"IsRoomLeader", account.IsRoomLeader}
                    });
                }
            }

            j["Teams"].push_back(teamJson);
        }

        return j.dump();
    }
    string Capture() const
    {
        stringstream ss;

        for (size_t ti = 0; ti < Teams.size(); ++ti)
        {
            const auto& team = Teams[ti];
            ss << "Team " << ti << ": [";

            for (size_t mi = 0; mi < team.Members.size(); ++mi)
            {
                const auto& m = team.Members[mi];

                auto& a = Accounts[m.ID];

                ss << "("
                    << m.ID << ","
                    << m.IsRequestPending << ","
                    << a.Name << ","
                    << a.IsRoomLeader << ","
                    << a.IsTeamLeader << ")";

                if (mi + 1 < team.Members.size())
                    ss << ",";
            }

            ss << "]";

            if (ti + 1 < Teams.size()) ss << ", ";
        }

        return ss.str();
    }

    void RemoveMember(int teamIndex, int id)
    {
        auto& account = Accounts[id];

        if (account.IsRoomLeader)
        {
            RoomLeader = 0;
            account.IsRoomLeader = false;

            if (JoinedMembers.size() > 1)
            {
                RoomLeader = JoinedMembers[1];
                Accounts[JoinedMembers[1]].IsRoomLeader = true;
            }
        }
        if (account.IsTeamLeader)
        {
            for (auto& member : Teams[account.Team].Members)
            {
                if (member.ID != 0 && member.ID != id)
                {
                    Accounts[member.ID].IsTeamLeader = true;
                    break;
                }
            }

            account.IsTeamLeader = false;
        }

        account.Team = -1;

        // Remove account
        for (auto& member : Teams[teamIndex].Members)
        {
			if (member.ID == id)
			{
				member.Reset();
				break;
			}
        }
    }
    int CountActiveTeam()
    {
        int count = 0;
        for (const auto& team : Teams)
        {
            if (team.CountMember() > 0)
            {
                count++;
            }
        }
        return count;
    }
};

#endif