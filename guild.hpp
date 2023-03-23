#pragma once
using namespace std;
using namespace this_thread;

class GuildData {
public:
	bool failed = false, joined = false;
	string prefix = "";
}; map<dpp::snowflake, GuildData> guilds;
inline GuildData GetGuildData(dpp::snowflake guild_id)
{
	GuildData data;
	if (not ifstream("database/guild/" + to_string(guild_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	json j;
	ifstream("database/guild/" + to_string(guild_id) + ".txt") >> j;
	data.joined = (not (j.find("joined") not_eq j.end()) ? static_cast<bool>(false) : j["joined"].get<bool>());
	data.prefix = (not (j.find("prefix") not_eq j.end()) ? static_cast<string>("") : j["prefix"].get<string>());
	return data;
}
inline void SaveGuildData(GuildData data, dpp::snowflake guild_id)
{
	json j;
	j.dump(1)
	j["joined"] = data.joined;
	j["prefix"] = data.prefix;
	ofstream("database/guild/" + to_string(guild_id) + ".txt") << setw(2) << j;
}
inline void new_guild(dpp::snowflake guild_id)
{
	GuildData data = GetGuildData(guild_id);
	ofstream("maps/guilds.txt", ios::app) << guild_id << endl;
	data.joined = false;
	data.prefix = "!";
	data.failed = false;
	SaveGuildData(data, guild_id); {
		GuildData data = GetGuildData(guild_id);
		guilds.emplace(guild_id, data);
	}
}
inline void StructGuildMap()
{
	string line;
	ifstream r("maps/guilds.txt");
	while (getline(r, line)) {
		guilds.emplace(
			static_cast<dpp::snowflake>(line),
			GetGuildData(stoull(line)));
	}
}
vector<thread> guild_create_executed;
inline void await_on_guild_create(const dpp::guild_create_t& event) {
	GuildData data = GetGuildData(event.created->id);
	if (static_cast<bool>(data.failed)) new_guild(event.created->id);
}
vector<thread> guild_delete_executed;
inline void await_on_guild_delete(const dpp::guild_delete_t& event) {
	ifstream r("maps/guilds.txt");
	ofstream w("maps/guilds.txt");
	string line;
	guilds.erase(event.deleted->id);
	while (getline(ifstream("maps/guilds.txt"), line)) {
		if (line not_eq to_string(event.deleted->id)) {
			ofstream("maps/guilds.txt") << line << endl;
		}
	}
}