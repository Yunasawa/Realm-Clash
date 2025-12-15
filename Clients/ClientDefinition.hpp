#ifndef CLIENT_DEFINITION
#define CLIENT_DECLARATION

#define LOG_NONE								FG_GREEN ""
#define LOG_UNKNOWN_COMMAND						FG_RED "Unknown command!"

#define LOG_LOBBY_BEING_RTLEADER				FG_GREEN "You are now the room and the team leader!"
#define LOG_LOBBY_BEING_RLEADER					FG_GREEN "You are now the room leader!"
#define LOG_LOBBY_BEING_TLEADER					FG_GREEN "You are now the team leader!"
#define LOG_LOBBY_WRONG_TEAM					FG_RED "Joined in wrong team!"
#define LOG_LOBBY_REQUEST_FULL					FG_RED "Joining request failed: Request list is full!"
#define LOG_LOBBY_REQUEST_ACCEPTED				FG_GREEN "Joining request accepted!"

//
#define CODE_NULL								"NULL"

// Client phases
<<<<<<< Updated upstream
#define PHASE_WELCOME							000

#define PHASE_LOBBY_JOINING_READY				100
#define PHASE_LOBBY_JOINING_PENDING				101		

#define PHASE_LOBBY_JOINED_MEMBER				110
#define PHASE_LOBBY_JOINED_RLEADER				111                
#define PHASE_LOBBY_JOINED_TLEADER				112                
#define PHASE_LOBBY_JOINED_RTLEADER				113     
=======
#define PHASE_WELCOME                       000 /* thông điệp welcome */
#define PHASE_LOBBY_JOINING                 100 /* state */
#define PHASE_LOBBY_JOINED_MEMBER           101 /* role thành viên */
#define PHASE_LOBBY_JOINED_RLEADER          102                
#define PHASE_LOBBY_JOINED_TLEADER          103                
#define PHASE_LOBBY_JOINED_RTLEADER         104                
>>>>>>> Stashed changes

#endif