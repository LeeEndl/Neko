﻿#pragma once
dpp::cluster bot(
	static_cast<string>("MTAwNDUxNDkzNTA1OTAwNTQ3MA.GCLzi3.oyFg9LNfawz4yT2t5ZyH87UbAx4iM9mAO_Ou2w"),
	static_cast<uint32_t>(dpp::i_default_intents | dpp::i_message_content)
);

namespace uncategorized
{
	class UserData
	{
	public:
		time_t daily = std::time(0), last_on = 0, last_fish = std::time(0);
		uint64_t user_id = 0;
		bool failed = false;
		string fishing = "", rod = "", username = "";
		int rod_d = 0, fish = 0;
		uint64_t dollars = 0;

		bool busy_fishing = false, once_fishing = false;
	};
	/* first = user_id, second = stored data */
	map<dpp::snowflake, UserData> members;
	class GuildData {
	public:
		bool failed = false, joined = false;
		string prefix = "";
	};
	/* first = guild_id, second = stored data */
	map<dpp::snowflake, GuildData> guilds;
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
		w << setw(2) << j; w.close();
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
		j["username"] = data.username;
		j["user_id"] = data.user_id;
		w << setw(2) << j; w.close();
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
		j["username"] = data.username;
		j["user_id"] = data.user_id;
		w << setw(2) << j; w.close();
	}
	inline void new_user(dpp::user_identified user)
	{
		UserData data = GetUserData(user);
		ofstream w("maps/members.txt", ios::app);
		w << user.id << '\n';
		data.username = user.username;
		data.daily = 0;
		data.dollars = 0;
		data.rod = "", data.rod_d = 0;
		data.fishing = "0", data.fish = 0, data.last_fish = 0;
		data.last_on = 0;
		data.user_id = user.id;
		data.failed = false;
		SaveUserData(data, user); {
			UserData data = GetUserData(user);
			members.emplace(user.id, data);
		}
	}
	inline void new_guild(dpp::snowflake guild_id)
	{
		GuildData data = GetGuildData(guild_id);
		ofstream w("maps/guilds.txt", ios::app);
		w << guild_id << '\n';
		data.joined = false;
		data.prefix = "!";
		data.failed = false;
		SaveGuildData(data, guild_id); {
			GuildData data = GetGuildData(guild_id);
			guilds.emplace(guild_id, data);
		}
	}
	inline void StructUserMap()
	{
		ifstream r("maps/members.txt");
		string line;
		while (getline(r, line)) {
			members.emplace(
				static_cast<dpp::snowflake>(line),
				FindUser(stoull(line)));
		}
	}
	inline void StructGuildMap()
	{
		ifstream r("maps/guilds.txt");
		string line;
		while (getline(r, line)) {
			guilds.emplace(
				static_cast<dpp::snowflake>(line),
				GetGuildData(stoull(line)));
		}
	}
	inline void fishing(const dpp::slashcommand_t& event)
	{
		for (auto& find : uncategorized::members) if (find.first == event.command.member.user_id) find.second.busy_fishing = true;
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
			data.last_fish = std::time(0);
			SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
		}
		for (auto& find : uncategorized::members) if (find.first == event.command.member.user_id) find.second.busy_fishing = false, find.second.busy_fishing = false;
	}
	inline thread mass_delete(vector<dpp::snowflake> oids, dpp::snowflake channel_id) {
		for (auto& msg : oids) bot.message_delete(msg, channel_id), sleep_for(200ms);
		return thread();
	}
	inline thread update_status() {
		while (true)
			bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, to_string(guilds.size()) + " servers")), sleep_for(10s);
		return thread();
	}
	inline thread background_tasks()
	{
		async(SomeTimeStuff::update_time);
		async(update_status);
		return thread();
	}
	vector<thread> ready_executed;
	inline void await_on_ready(const dpp::ready_t& event) {
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		ready_executed.emplace_back(thread::thread(background_tasks));
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
		while (getline(r, line)) {
			if (line not_eq to_string(event.deleted->id)) {
				w << line << '\n';
			}
		}
	}
}