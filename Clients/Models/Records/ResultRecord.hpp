#ifndef SERVER_MODEL_RECORD_RESULT
#define SERVER_MODEL_RECORD_RESULT

struct ResultRecord
{
    vector<pair<int,int>> Results; // <teamId, point>

    static ResultRecord Deserialize(const string& data)
    {
        ResultRecord record;

        json j = json::parse(data);

        for (const auto& teamResult : j.at("result"))
        {
            int id    = teamResult.at("id").get<int>();
            int point = teamResult.at("point").get<int>();

            record.Results.emplace_back(id, point);
        }

        return record;
    }
};


#endif