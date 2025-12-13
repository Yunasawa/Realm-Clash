#ifndef SERVER_MODEL_ENTITY_LOBBY
#define SERVER_MODEL_ENTITY_LOBBY

#include <array>

struct TeamEntity
{
    array<int, 3> Members;
    vector<int> JoinRequests;

    int CountFreeSlot()
    {
        int counter = 3;
        for (int i = 0; i < 3; i++)
        {
            if (Members[i] != 0) counter--;
        }

        return counter;
    }
    int CountMember() const
    {
        int counter = 0;
        for (int i = 0; i < 3; i++)
        {
            if (Members[i] != 0) counter++;
        }

        return counter;
    }
    int AssignFreeSlot(int account)
    {
        for (int i = 0; i < 3; i++)
        {
            if (Members[i] == 0)
            {
                Members[i] = account;
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

            for (const int& member : team.Members)
            {
                if (member == 0)
                {
                    teamJson["Members"].push_back(
                    {
                        {"ID", 0},
                        {"Name", ""},
                        {"IsTeamLeader", false},
                        {"IsRoomLeader", false}
                    });
                }
                else
                {
                    auto account = Accounts[member];
                    teamJson["Members"].push_back(
                    {
                        {"ID", member},
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

    void RemoveMember(int teamIndex, int id)
    {
        auto& mem = Teams[teamIndex].Members;

        auto& account = Accounts[id];
        account.Team = 0;

        if (account.IsRoomLeader)
        {
            if (JoinedMembers.size() > 1)
            {
                RoomLeader = JoinedMembers[1];
                Accounts[JoinedMembers[1]].IsRoomLeader = true;
            }

            RoomLeader = 0;
            account.IsRoomLeader = false;
        }
        if (account.IsTeamLeader)
        {
            if (Teams[account.Team].CountMember() > 1)
            {
                Accounts[Teams[account.Team].Members[1]].IsTeamLeader = true;
            }

            account.IsTeamLeader = false;
        }

        // Remove account
        auto endIt = remove(mem.begin(), mem.end(), id);

        // Fill tails with 0
        fill(endIt, mem.end(), 0);
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