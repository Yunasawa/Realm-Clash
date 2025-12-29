#ifndef SERVER_HANDLER_GAME
#define SERVER_HANDLER_GAME


#include "../../../Commons/CommonIncluding.hpp"
#include "../../Models/Entities/GameEntity.hpp"
#include <random>


extern QuestionBankEntity QuestionBank;

void ResetGame();
extern MapEntity Map;
extern QuestionEntity TargetSpotQuestion;
extern QuestionEntity TargetCastleQuestion;

void UpdateResourcesQuantity(TeamEntity& team, unordered_map<ResourceType,int> cost, int amount);
int ResourceCompare(const TeamEntity& team, unordered_map<ResourceType,int> require, int amount);
pair<int,QuestionEntity> ChangeSpotQuestion(int currentQuestionID);
pair<int,QuestionEntity> ChangeCastleQuestion(int currentQuestionID);

void RequestStopServerTick()
{
    ServerTicking.store(false);
}
int GetResultPoint(int teamID) {
    int point = 0;
    auto& team = Group.Teams[teamID];

    // Tính điểm từ tài nguyên
    point += team.Resources[0];       // Wood
    point += team.Resources[1] * 2;   // Stone
    point += team.Resources[2] * 3;   // Iron
    point += team.Resources[3] * 10;  // Gold

    return point;
}

pair<int, int> GetResult(int teamID) {
    int point = GetResultPoint(teamID);
    return make_pair(teamID, point);
}

string HandleEndGame() 
{
    vector<pair<int, int>> allResults;
    
    for (int id = 0; id < Group.Teams.size(); ++id) {
        allResults.push_back(GetResult(id));
    }

    std::sort(allResults.begin(), allResults.end(), 
        [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second > b.second;
    });

    json j;
    j["result"] = json::array();

    for (const auto& res : allResults) {
        json teamData;
        teamData["id"] = res.first;
        teamData["point"] = res.second;
        j["result"].push_back(teamData);
    }

    /* Cấu trúc JSON gửi về client sẽ trông như thế này:
       {
           "result": [
               { "id": 2, "point": 1500 },  <- Top 1
               { "id": 0, "point": 1200 },  <- Top 2
               { "id": 1, "point": 500 }    <- Top 3
           ]
       }
    */

    return j.dump();
}
void HandleStartGame(int clientFD)
{
    WriteLog(LogType::Request, clientFD, "START GAME");

    Map = MapEntity();

    //if (Lobby.CountTeam() < 3)
    //{
    //  WriteLog(LogType::Failure, clientFD, "START GAME : Not enough teams.", "Current team: " + to_string(Lobby.CountTeam()));
    //  SendMessage(clientFD, string(RS_START_GAME_F_NOT_ENOUGH_TEAMS));
    //  
    //  return;
    //}

    /*Load biến lưu trữ toàn bộ câu hỏi*/
    string path = "Datas/Question.ynl"; // relative to project root/executable
    if (!QuestionBank.load_data(path)) {
        WriteLog(LogType::Failure, clientFD, "START GAME : Failed to load question bank", path);
        return;
    } else {
        WriteLog(LogType::Update, clientFD, "Loaded question bank", path);
    }
    for(int i = 0; i<(int)Map.Spots.size(); i++){
        Map.Spots[i].CurrentQuestion.second = QuestionBank.spot_questions[i];
        Map.Spots[i].CurrentQuestion.first = i;
    }


    for(int i =0; i<(int)Map.Castles.size(); i++){
        Map.Castles[i].CurrentQuestion.first = i;
        Map.Castles[i].CurrentQuestion.second =QuestionBank.castle_questions[i];
    }

    Group.CreateTeam();


    WriteLog(LogType::Success, clientFD, "START GAME");
    BroadcastToClient(clientFD, string(RS_UPDATE_GAME_START), true);


    StartTickOnServer(
        [](int tick)
        {
            if (tick % TICK_RESOURCE_UPDATE == 0)
            {
                Group.UpdateResource();


                auto json = Group.SerializeResource();


                WriteLog(LogType::Update, -1, "UPDATE RESOURCE", json);
                BroadcastToClient(-1, string(RS_UPDATE_TEAM_RESOURCE) + " " + json);
            }


            BroadcastToClient(-1, string(RS_UPDATE_GAME_TICK) + " " + to_string(tick));


            if (tick == TIME_START_COMBAT)
            {
                WriteLog(LogType::Update, -1, "START COMBAT");
                BroadcastToClient(-1, string(RS_UPDATE_START_COMBAT));


                GamePhase = PHASE_CASTLE_COMBATING;
            }
            if (tick == TIME_END_GAME)
            {
                WriteLog(LogType::Update, -1, "END GAME");
                string jsonData = HandleEndGame();
                BroadcastToClient(-1, string(RS_UPDATE_END_GAME) + " " + jsonData);
                StopTickOnServer();
                GamePhase = PHASE_LOBBY_IDLING;
            }
        },
        []()
        {
            cout << "Tick end" << endl;
        }
    );
}

