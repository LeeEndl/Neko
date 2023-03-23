#pragma once

class UserData
{
public:
	time_t daily = std::time(0), last_on = 0, last_fish = std::time(0);
	uint64_t user_id = 0;
	bool failed = false;
	string rod = "", username = "";
	int rod_d = 0, fish = 0;
	uint64_t dollars = 0;

	bool busy_fishing = false, once_fishing = false;
}; map<dpp::snowflake, UserData> members;
inline UserData GetUserData(dpp::snowflake user_id)
{
	UserData data;
	if (not ifstream("database/user/" + to_string(user_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	json j;
	ifstream("database/user/" + to_string(user_id) + ".txt") >> j;
	data.daily = (not (j.find("daily") not_eq j.end()) ? static_cast<long long>(0) : j["daily"].get<long long>());
	data.dollars = (not (j.find("dollars") not_eq j.end()) ? static_cast<uint64_t>(0) : j["dollars"].get<uint64_t>());
	data.rod = (not (j.find("rod") not_eq j.end()) ? static_cast<string>("") : j["rod"].get<string>());
	data.rod_d = (not (j.find("rod_d") not_eq j.end()) ? static_cast<int>(0) : j["rod_d"].get<int>());
	data.fish = (not (j.find("fish") not_eq j.end()) ? static_cast<int>(0) : j["fish"].get<int>());
	data.last_fish = (not (j.find("last_fish") not_eq j.end()) ? static_cast<long long>(0) : j["last_fish"].get<long long>());
	data.last_on = (not (j.find("last_on") not_eq j.end()) ? static_cast<long long>(0) : j["last_on"].get<long long>());
	data.username = (not (j.find("username") not_eq j.end()) ? static_cast<string>("") : j["username"].get<string>());
	data.user_id = (not (j.find("user_id") not_eq j.end()) ? static_cast<uint64_t>(0) : j["user_id"].get<uint64_t>());
	return data;
}
inline void SaveUserData(UserData data, dpp::snowflake user)
{
	json j;
	j.dump(1);
	j["daily"] = data.daily;
	j["dollars"] = data.dollars;
	j["rod"] = data.rod;
	j["rod_d"] = data.rod_d;
	j["fish"] = data.fish;
	j["last_fish"] = data.last_fish;
	j["last_on"] = data.last_on;
	j["username"] = data.username;
	j["user_id"] = data.user_id;
	ofstream("database/user/" + to_string(user) + ".txt") << setw(2) << j;
}
inline void new_user(dpp::snowflake user_id)
{
	UserData data = GetUserData(user_id);
	ofstream("maps/members.txt", ios::app) << user_id << endl;
	data.username = bot.user_get_sync(user_id).username;
	data.daily = 0;
	data.dollars = 0;
	data.rod = "", data.rod_d = 0;
	data.fish = 0, data.last_fish = 0;
	data.last_on = 0;
	data.user_id = bot.user_get_sync(user_id).id;
	data.failed = false;
	SaveUserData(data, user_id); {
		UserData data = GetUserData(user_id);
		members.emplace(bot.user_get_sync(user_id).id, data);
	}
}
inline void StructUserMap()
{
	string line;
	ifstream r("maps/members.txt");
	while (getline(r, line)) {
		members.emplace(
			static_cast<dpp::snowflake>(line),
			GetUserData(stoull(line)));
	}
}