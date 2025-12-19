#ifndef SERVER_MODEL_RECORD_RESOURCE
#define SERVER_MODEL_RECORD_RESOURCE

struct ResourceRecord
{
	vector<array<int, 4>> Resources;

	static ResourceRecord Deserialize(const string& data)
	{
        ResourceRecord record;

        json j = json::parse(data);

        for (const auto& teamResource : j)
        {
            record.Resources.push_back(teamResource.get<array<int, 4>>());
        }

        return record;
	}
};

#endif