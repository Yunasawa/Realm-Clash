#ifndef CORE_DEFINITION
#define CORE_DEFINITION

#define SERVER_PORT 5874

// Client request protocols
#define CREATE_ROOM                         "CREATE_ROOM"
#define JOIN_ROOM                           "JOIN_ROOM"
#define EXIT_ROOM                           "EXIT_ROOM"
#define REQUEST_SPOT                        "REQUEST_SPOT"
#define ANSWER_SPOT                         "ANSWER_SPOT"
#define REQUEST_CASTLE                      "REQUEST_CASTLE"
#define ANSWER_CASTLE                       "ANSWER_CASTLE"
#define BUY_DEFENSE                         "BUY_DEFENSE"
#define BUY_WEAPON                          "BUY_WEAPON"
#define ATTACK_CASTLE                       "ATTACK_CASTLE"
#define GET_STATUS                          "GET_STATUS"

// Server response protocols
#define CREATE_ROOM_S                       110
#define JOIN_ROOM_S                         111
#define JOIN_ROOM_F_ROOM_FULL               101
#define JOIN_ROOM_F_SAME_NAME               102
#define EXIT_ROOM_S                         112
#define EXIT_ROOM_F_MATCH_STARTED           103

#define REQUEST_QUESTION_S                  210
#define REQUEST_QUESTION_F_SLOT_OCCUPIED    201
#define REQUEST_QUESTION_F_SLOT_LIMIT       202

#define ANSWER_QUESTION_S                   310
#define ANSWER_QUESTION_F_WRONG_ANSWER      301
#define ANSWER_QUESTION_F_TIME_OUT          302

#define GIVE_RESOURCE_S                     410
#define GIVE_RESOURCE_F                     401

#define SHOP_EQUIPMENT_S                    510
#define SHOP_EQUIPMENT_F_LACK_RESOURCE      501

#define ATTACK_CASTLE_S                     610
#define ATTACK_CASTLE_F_INSUFFICIENT_POWER  601

#define STATUS_UPDATE_RESOURCE              911
#define STATUS_UPDATE_TEAM                  912

struct MessageData
{
    string text;
    string time;
};

void to_json(json& j, const MessageData& m)
{
    j = json{
        {"text", m.text},
        {"time", m.time}
    };
}

void from_json(const json& j, MessageData& m)
{
    m.text = j.at("text").get<string>();
    m.time = j.at("time").get<string>();
}

#endif