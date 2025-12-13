#ifndef CORE_DEFINITION
#define CORE_DEFINITION

#define SERVER_PORT                             5874
#define CLIENT_IP                               "192.168.10.1" //"127.0.0.1"

// Client request protocols
#define RQ_SIGN_UP                              "SIGN_UP"
#define RQ_LOG_IN                               "LOG_IN"

#define RQ_UPDATE_LOBBY                         "UPDATE_LOBBY"
#define RQ_JOIN_TEAM                            "JOIN_TEAM"
#define RQ_CANCEL_JOINING                       "CANCEL_JOINING"
#define RQ_ACCEPT_PARTICIPATION                 "ACCEPT_PARTICIPATION"
#define RQ_ADD_MEMBER                           "ADD_MEMBER"
#define RQ_INVITE_MEMBER                        "INVITE_MEMBER"
#define RQ_EXIT_TEAM                            "EXIT_TEAM"
#define RQ_KICK_MEMBER                          "KICK_MEMBER"

#define RQ_START_GAME                           "START_GAME"
#define RQ_REQUEST_SPOT                         "REQUEST_SPOT"
#define RQ_ANSWER_SPOT                          "ANSWER_SPOT"
#define RQ_REQUEST_CASTLE                       "REQUEST_CASTLE"
#define RQ_ANSWER_CASTLE                        "ANSWER_CASTLE"
#define RQ_BUY_DEFENSE                          "BUY_DEFENSE"
#define RQ_BUY_WEAPON                           "BUY_WEAPON"
#define RQ_ATTACK_CASTLE                        "ATTACK_CASTLE"
#define RQ_GET_STATUS                           "GET_STATUS"

// Server response protocols
#define RS_SIGN_UP_S                            "110"
#define RS_SIGN_UP_F_ACCOUNT_EXISTED            "101"
#define RS_LOG_IN_S                             "111"
#define RS_LOG_IN_F_ACCOUNT_NOT_EXISTED         "102"
#define RS_LOG_IN_F_WRONG_PASSWORD              "103"
#define RS_LOG_IN_F_ACCOUNT_HAS_BEEN_USED       "104"

#define RS_JOIN_TEAM_S                          "210"
#define RS_JOIN_TEAM_F_TEAM_FULL                "201"
#define RS_JOIN_TEAM_F_SAME_NAME                "202"
#define RS_EXIT_TEAM_S                          "212"
#define RS_EXIT_TEAN_F_MATCH_STARTED            "203"
#define RS_ADD_MEMBER_S                         "213"
#define RS_ADD_MEMBER_F_NOT_FOUND               "204"
#define RS_ADD_MEMBER_F_TEAM_FULL               "205"

#define RS_REQUEST_QUESTION_S                   "310"
#define RS_REQUEST_QUESTION_F_SLOT_OCCUPIED     "301"
#define RS_REQUEST_QUESTION_F_SLOT_LIMIT        "302"

#define RS_ANSWER_QUESTION_S                    "410"
#define RS_ANSWER_QUESTION_F_WRONG_ANSWER       "401"
#define RS_ANSWER_QUESTION_F_TIME_OUT           "402"

#define RS_GIVE_RESOURCE_S                      "510"
#define RS_GIVE_RESOURCE_F                      "501"

#define RS_SHOP_EQUIPMENT_S                     "610"
#define RS_SHOP_EQUIPMENT_F_LACK_RESOURCE       "601"

#define RS_ATTACK_CASTLE_S                      "710"
#define RS_ATTACK_CASTLE_F_INSUFFICIENT_POWER   "701"

#define RS_UPDATE_ROOM_LIST                     "810"
#define RS_UPDATE_TEAM_ROLE                     "811"
#define RS_UPDATE_JOIN_REQUEST					"812"
#define RS_UPDATE_PENDING_JOIN					"813"

#define RS_NETWORK_CONNECTED                    "910"

// Client console messages
#define CS_SIGN_UP_F_ACCOUNT_EXISTED            "Sign up failed: Account existed"
#define CS_LOG_IN_F_ACCOUNT_NOT_EXISTED         "Log in failed: Account not existed"
#define CS_LOG_IN_F_WRONG_PASSWORD              "Log in failed: Wrong password"
#define CS_LOG_IN_F_ACCOUNT_HAS_BEEN_USED       "Log in failed: Account has been used"

#endif