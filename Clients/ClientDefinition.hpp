#ifndef CLIENT_DEFINITION
#define CLIENT_DECLARATION

#define WINDOW_WIDTH							70

#define LOG_NONE								FG_GREEN ""
#define LOG_UNKNOWN_COMMAND						FG_RED "Unknown command!"

#define LOG_LOBBY_BEING_RTLEADER				FG_GREEN "You are now the room and the team leader!"
#define LOG_LOBBY_BEING_RLEADER					FG_GREEN "You are now the room leader!"
#define LOG_LOBBY_BEING_TLEADER					FG_GREEN "You are now the team leader!"
#define LOG_LOBBY_WRONG_TEAM					FG_RED "Joined in wrong team!"
#define LOG_LOBBY_REQUEST_FULL					FG_RED "Joining request failed: Request list is full!"
#define LOG_LOBBY_REQUEST_ACCEPTED				FG_GREEN "Joining request accepted!"

#define LOG_GAME_WRONG_SPOT						FG_RED "Trying to occupy wrong spot!"
#define LOG_GAME_WRONG_RESOURCE_TYPE			FG_RED "Selected wrong resource type!"

//
#define CODE_NULL								"NULL"

// Client phases
#define PHASE_LOBBY_JOINING_READY				100
#define PHASE_LOBBY_JOINING_PENDING				101		

#define PHASE_LOBBY_JOINED_MEMBER				110
#define PHASE_LOBBY_JOINED_RLEADER				111                
#define PHASE_LOBBY_JOINED_TLEADER				112                
#define PHASE_LOBBY_JOINED_RTLEADER				113              

#define PHASE_GAME_MAP_IDLING					200
#define PHASE_GAME_QUESTION_ANSWERING			201
#define PHASE_GAME_MAP_COMBATING				202
#define PHASE_GAME_SHOPING_WEAPON				203
#define PHASE_GAME_SHOPING_DEFENSE				204
#define PHASE_GAME_CASTLE_ATTACKING				205
#define PHASE_GAME_ENDING						206

#endif