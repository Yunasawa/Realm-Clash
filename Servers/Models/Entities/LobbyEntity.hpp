#ifndef SERVER_MODEL_ENTITY_LOBBY
#define SERVER_MODEL_ENTITY_LOBBY

struct LobbyMemberEntity
{
    int ID; // AccountID

    bool IsRequestPending;

    void Reset()
    {
		ID = 0;
		IsRequestPending = false;
    }
};

struct LobbyTeamEntity
{
    vector<int> JoinRequests;
    array<LobbyMemberEntity, 3> Members;

    int CountMember(bool includingRequest = false) const
    {
        return count_if(Members.begin(), Members.end(),
            [includingRequest](const LobbyMemberEntity& m)
            {
                if (includingRequest)
                {
					return m.ID != 0;
                }
                else
                {
                    return m.ID != 0 && m.IsRequestPending == false;
                }
            });
    }
    int CountFreeSlot(bool includingRequest = false) const
    {
		return 3 - CountMember(includingRequest);
    }
    int AssignFreeSlot(int account, bool isPending = false)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Members[i].ID == 0)
            {
                Members[i].ID = account;
                Members[i].IsRequestPending = isPending;
                return i;
            }
        }

        return -1;
    }
    LobbyMemberEntity* CheckIfPending(int id)
    {
        for (auto& member : Members)
        {
            if (member.ID == id && member.IsRequestPending)
                return &member;
        }
        return nullptr;
    }
};

struct LobbyEntity
{
    array<LobbyTeamEntity, 5> Teams;

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

    pair<int, int> RemoveMember(int teamIndex, int id)
    {
        auto& account = Accounts[id];

        auto newRoomLeader = 0;
        auto newTeamLeader = 0;

        if (account.IsRoomLeader)
        {
            RoomLeader = 0;
            account.IsRoomLeader = false;

            if (JoinedMembers.size() > 1)
            {
                newRoomLeader = JoinedMembers[1];

                RoomLeader = newRoomLeader;
                Accounts[newRoomLeader].IsRoomLeader = true;
            }
        }
        if (account.IsTeamLeader)
        {
            for (auto& member : Teams[account.LobbyTeam].Members)
            {
                if (member.ID != 0 && member.ID != id)
                {
                    newTeamLeader = member.ID;

                    Accounts[newTeamLeader].IsTeamLeader = true;
                    break;
                }
            }

            account.IsTeamLeader = false;
        }

        account.LobbyTeam = -1;

        // Remove account
        auto& members = Teams[teamIndex].Members;

        auto it = find_if(members.begin(), members.end(), [&](const LobbyMemberEntity& m) { return m.ID == id; });
    
        if (it != members.end())
        {
            move(it + 1, members.end(), it);
            members.back().Reset();
        }

		return { newRoomLeader, newTeamLeader };
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
    int CountTeam()
    {
		return count_if(Teams.begin(), Teams.end(),
			[](const LobbyTeamEntity& team)
			{
				return team.CountMember(true) > 0;
			});
    }
};

#endif