void HandleOccupySpot(int clientFD, const string& data)
{
    SpotRequest = SpotOccupationRecord::Deserialize(data);
    WriteLog(LogType::Request, clientFD, "OCCUPY SPOT", SpotRequest.Capture());


    auto account = Accounts[Clients[clientFD]];
    auto& spot = Map.Spots[SpotRequest.Spot];
    auto& slot = spot.Slots[SpotRequest.Type];


    if (slot != -1) /* Nếu đã có người chiếm */
    {
        WriteLog(LogType::Failure, clientFD, "OCCUPY SLOT : Slot occupied", SpotRequest.Capture());
        SendMessage(clientFD, string(RS_OCCUPY_SPOT_F_SPOT_OCCUPIED));
        return;
    }
    auto& team = Group.Teams[account.GameTeam];
    auto freeSlot = team.GetFreeSlot(SpotRequest.Type);
    /* Quy định mỗi team chỉ được phép chiếm 2 slot cho 1 loại tài nguyên*/


    if (freeSlot == -1)
    {
        WriteLog(LogType::Failure, clientFD, "OCCUPY SLOT : Both slots are occupied", SpotRequest.Capture());
        SendMessage(clientFD, string(RS_OCCUPY_SPOT_F_FULL_OF_SLOT));


        return;
    }


    if (slot == -1){
        WriteLog(LogType::Request, clientFD, "OCCUPY SLOT : Answer the question", SpotRequest.Capture());


        WriteLog(LogType::Update, clientFD, "DEBUG", string("[OccupySpot] spot_questions.size=") + to_string(QuestionBank.spot_questions.size()) + string(" request.Spot=") + to_string(SpotRequest.Spot));
        if (SpotRequest.Spot < 0 || SpotRequest.Spot >= (int)Map.Spots.size()) {
            WriteLog(LogType::Failure, clientFD, "OCCUPY SLOT : Invalid spot", to_string(SpotRequest.Spot));
            SendMessage(clientFD, string(RS_OCCUPY_SPOT_F_INVALID_SPOT));
            return;
        }
        TargetSpotQuestion = Map.Spots[SpotRequest.Spot].CurrentQuestion.second;
        json j;
        j["id"] = SpotRequest.Spot;
        j["content"] = TargetSpotQuestion.content;
        j["answers"] = json::array();
        for (const auto& a : TargetSpotQuestion.answers) j["answers"].push_back(a);
        j["difficulty"] = (TargetSpotQuestion.difficulty == Difficulty::EASY) ? string("Easy") : string("Difficult");
        j["correctAnswer"] = TargetSpotQuestion.correctAnswer; // server keeps true answer, client won't use it
        SendMessage(clientFD, string(RS_OCCUPY_REQUIRE_ANSWER_QUESTION) + " " + to_string(SpotRequest.Spot) + " spot " + j.dump());
    }
   
}

void HandleOccupyCastle(int clientFD, const string& data)
{
    CastleRequest = stoi(data);

    WriteLog(LogType::Request, clientFD, "OCCUPY CASTLE", "Castle: " + data);

    auto account = Accounts[Clients[clientFD]];
    auto& castle = Map.Castles[CastleRequest];

    if (castle.OwnerTeam != -1)
    {
        WriteLog(LogType::Failure, clientFD, "OCCUPY CASTLE : Castle occupied", "Castle: " + data);
        SendMessage(clientFD, string(RS_OCCUPY_CASTLE_F_CASTLE_OCCUPIED));
        return;
    }

    // if (team.CastleSlot != -1)
    // {
    //     WriteLog(LogType::Failure, clientFD, "OCCUPY CASTLE : Slot full");
    //     SendMessage(clientFD, string(RS_OCCUPY_CASTLE_F_SLOT_FULL));
    //     return;
    // }
    if (CastleRequest < 0 || CastleRequest >= (int)Map.Castles.size()) {
        WriteLog(LogType::Failure, clientFD, "OCCUPY CASTLE : Invalid castle", to_string(CastleRequest));
        SendMessage(clientFD, string(RS_OCCUPY_CASTLE_F_INVALID_CASTLE));
        return;
    }
    TargetCastleQuestion = Map.Castles[CastleRequest].CurrentQuestion.second;
    json j;
    j["id"] = CastleRequest;
    j["content"] = TargetCastleQuestion.content;
    j["answers"] = json::array();
    for (const auto& a : TargetCastleQuestion.answers) j["answers"].push_back(a);
    j["difficulty"] = (TargetCastleQuestion.difficulty == Difficulty::EASY) ? string("Easy") : string("Difficult");
    j["correctAnswer"] = TargetCastleQuestion.correctAnswer;

    SendMessage(clientFD, string(RS_OCCUPY_REQUIRE_ANSWER_QUESTION) + " " + to_string(CastleRequest) + " castle " + j.dump());
}

