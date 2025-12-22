
```
Realm-Clash
├─ Clients
│  ├─ Client.cpp
│  ├─ Client.d
│  ├─ Client.o
│  ├─ ClientDefinition.hpp
│  ├─ ClientIncluding.hpp
│  ├─ ClientNetwork.hpp
│  ├─ ClientUtilities.hpp
│  ├─ Handlers
│  │  └─ Phases
│  │     ├─ PhaseGame.hpp
│  │     ├─ PhaseLobby.hpp
│  │     └─ PhaseWelcome.hpp
│  ├─ Models
│  │  ├─ Entities 
│  │  │  ├─ AccountEntity.hpp
│  │  │  ├─ CartEntity.hpp
│  │  │  └─ OccupationEntity.hpp
│  │  └─ Records
│  │     ├─ AccountRecord.hpp
│  │     ├─ LobbyRecord.hpp
│  │     ├─ MapRecord.hpp
│  │     └─ ResourceRecord.hpp
│  └─ Views
│     ├─ AttackView.hpp
│     ├─ GameView.hpp
│     ├─ LobbyView.hpp
│     ├─ QuestionView.hpp
│     ├─ ShopView.hpp
│     └─ WelcomeView.hpp
├─ Commons
│  ├─ CommonDefinition.hpp
│  ├─ CommonIncluding.hpp
│  ├─ CoreFunction.hpp
│  ├─ Definitions
│  │  └─ TextStyle.hpp
│  ├─ Networks
│  │  └─ MessageHandler.hpp
│  └─ Plugins
│     └─ Nlohmann Json
│        └─ json.hpp
├─ Datas
│  ├─ Account.ynl
│  ├─ Log.ynl
│  ├─ Question.ynl
│  └─ Status.ynl
├─ Functions
│  ├─ ResetData.cpp
│  ├─ ResetData.d
│  └─ ResetData.o
├─ Makefile
├─ Servers
│  ├─ Handlers
│  │  ├─ Handlers
│  │  │  ├─ AccountHandler.hpp
│  │  │  ├─ GameHandler.hpp
│  │  │  └─ LobbyHandler.hpp
│  │  └─ Phases
│  │     ├─ GamePhase.hpp
│  │     ├─ LobbyPhase.hpp
│  │     ├─ ProgressPhase.hpp
│  │     └─ WelcomePhase.hpp
│  ├─ Models
│  │  ├─ Entities
│  │  │  ├─ AccountEntity.hpp
│  │  │  ├─ GameEntity.hpp
│  │  │  └─ LobbyEntity.hpp
│  │  └─ Records
│  │     ├─ AccountRecord.hpp
│  │     └─ OccupationRecord.hpp
│  ├─ Server.cpp
│  ├─ Server.d
│  ├─ Server.o
│  ├─ ServerDefinition.hpp
│  ├─ ServerIncluding.hpp
│  ├─ ServerNetwork.hpp
│  └─ Utilities
│     ├─ ServerLogger.hpp
│     └─ ServerTimer.hpp
├─ Temps
│  ├─ Models
│  │  ├─ GameEntities.hpp
│  │  ├─ GetInfo.hpp
│  │  ├─ InGameClient.hpp
│  │  ├─ InGameServer.hpp
│  │  └─ Participant.hpp
│  ├─ host_remove_member.cpp
│  └─ test.cpp
├─ Tests
│  └─ test.cpp
├─ client
├─ reset
├─ server
└─ test

```