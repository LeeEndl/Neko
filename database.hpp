#pragma once

class tools {
public:
	string name = "";
	int durability = 0;
};
class UserData {
public:
	time_t daily = time(0), last_on = 0, last_fish = 0, last_hunt = 0;
	uint64_t user_id = 0; string username = "";
	bool failed = false;
	vector<tools> tools;
	int fish = 0;
	uint64_t dollars = 0;

	short ratelimit = 0, queue = 0;
	bool once_ratelimit = 0;
	bool busy_fishing = false, once_fishing = false;
	bool busy_hunting = false, once_hunting = false;
}; map<dpp::snowflake, UserData> members;
class GuildData {
public:
	bool failed = false;
	bool joined = false;
	string prefix = "";
}; map<dpp::snowflake, GuildData> guilds;
inline UserData GetUserData(dpp::snowflake user_id)
{
	UserData data;
	if (not ifstream("database/users/" + to_string(user_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	json j;
	ifstream("database/users/" + to_string(user_id) + ".txt") >> j;
	data.daily = (not (j.find("daily") not_eq j.end()) ? static_cast<long long>(0) : j["daily"].get<long long>());
	data.dollars = (not (j.find("dollars") not_eq j.end()) ? static_cast<uint64_t>(0) : j["dollars"].get<uint64_t>());
	data.fish = (not (j.find("fish") not_eq j.end()) ? static_cast<int>(0) : j["fish"].get<int>());
	data.last_on = (not (j.find("last_on") not_eq j.end()) ? static_cast<long long>(0) : j["last_on"].get<long long>());
	data.username = (not (j.find("username") not_eq j.end()) ? static_cast<string>("") : j["username"].get<string>());
	data.user_id = (not (j.find("user_id") not_eq j.end()) ? static_cast<uint64_t>(0) : j["user_id"].get<uint64_t>());
	json array = j["tools"].get<json>();
	if (array.empty()) {
		tools buf;
		buf.name = "", buf.durability = 0;
		data.tools.emplace_back(buf);
	}
	tools buf;
	for (int i_ = 0; i_ < array.size(); i_++)
		buf.name = (not (array[i_].find("name") not_eq array[i_].end()) ? static_cast<string>("") : array[i_]["name"].get<string>()),
		buf.durability = (not (array[i_].find("durability") not_eq array[i_].end()) ? static_cast<int>(0) : array[i_]["durability"].get<int>()),
		data.tools.emplace_back(buf);
	return data;
}
inline void SaveUserData(UserData data, dpp::snowflake user)
{
	json j;
	j.dump(1);
	j["daily"] = data.daily;
	j["dollars"] = data.dollars;
	j["fish"] = data.fish;
	j["last_fish"] = data.last_fish;
	j["last_on"] = data.last_on;
	j["username"] = data.username;
	j["user_id"] = data.user_id;
	json array = json::array();
	for (int i_ = 0; i_ < data.tools.size(); i_++) {
		json j;
		j["name"] = data.tools[i_].name;
		j["durability"] = data.tools[i_].durability;
		array.emplace_back(j);
	}
	j["tools"] = array;
	ofstream("database/users/" + to_string(user) + ".txt") << setw(2) << j;
}
inline void new_user(dpp::snowflake user_id)
{
	UserData data = GetUserData(user_id);
	data.username = bot.user_get_sync(user_id).username;
	data.daily = 0;
	data.dollars = 0;
	data.fish = 0, data.last_fish = 0;
	data.last_on = 0;
	data.user_id = bot.user_get_sync(user_id).id;
	data.failed = false;
	SaveUserData(data, user_id); {
		UserData data = GetUserData(user_id);
		members.emplace(bot.user_get_sync(user_id).id, data);
	}
}
inline GuildData GetGuildData(dpp::snowflake guild_id)
{
	GuildData data;
	if (not ifstream("database/guilds/" + to_string(guild_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	json j;
	ifstream("database/guilds/" + to_string(guild_id) + ".txt") >> j;
	data.joined = (not (j.find("joined") not_eq j.end()) ? static_cast<bool>(false) : j["joined"].get<bool>());
	data.prefix = (not (j.find("prefix") not_eq j.end()) ? static_cast<string>("") : j["prefix"].get<string>());
	return data;
}
inline void SaveGuildData(GuildData data, dpp::snowflake guild_id)
{
	json j;
	j.dump(1);
	j["joined"] = data.joined;
	j["prefix"] = data.prefix;
	ofstream("database/guilds/" + to_string(guild_id) + ".txt") << setw(2) << j;
}
inline void new_guild(dpp::snowflake guild_id)
{
	GuildData data = GetGuildData(guild_id);
	data.joined = false;
	data.prefix = "!";
	data.failed = false;
	SaveGuildData(data, guild_id); {
		GuildData data = GetGuildData(guild_id);
		guilds.emplace(guild_id, data);
	}
}
inline void await_on_guild_create(const dpp::guild_create_t& event) {
	GuildData data = GetGuildData(event.created->id);
	if (static_cast<bool>(data.failed)) new_guild(event.created->id);
}
inline void await_on_guild_delete(const dpp::guild_delete_t& event) {
	for (const auto& i : filesystem::directory_iterator("database/guilds")) {
		vector<string> index = dpp::index(i.path().filename().string(), '.');
		guilds.erase(static_cast<dpp::snowflake>(stoull(index[0])));
		string FileName = "database/guilds/" + i.path().filename().string();
		remove(FileName.c_str());
	}
}

inline void wrap_database() {
	if (not filesystem::exists("database")) filesystem::create_directory("database");
	if (not filesystem::exists("./database/guilds")) filesystem::create_directory("./database/guilds");
	if (not filesystem::exists("./database/users")) filesystem::create_directory("./database/users");

	for (const auto& i : filesystem::directory_iterator("database/users")) {
		vector<string> index = dpp::index(i.path().filename().string(), '.');
		members.emplace(static_cast<dpp::snowflake>(stoull(index[0])), GetUserData(stoull(index[0])));
	}
	for (const auto& i : filesystem::directory_iterator("database/guilds")) {
		vector<string> index = dpp::index(i.path().filename().string(), '.');
		guilds.emplace(static_cast<dpp::snowflake>(stoull(index[0])), GetGuildData(stoull(index[0])));
	}
}