void HandleAnswerSpotQuestion(int clientFD, const string& data)
{
    int teamAnswer = -1;
    try { teamAnswer = stoi(data) - 1; } catch(...) { teamAnswer = -1; }
    WriteLog(LogType::Request, clientFD, "OCCUPY SLOT : Answer question", "Answer:" + data);
    int trueAnswer = TargetSpotQuestion.correctAnswer;

    auto account = Accounts[Clients[clientFD]];
    auto& spot = Map.Spots[SpotRequest.Spot];
       
    auto& team = Group.Teams[account.GameTeam];
    auto freeSlot = team.GetFreeSlot(SpotRequest.Type);
   
    int result = (TargetSpotQuestion.content).compare(Map.Spots[SpotRequest.Spot].CurrentQuestion.second.content);  


    if ((result == 0) && (teamAnswer == trueAnswer))
    {
        WriteLog(LogType::Success, clientFD, "OCCUPY SLOT SUCCESS", "");
        SendMessage(clientFD, string(RS_ANSWER_QUESTION_SPOT_S));


        team.SpotSlots[SpotRequest.Type][freeSlot] = SpotRequest.Spot;
        spot.Slots[SpotRequest.Type] = account.GameTeam;


        int currentQuestionID = Map.Spots[SpotRequest.Spot].CurrentQuestion.first;
        Map.Spots[SpotRequest.Spot].CurrentQuestion = ChangeSpotQuestion(currentQuestionID);
        // BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);
    }
    else if ((result == 0) && (teamAnswer != trueAnswer))
    {
        SendMessage(clientFD, string(RS_ANSWER_QUESTION_F_WRONG_ANSWER));
    }
    else if (result != 0)
    {
        SendMessage(clientFD,string(RS_ANSWER_QUESTION_F_TOO_SLOW));
    }
   
    BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);
}

void HandleAnswerCastleQuestion(int clientFD, const string& data)
{
    int teamAnswer = -1;
    try { teamAnswer = stoi(data) - 1; } catch(...) { teamAnswer = -1; }
    WriteLog(LogType::Request, clientFD, "OCCUPY CASTLE : Answer question", "Answer:" + data);
    int trueAnswer = TargetCastleQuestion.correctAnswer;

    auto account = Accounts[Clients[clientFD]];
    auto& castle = Map.Castles[CastleRequest]; 
    auto& team = Group.Teams[account.GameTeam];


    if (teamAnswer == trueAnswer)
    {
        WriteLog(LogType::Success, clientFD, "OCCUPY CASTLE SUCCESS", "");
        SendMessage(clientFD, string(RS_ANSWER_QUESTION_CASTLE_S));

        team.CastleSlot.push_back(CastleRequest);
        castle.OwnerTeam = account.GameTeam;

        int currentQuestionID = Map.Castles[CastleRequest].CurrentQuestion.first;
        Map.Castles[CastleRequest].CurrentQuestion = ChangeSpotQuestion(currentQuestionID);

        BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);

    }
    else
    {
        SendMessage(clientFD, string(RS_ANSWER_QUESTION_F_WRONG_ANSWER));
    }
}

