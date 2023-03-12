#pragma once
#include <ctime>
#include <random>
#include <filesystem>
using namespace std;
using namespace this_thread;
namespace fs = std::filesystem;
dpp::cluster bot(
	static_cast<string>("MTAwNDUxNDkzNTA1OTAwNTQ3MA.GCLzi3.oyFg9LNfawz4yT2t5ZyH87UbAx4iM9mAO_Ou2w"),
	static_cast<uint32_t>(dpp::i_default_intents | dpp::i_message_content)
);
map<int, thread> tasks;
class randomx {
public:
	static int Int(int min, int max) { random_device picker; uniform_int_distribution<int> numbers(min, max); return numbers(picker); }
	static long Long(long min, long max) { random_device picker; uniform_int_distribution<long> numbers(min, max); return numbers(picker); }
};
template <class T1, class T2, class Pred = greater<T1> >
struct first {
	bool operator()(const pair<T1, T2>& left, const pair<T1, T2>& right) {
		Pred p;
		return p(left.first, right.first);
	}
};
template <class T1, class T2, class Pred = greater<T2> >
struct second {
	bool operator()(const pair<T1, T2>& left, const pair<T1, T2>& right) {
		Pred p;
		return p(left.second, right.second);
	}
};
bool number(string str) {
	return any_of(str.begin(), str.end(), ::isdigit);
}
namespace memory
{
	inline tm* mold_time(time_t t)
	{
		long long _time = t;
		tm* types = localtime(&_time);
		return types;
	}
	class time_
	{
	public:
		tm* track_time;
		bool pm = false, am = false;
		string hour, min, sec, wday, mday, month_num, month, year, century = "20", suffix;
		string time, dropsec;
	}; time_ time;
	time_t mtt(tm* tm) {
		time_t ftm = mktime(memory::time.track_time);
		return ftm;
	}
	class manipulate {
	public:
		time_ t;
		time_t dynamic = std::time(0);
		manipulate() {
			t.track_time = mold_time(std::time(0));
			t.pm = time.pm;
			t.am = time.am;
			t.hour = time.hour;
			t.min = time.min;
			t.sec = time.sec;
			t.wday = time.mday;
			t.month_num = time.month;
			t.century = time.century;
			t.suffix = time.suffix;
			t.time = time.time;
			t.dropsec = time.dropsec;
			t.time = time.time;
		}
		string subtime(time_ t1, time_ t2)
		{
			string hour = "", min = "", sec = "";
			if (stoi(t1.hour) < stoi(t2.hour)) hour = to_string(stoi(t2.hour) - stoi(t1.hour));
			else hour = to_string(stoi(t1.hour) - stoi(t2.hour));
			if (stoi(t1.min) < stoi(t2.min)) min = to_string(stoi(t2.min) - stoi(t1.min));
			else min = to_string(stoi(t1.min) - stoi(t2.min));
			if (stoi(t1.sec) < stoi(t2.sec)) sec = to_string(stoi(t2.sec) - stoi(t1.sec));
			else sec = to_string(stoi(t1.sec) - stoi(t2.sec));
			return (hour == "0" ? "" : hour + ":") + (min == "0" ? "" : min + ":") + (sec);
		}
	};
	time_t change_time(tm* old_time, int second, int minute, int hour, int wday, int mday, int month_num) {
		old_time->tm_sec = second;
		old_time->tm_min = minute;
		old_time->tm_hour = hour;
		old_time->tm_wday = wday;
		old_time->tm_mday = mday;
		old_time->tm_mon = month_num;
		return mktime(old_time);
	}
	inline thread update_time()
	{
		while (true)
		{
			time.track_time = mold_time(std::time(0));
			if (time.track_time->tm_hour == 13) time.hour = "1", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 14) time.hour = "2", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 15) time.hour = "3", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 16) time.hour = "4", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 17) time.hour = "5", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 18) time.hour = "6", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 19) time.hour = "7", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 20) time.hour = "8", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 21) time.hour = "9", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 22) time.hour = "10", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 23) time.hour = "11", time.pm = true, time.am = false;
			if (time.track_time->tm_hour == 0) time.hour = "12", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 1) time.hour = "1", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 2) time.hour = "2", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 3) time.hour = "3", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 4) time.hour = "4", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 5) time.hour = "5", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 6) time.hour = "6", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 7) time.hour = "7", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 8) time.hour = "8", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 9) time.hour = "9", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 10) time.hour = "10", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 11) time.hour = "11", time.am = true, time.pm = false;
			if (time.track_time->tm_hour == 12) time.hour = "12", time.am = true, time.pm = false;
			if (time.track_time->tm_min < 10) time.min = "0" + to_string(time.track_time->tm_min);
			else time.min = to_string(time.track_time->tm_min);
			if (time.track_time->tm_sec < 10) time.sec = "0" + to_string(time.track_time->tm_sec);
			else time.sec = to_string(time.track_time->tm_sec);
			time.mday = to_string(time.track_time->tm_mday);
			if (time.track_time->tm_mday == 1 or time.track_time->tm_mday == 21 or time.track_time->tm_mday == 31) time.suffix = "st";
			else if (time.track_time->tm_mday == 2 or time.track_time->tm_mday == 22) time.suffix = "nd";
			else if (time.track_time->tm_mday == 3 or time.track_time->tm_mday == 23) time.suffix = "rd";
			else time.suffix = "th";
			time.year = to_string(time.track_time->tm_year);
			time.year.erase(time.year.begin());
			if (time.track_time->tm_wday == 0) time.wday = "Sunday";
			if (time.track_time->tm_wday == 1) time.wday = "Monday";
			if (time.track_time->tm_wday == 2) time.wday = "Tuesday";
			if (time.track_time->tm_wday == 3) time.wday = "Wednesday";
			if (time.track_time->tm_wday == 4) time.wday = "Thursday";
			if (time.track_time->tm_wday == 5) time.wday = "Friday";
			if (time.track_time->tm_wday == 6) time.wday = "Saturday";
			time.month_num = to_string(time.track_time->tm_mon + 1);
			if (time.track_time->tm_mon == 0) time.month = "January";
			if (time.track_time->tm_mon == 1) time.month = "February";
			if (time.track_time->tm_mon == 2) time.month = "March";
			if (time.track_time->tm_mon == 3) time.month = "April";
			if (time.track_time->tm_mon == 4) time.month = "May";
			if (time.track_time->tm_mon == 5) time.month = "June";
			if (time.track_time->tm_mon == 6) time.month = "July";
			if (time.track_time->tm_mon == 7) time.month = "August";
			if (time.track_time->tm_mon == 8) time.month = "September";
			if (time.track_time->tm_mon == 9) time.month = "October";
			if (time.track_time->tm_mon == 10) time.month = "November";
			if (time.track_time->tm_mon == 11) time.month = "December";
			time.time = time.hour + ":" + time.min + ":" + time.sec + " " + (time.pm ? "PM" : "") + (time.am ? "AM" : "");
			time.dropsec = time.hour + ":" + time.min + " " + (time.pm ? "PM" : "") + (time.am ? "AM" : "");
			sleep_for(static_cast<chrono::milliseconds>(400ms));
		}
		return thread();
	}
	class timestamp {
	public:
		static string short_t(time_t t)
		{
			return "<t:" + to_string(t) + ":t>";
		}
		static string long_t(time_t t)
		{
			return "<t:" + to_string(t) + ":T>";
		}
		static string short_d(time_t t)
		{
			return "<t:" + to_string(t) + ":d>";
		}
		static string long_d(time_t t)
		{
			return "<t:" + to_string(t) + ":D>";
		}
		static string long_d_short_t(time_t t)
		{
			return "<t:" + to_string(t) + ":f>";
		}
		static string full(time_t t)
		{
			return "<t:" + to_string(t) + ":F>";
		}
		static string relevant(time_t t)
		{
			return "<t:" + to_string(t) + ":R>";
		}
	};
	vector<string> explode(string source, const char& find)
	{
		string This = "";
		vector<string> i;
		for (auto c : source)
			if (c not_eq find) This += c;
			else if (c == find and This not_eq "") i.push_back(This), This = "";
		if (This not_eq "") i.push_back(This);
		return i;
	}
	class TempData {
	public:
		bool busy_fishing = false, once_fishing = false;
	};
	class UserData
	{
	public:
		time_t daily = std::time(0), last_on = 0, last_fish = std::time(0);
		uint64_t user_id = 0;
		bool failed = false;
		uint64_t repair = 0;
		string fishing = "", rod = "", username = "";
		int rod_d = 0, fish = 0;
		uint64_t dollars = 0;
	};
	map<dpp::snowflake, UserData> members;
	map<uint64_t, UserData> leaderboard;
	class GuildData {
	public:
		bool failed = false, joined = false;
		string prefix = "";
	}; map<dpp::snowflake, GuildData> guilds;
	inline TempData GetTempData(dpp::snowflake user_id) {
		TempData data;
		json j; ifstream r("temp/" + to_string(user_id) + ".txt");
		if (not r.is_open()) {
			data.busy_fishing = false;
			return data;
		}
		r >> j;
		data.busy_fishing = (not (j.find("busy_fishing") not_eq j.end()) ? static_cast<bool>(false) : j["busy_fishing"].get<bool>());
		data.once_fishing = (not (j.find("once_fishing") not_eq j.end()) ? static_cast<bool>(false) : j["once_fishing"].get<bool>());
		return data;
	}
	inline void SaveTempData(TempData data, dpp::snowflake user_id)
	{
		ofstream w("temp/" + to_string(user_id) + ".txt");
		json j;
		j.dump(1);
		j["busy_fishing"] = data.busy_fishing, j["once_fishing"] = data.once_fishing;
		w << setw(2) << j;
	}
	inline UserData GetUserData(dpp::user_identified user)
	{
		UserData data;
		json j; ifstream r("database/user/" + to_string(user.id) + ".txt");
		if (not r.is_open()) {
			data.failed = true;
			return data;
		}
		r >> j;
		data.daily = (not (j.find("daily") not_eq j.end()) ? static_cast<long long>(0) : j["daily"].get<long long>());
		data.fishing = (not (j.find("fishing") not_eq j.end()) ? static_cast<string>("") : j["fishing"].get<string>());
		data.dollars = (not (j.find("dollars") not_eq j.end()) ? static_cast<uint64_t>(0) : j["dollars"].get<uint64_t>());
		data.rod = (not (j.find("rod") not_eq j.end()) ? static_cast<string>("") : j["rod"].get<string>());
		data.rod_d = (not (j.find("rod_d") not_eq j.end()) ? static_cast<int>(0) : j["rod_d"].get<int>());
		data.fish = (not (j.find("fish") not_eq j.end()) ? static_cast<int>(0) : j["fish"].get<int>());
		data.last_fish = (not (j.find("last_fish") not_eq j.end()) ? static_cast<long long>(0) : j["last_fish"].get<long long>());
		data.last_on = (not (j.find("last_on") not_eq j.end()) ? static_cast<long long>(0) : j["last_on"].get<long long>());
		data.repair = (not (j.find("repair") not_eq j.end()) ? static_cast<uint64_t>(0) : j["repair"].get<uint64_t>());
		data.username = (not (j.find("username") not_eq j.end()) ? static_cast<string>("") : j["username"].get<string>());
		data.user_id = (not (j.find("user_id") not_eq j.end()) ? static_cast<uint64_t>(0) : j["user_id"].get<uint64_t>());
		return data;
	}
	inline UserData FindUser(dpp::snowflake user_id)
	{
		UserData data;
		json j; ifstream r("database/user/" + to_string(user_id) + ".txt");
		if (not r.is_open()) {
			data.failed = true;
			return data;
		}
		r >> j;
		data.daily = (not (j.find("daily") not_eq j.end()) ? static_cast<long long>(0) : j["daily"].get<long long>());
		data.fishing = (not (j.find("fishing") not_eq j.end()) ? static_cast<string>("") : j["fishing"].get<string>());
		data.dollars = (not (j.find("dollars") not_eq j.end()) ? static_cast<uint64_t>(0) : j["dollars"].get<uint64_t>());
		data.rod = (not (j.find("rod") not_eq j.end()) ? static_cast<string>("") : j["rod"].get<string>());
		data.rod_d = (not (j.find("rod_d") not_eq j.end()) ? static_cast<int>(0) : j["rod_d"].get<int>());
		data.fish = (not (j.find("fish") not_eq j.end()) ? static_cast<int>(0) : j["fish"].get<int>());
		data.last_fish = (not (j.find("last_fish") not_eq j.end()) ? static_cast<long long>(0) : j["last_fish"].get<long long>());
		data.last_on = (not (j.find("last_on") not_eq j.end()) ? static_cast<long long>(0) : j["last_on"].get<long long>());
		data.repair = (not (j.find("repair") not_eq j.end()) ? static_cast<uint64_t>(0) : j["repair"].get<uint64_t>());
		data.username = (not (j.find("username") not_eq j.end()) ? static_cast<string>("") : j["username"].get<string>());
		data.user_id = (not (j.find("user_id") not_eq j.end()) ? static_cast<uint64_t>(0) : j["user_id"].get<uint64_t>());
		return data;
	}
	inline GuildData GetGuildData(dpp::snowflake guild_id)
	{
		GuildData data;
		json j; ifstream r("database/guild/" + to_string(guild_id) + ".txt");
		if (not r.is_open()) {
			data.failed = true;
			return data;
		}
		r >> j;
		data.joined = (not (j.find("joined") not_eq j.end()) ? static_cast<bool>(false) : j["joined"].get<bool>());
		data.prefix = (not (j.find("prefix") not_eq j.end()) ? static_cast<string>("") : j["prefix"].get<string>());
		return data;
	}
	inline void SaveGuildData(GuildData data, dpp::snowflake guild_id)
	{
		ofstream w("database/guild/" + to_string(guild_id) + ".txt");
		json j;
		j.dump(1);
		j["joined"] = data.joined;
		j["prefix"] = data.prefix;
		w << setw(2) << j;
	}
	inline void SaveUserData(UserData data, dpp::user_identified user)
	{
		ofstream w("database/user/" + to_string(user.id) + ".txt");
		json j;
		j.dump(1);
		j["daily"] = data.daily;
		j["fishing"] = data.fishing;
		j["dollars"] = data.dollars;
		j["rod"] = data.rod;
		j["rod_d"] = data.rod_d;
		j["fish"] = data.fish;
		j["last_fish"] = data.last_fish;
		j["last_on"] = data.last_on;
		j["repair"] = data.repair;
		j["username"] = data.username;
		j["user_id"] = data.user_id;
		w << setw(2) << j;
	}
	inline void SaveFoundUser(UserData data, dpp::snowflake user)
	{
		ofstream w("database/user/" + to_string(user) + ".txt");
		json j;
		j.dump(1);
		j["daily"] = data.daily;
		j["fishing"] = data.fishing;
		j["dollars"] = data.dollars;
		j["rod"] = data.rod;
		j["rod_d"] = data.rod_d;
		j["fish"] = data.fish;
		j["last_fish"] = data.last_fish;
		j["last_on"] = data.last_on;
		j["repair"] = data.repair;
		j["username"] = data.username;
		j["user_id"] = data.user_id;
		w << setw(2) << j;
	}
	inline void new_user(dpp::user_identified user)
	{
		memory::UserData data = memory::GetUserData(user);
		ofstream w("maps/members.txt", ios::app);
		w << user.id << '\n';
		data.username = user.username;
		data.daily = 0;
		data.dollars = 0;
		data.rod = "", data.rod_d = 0;
		data.fishing = "0", data.fish = 0, data.last_fish = 0;
		data.last_on = 0;
		data.repair = 0;
		data.user_id = user.id;
		data.failed = false;
		memory::SaveUserData(data, user);
	}
	inline void new_guild(dpp::snowflake guild_id)
	{
		memory::GuildData data = memory::GetGuildData(guild_id);
		ofstream w("maps/guilds.txt", ios::app);
		w << guild_id << '\n';
		data.joined = false;
		data.prefix = "!";
		data.failed = false;
		memory::SaveGuildData(data, guild_id);
	}
	inline void delete_guild(dpp::snowflake guild_id) {
		ifstream r("maps/guilds.txt");
		ofstream w("maps/guilds.txt");
		string line;
		guilds.erase(guild_id);
		while (getline(r, line)) {
			if (line not_eq to_string(guild_id)) {
				w << line << '\n';
			}
		}
	}
	inline thread StructUserMap()
	{
		while (true) {
			ifstream r("maps/members.txt");
			string line;
			while (getline(r, line)) {
				members.erase(static_cast<dpp::snowflake>(line));
				members.emplace(
					static_cast<dpp::snowflake>(line),
					FindUser(stoull(line)));
			}
			sleep_for(10s);
		}
		return thread();
	}
	inline thread StructGuildMap()
	{
		while (true) {
			ifstream r("maps/guilds.txt");
			string line;
			while (getline(r, line)) {
				guilds.erase(static_cast<dpp::snowflake>(line));
				guilds.emplace(
					static_cast<dpp::snowflake>(line),
					GetGuildData(stoull(line)));
			}
			sleep_for(10s);
		}
		return thread();
	}
	inline thread StructLeaderboardMap()
	{
		while (true) {
			ifstream r("maps/leaderboard.txt");
			vector<pair<uint64_t, dpp::snowflake>> buffer;
			string line;
			while (getline(r, line)) {
				auto i = explode(line, ' ');
				buffer.emplace_back(stoull(i[1]), stoull(i[0]));
			}
			sort(buffer.begin(), buffer.end(), first<uint64_t, uint64_t>());
			ofstream w("maps/leaderboard.txt");
			for (auto& i : buffer) {
				leaderboard.erase(i.second);
				leaderboard.emplace(i.first, FindUser(i.second));
				w << " " << i.second << " " << i.first << endl;
			}
			sleep_for(10s);
		}
		return thread();
	}
	inline void fishing(const dpp::slashcommand_t& event)
	{
		{
			TempData data = GetTempData(event.command.member.user_id);
			data.busy_fishing = true;
			SaveTempData(data, event.command.member.user_id);
		}
		{
			UserData data = GetUserData(bot.user_get_sync(event.command.member.user_id));
			if (data.rod not_eq "-1" and data.rod_d == 0)
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("Your fishing rod broke! **/repair 1**");
				event.reply(dpp::message(event.command.channel_id, embed));
				data.rod = "-1";
			}
			SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
		}
		UserData data = GetUserData(bot.user_get_sync(event.command.member.user_id));
		if (data.rod_d not_eq 0)
		{
			dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("Waiting for the fish to bite...");
			dpp::message msg = dpp::message();
			msg.channel_id = event.command.channel_id;
			msg.add_embed(embed);
			event.reply(msg);
			async(Sleep, randomx::Int(6000, 11000));
			for (dpp::embed& e : msg.embeds) e.description = "You caught 1 :fish:!";
			event.edit_response(msg);
			data.rod_d -= 1;
			data.fish += 1;
			data.last_fish = mtt(time.track_time);
			SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
		}
		{
			TempData data = GetTempData(event.command.member.user_id);
			data.busy_fishing = false;
			data.once_fishing = false;
			SaveTempData(data, event.command.member.user_id);
		}
	}
	inline void mass_delete(vector<dpp::snowflake> oids, dpp::snowflake channel_id) {
		for (auto& msg : oids)
			bot.message_delete(msg, channel_id), sleep_for(200ms);
	}
	inline thread ImportThreads()
	{
		auto update_time = async(memory::update_time);
		auto StructUserMap = async(memory::StructUserMap);
		auto StructGuildMap = async(memory::StructGuildMap);
		auto StructLeaderboardMap = async(memory::StructLeaderboardMap);
		return thread();
	}
}