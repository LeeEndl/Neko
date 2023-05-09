#pragma once

namespace command {
	map<string, dpp::snowflake> name_to_id;
}

struct tools { string name = ""; int durability = 0; };
struct UserData {
	JINT daily = time(0), last_on = 0, fish = 0, user_id = 0, dollars = 0;
	JSTRING username = "";
	vector<tools> tools;

	bool busy_fishing = false, once_fishing = false;
	bool busy_hunting = false, once_hunting = false;
	time_t last_fish = 0, last_hunt = 0;
	bool failed = false;
}; map<dpp::snowflake, UserData> members;
struct GuildData {
	JBOOLEAN joined = false;
	bool failed = false;

	string prefix = "";
};
inline UserData GetUserData(dpp::snowflake user_id)
{
	UserData data;
	if (not ifstream("database/users/" + to_string(user_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	ifstream("database/users/" + to_string(user_id) + ".txt") >> J;
	data.daily = ELEMENT_JI("daily")
		data.dollars = ELEMENT_JI("dollars")
		data.fish = ELEMENT_JI("fish")
		data.last_on = ELEMENT_JI("last_on")
		data.username = ELEMENT_JS("username")
		data.user_id = ELEMENT_JI("user_id")
		json array = J["tools"].get<json>();
	if (array.empty()) {
		tools buf;
		buf.name = "", buf.durability = 0;
		data.tools.emplace_back(buf);
	}
	tools buf;
	for (int i = 0; i < array.size(); i++)
		buf.name = not (array[i].find("name") not_eq array[i].end()) ? "" : array[i]["name"].get<string>(),
		buf.durability = not (array[i].find("durability") not_eq array[i].end()) ? 0 : array[i]["durability"].get<int>(),
		data.tools.emplace_back(buf);
	J = json();
	return data;
}
inline void SaveUserData(UserData data, dpp::snowflake user)
{
	J.dump(1);
	J["daily"] = data.daily;
	J["dollars"] = data.dollars;
	J["fish"] = data.fish;
	J["last_on"] = data.last_on;
	J["username"] = data.username;
	J["user_id"] = data.user_id;
	json array = json::array();
	for (int i_ = 0; i_ < data.tools.size(); i_++) {
		json j;
		j["name"] = data.tools[i_].name;
		j["durability"] = data.tools[i_].durability;
		array.emplace_back(j);
	}
	J["tools"] = array;
	ofstream("database/users/" + to_string(user) + ".txt") << setw(2) << J;
	J = json();
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
	ifstream("database/guilds/" + to_string(guild_id) + ".txt") >> J;
	data.joined = ELEMENT_JB("joined");
	data.prefix = ELEMENT_JS("prefix");
	J = json();
	return data;
}
inline void SaveGuildData(GuildData data, dpp::snowflake guild_id)
{
	J.dump(1);
	J["joined"] = data.joined;
	J["prefix"] = data.prefix;
	ofstream("database/guilds/" + to_string(guild_id) + ".txt") << setw(2) << J;
	J = json();
}
inline void new_guild(dpp::snowflake guild_id)
{
	GuildData data = GetGuildData(guild_id);
	data.joined = false;
	data.prefix = "!";
	data.failed = false;
	SaveGuildData(data, guild_id);
}

inline void await_on_guild_create(const dpp::guild_create_t& event) {
	GuildData data = GetGuildData(event.created->id);
	if (static_cast<bool>(data.failed)) new_guild(event.created->id);
}

inline void wrap_database() {
	getline(ifstream("token"), bot.token);
	if (not filesystem::exists("database")) filesystem::create_directory("database");
	if (not filesystem::exists("./database/guilds")) filesystem::create_directory("./database/guilds");
	if (not filesystem::exists("./database/users")) filesystem::create_directory("./database/users");

	for (const auto& i : filesystem::directory_iterator("database/users")) {
		vector<string> index = dpp::index(i.path().filename().string(), '.');
		members.emplace(static_cast<dpp::snowflake>(stoull(index[0])), GetUserData(stoull(index[0])));
	}
}

enum passive { none, poison };
class passives {
public:
	string emoji = "";
	passive passive = passive::none;
	int turns = 0;
};
class stats {
public:
	int ATK = 0;
	int DEF = 0;
	int SPD = 0;
	map<passive, passives> passive;
};
map<string, stats> Stat = {
	{":worm:", stats{0, 9, 0}},
	{":lady_beetle:", stats{1, 3, 1}},
	{":rat:", stats{1, 2, 3}},
	{":butterfly:", stats{1, 0, 4}},
	{":bee:", stats{1, 3, 4}},

	{":knife:", stats{1, 0, 5}}
};

auto Beg = chrono::high_resolution_clock::now(), End = chrono::high_resolution_clock::now();