void HandleBuyWeapon(int clientFD, const CartRecord& cart)
{
    /* Equipment & Amount */
    /*1. Balista 2. Catapult 3. Cannon*/
    /* array<int,3> Weapons*/
    auto account = Accounts[Clients[clientFD]];
    auto& team = Group.Teams[account.GameTeam];
    
    Item* item = GetItem((Items)cart.Equipment);
    int compare = ResourceCompare(team,item->Cost,cart.Amount);
    if (compare == 0)
    {
        delete item;
        WriteLog(LogType::Failure, clientFD, "LACK RESOUCRE", "");
        SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_F_LACK_RESOURCE));
    }
    else
    {
        team.Weapons[cart.Equipment] += cart.Amount;
        UpdateResourcesQuantity(team,item->Cost,cart.Amount);
        delete item; 
        WriteLog(LogType::Failure, clientFD, "BUY SUCCESS", "");
        SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_S));
    }
}

void HandleBuyDefense(int clientFD, const CartRecord& cart)
{
    auto account = Accounts[Clients[clientFD]];
    auto& team = Group.Teams[account.GameTeam];
    auto& targetCastle = Map.Castles[cart.TargetCastle];
    /* Items là 1 enum có 3 attack & 4 defense item */
    Item* item = GetItem((Items)(cart.Equipment + 3));

    int compare = ResourceCompare(team,item->Cost,cart.Amount);

    /* Team không sở hữu castle nào nhưng vẫn mua defense*/
    if (team.CastleSlot.size() == 0)
    {
        delete item;
        WriteLog(LogType::Failure, clientFD, "NOT HAVE ANY CASTLE", "");
        SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_F_ZERO_CASTLE));
    }
    /* Team không sở hữu target castle*/
    else if ( (team.CastleSlot.size() >=1) && (targetCastle.OwnerTeam != account.GameTeam))
    {
        delete item;
        WriteLog(LogType::Failure, clientFD, "NOT OWN CASTLE", "");
        SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_F_NOT_OWN_CASTLE));
    } 
    else if (targetCastle.OwnerTeam == account.GameTeam)
    {
        /*Không đủ tài nguyên để mua lượng vật phẩm*/
        if (compare == 0)
        {
            delete item;
            WriteLog(LogType::Failure, clientFD, "LACK RESOUCRE", "");
            SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_F_LACK_RESOURCE));
        }
        /*Đủ tài nguyên để mua*/
        else 
        {
            targetCastle.Defense += item->DefensePoint * cart.Amount;
            for (int i = 0; i < cart.Amount; i++) targetCastle.EquippedItems.push_back(int(item->ItemType));
            UpdateResourcesQuantity(team,item->Cost,cart.Amount);
            delete item;
            WriteLog(LogType::Failure, clientFD, "BUY SUCCESS", "");
            SendMessage(clientFD, string(RS_SHOP_EQUIPMENT_S));

            BroadcastToClient(clientFD, string(RS_UPDATE_GAME_MAP) + " " + Map.Serialize(), true);            
        }
    }

}

pair<int,QuestionEntity> ChangeSpotQuestion(int currentQuestionID){
    int nums_question = QuestionBank.spot_questions.size();
    QuestionEntity returnQuestion;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,nums_question-1);
    while(1){
        int random = dist(gen);
        returnQuestion = QuestionBank.spot_questions[random];
        if(random != currentQuestionID) return make_pair(random,returnQuestion);
    }
}

pair<int,QuestionEntity> ChangeCastleQuestion(int currentQuestionID){
    int nums_question = QuestionBank.castle_questions.size();
    QuestionEntity returnQuestion;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,nums_question-1);
    while(1){
        int random = dist(gen);
        returnQuestion = QuestionBank.castle_questions[random];
        if(random != currentQuestionID) return make_pair(random,returnQuestion);
    }
}

int ResourceCompare(const TeamEntity& team, unordered_map<ResourceType,int> require, int amount){
     /* 0 : Wood, 1 : Rock, 2 : Iron, 3 : Gold*/
    for (const auto& req : require)
    {
        int requiredAmount = req.second * amount;
        int teamQuantity = team.Resources[int(
            
        )];
        if (teamQuantity < requiredAmount) return false;
    }
    return true;
}

void UpdateResourcesQuantity(TeamEntity& team, unordered_map<ResourceType,int> cost, int amount){
    for (const auto& c : cost)
    {
        int requiredAmount = c.second;
        team.Resources[int(c.first)] -= requiredAmount * amount;
    }
}

void ResetGame()
{
    Group = GroupEntity();
    Map = MapEntity();
    QuestionBank = QuestionBankEntity();
    GamePhase = PHASE_LOBBY_IDLING;
    BroadcastToClient(-1, string(RS_RESET_GAME), true);
    BroadcastToClient(-1, string(RS_UPDATE_ROOM_LIST) + " " + Lobby.Serialize(), true);
}

#endif

