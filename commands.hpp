#pragma once
using namespace std;

const enum command { prefix, daily, profile, shop, buy, sell, fish, repair, leaderboard, purge };
static int find_command(string command)
{
	if (command == "prefix") return command::prefix;
	else if (command == "daily") return command::daily;
	else if (command == "profile") return command::profile;
	else if (command == "shop") return command::shop;
	else if (command == "buy") return command::buy;
	else if (command == "sell") return command::sell;
	else if (command == "fish") return command::fish;
	else if (command == "repair") return command::repair;
	else if (command == "leaderboard") return command::leaderboard;
	else if (command == "purge") return command::purge;
	else return -1;
}
static int find_command_with_prefix(string command, dpp::snowflake guild_id)
{
	memory::GuildData g_data = memory::GetGuildData(guild_id);
	if (command.find(g_data.prefix + "prefix ") not_eq -1) return command::prefix;
	else if (command.find(g_data.prefix + "daily") not_eq -1) return command::daily;
	else if (command.find(g_data.prefix + "leaderboard") not_eq -1) return command::leaderboard;
	else if (command.find(g_data.prefix + "purge ") not_eq -1) return command::purge;
	else return -1;
}
class commands
{
public:
	static bool prefix(const dpp::message_create_t& event)
	{
		auto i = memory::explode(event.msg.content, ' ');
		dpp::user* user = dpp::find_user(event.msg.member.user_id);
		dpp::guild* guild = dpp::find_guild(event.msg.guild_id);
		if (guild->base_permissions(user) & dpp::p_administrator) {
			memory::GuildData g_data = memory::GetGuildData(event.msg.guild_id);
			if (i[1].size() > 1 or i[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
			else {
				g_data.prefix = i[1];
				memory::SaveGuildData(g_data, event.msg.guild_id);
				event.reply("Prefix is now set to: **" + g_data.prefix + "**");
			}
		}
		else event.reply("Sorry, but only administrator can change guild prefix!");
		return true;
	}
	static bool daily(const dpp::message_create_t& event)
	{
		memory::GuildData g_data = memory::GetGuildData(event.msg.guild_id);
		if (g_data.prefix.empty()) return false;
		memory::UserData data = memory::GetUserData(bot.user_get_sync(event.msg.member.user_id));
		tm* mt = memory::mold_time(data.daily);
		if (to_string(mt->tm_mon + 1) + "/" + to_string(mt->tm_mday) not_eq memory::time.month_num + "/" + memory::time.mday)
		{
			int dollar = randomx::Int(30, 92);
			data.daily = time(0);
			data.dollars = data.dollars += dollar;
			memory::SaveUserData(data, bot.user_get_sync(event.msg.member.user_id));
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_title("Thanks for opening my gift! :tada:")
				.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:");
			event.reply(dpp::message(event.msg.channel_id, embed));
		}
		else {
			time_t ct = memory::change_time(mt, mt->tm_sec, mt->tm_min, mt->tm_hour, mt->tm_wday += 1, mt->tm_mday += 1, mt->tm_mon);
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_description("You've already claimed todays gift! You can obtain a new gift " + memory::timestamp::relevant(ct) + "");
			event.reply(dpp::message(event.msg.channel_id, embed));
		}
		return true;
	}
	static bool leaderboard(const dpp::message_create_t& event)
	{
		memory::GuildData g_data = memory::GetGuildData(event.msg.guild_id);
		if (g_data.prefix.empty()) return false;
		string oriented = "";
		ifstream r("maps/leaderboard.txt");
		string line = "";
		while (std::getline(r, line)) {
			auto i = memory::explode(line, ' ');
			memory::UserData data = memory::FindUser(stoull(i[0]));
			oriented += "**" + data.username + "**: " + i[1] + " :dollar: \n";
		}
		dpp::embed embed = dpp::embed()
			.set_color(dpp::colors::cute_blue)
			.set_title("Leaderboard: ")
			.set_description(oriented);
		event.reply(dpp::message(event.msg.channel_id, embed));
		return true;
	}
	static bool purge(const dpp::message_create_t& event)
	{
		auto i = memory::explode(event.msg.content, ' ');
		int deleted = stoi(i[1]);
		if (number(i[1]) == 0) return true;
		if (stoi(i[1]) <= 1 or stoi(i[1]) >= 200) return false;
		else {
			auto msgs = bot.messages_get_sync(event.msg.channel_id, 0, 0, 0, stoull(i[1]) + 1);
			vector<dpp::snowflake> ids, oids;
			for (auto& msg : msgs) {
				tm* mt = memory::mold_time(msg.second.sent);
				if (memory::time.track_time->tm_mday < mt->tm_mday) {
					if (mt->tm_mday - memory::time.track_time->tm_mday > 14 and mt->tm_mon not_eq memory::time.track_time->tm_mon or
						mt->tm_mday - memory::time.track_time->tm_mday < 14 and mt->tm_mon not_eq memory::time.track_time->tm_mon) {
						oids.emplace_back(msg.second.id);
						continue;
					}
				}
				else
					if (memory::time.track_time->tm_mday - mt->tm_mday > 14 and mt->tm_mon not_eq memory::time.track_time->tm_mon or
						mt->tm_mday - memory::time.track_time->tm_mday < 14 and mt->tm_mon not_eq memory::time.track_time->tm_mon) {
						oids.emplace_back(msg.second.id);
						continue;
					}
				ids.emplace_back(msg.second.id);
			}
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_description("Deleted `" + to_string(deleted) + "` Message(s)");
			bot.message_create_sync(dpp::message(event.msg.channel_id, embed));
			if (not ids.empty()) bot.message_delete_bulk_sync(ids, event.msg.channel_id);
			if (not oids.empty()) auto mass_delete = async(memory::mass_delete, oids, event.msg.channel_id);
		}
		return true;
	}
};

vector<thread> commands_executed;
inline void await_on_message_create(const dpp::message_create_t& event) {
	if (event.msg.member.user_id == bot.me.id) return;
	bool found = false;  for (auto it = memory::members.begin(); it != memory::members.end(); ++it) {
		if (it->first == event.msg.member.user_id) found = true;
	}
	if (not found) {
		ofstream w("maps/members.txt", ios::app);
		w << event.msg.member.user_id << '\n';
	}
	memory::GuildData g_data = memory::GetGuildData(event.msg.guild_id);
	memory::UserData data = memory::GetUserData(bot.user_get_sync(event.msg.member.user_id));
	if (static_cast<bool>(data.failed)) memory::new_user(bot.user_get_sync(event.msg.member.user_id));
	switch (find_command_with_prefix(event.msg.content, event.msg.guild_id))
	{
	case command::prefix: {
		future<bool> prefix = async(commands::prefix, event);
		prefix.wait();
		if (not prefix.get()) return;
		break;
	}
	case command::daily: {
		future<bool> daily = async(commands::daily, event);
		break;
	}
	case command::leaderboard: {
		future<bool> leaderboard = async(commands::leaderboard, event);
		leaderboard.wait();
		if (not leaderboard.get()) return;
		break;
	}
	case command::purge: {
		future<bool> purge = async(commands::purge, event);
		break;
	}
	default: return;
	}
	{
		memory::UserData data = memory::GetUserData(bot.user_get_sync(event.msg.member.user_id));
		data.last_on = std::time(0);
		memory::SaveUserData(data, bot.user_get_sync(event.msg.member.user_id));
	}
}