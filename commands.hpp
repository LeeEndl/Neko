#pragma once

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
	uncategorized::GuildData g_data = uncategorized::GetGuildData(guild_id);
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
		auto i = explode(event.msg.content, ' ');
		dpp::user* user = dpp::find_user(event.msg.member.user_id);
		dpp::guild* guild = dpp::find_guild(event.msg.guild_id);
		if (guild->base_permissions(user) & dpp::p_administrator) {
			uncategorized::GuildData g_data = uncategorized::GetGuildData(event.msg.guild_id);
			if (i[1].size() > 1 or i[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
			else {
				g_data.prefix = i[1];
				uncategorized::SaveGuildData(g_data, event.msg.guild_id);
				event.reply("Prefix is now set to: **" + g_data.prefix + "**");
			}
		}
		else event.reply("Sorry, but only administrator can change guild prefix!");
		return true;
	}
	static bool daily(const dpp::message_create_t& event)
	{
		uncategorized::GuildData g_data = uncategorized::GetGuildData(event.msg.guild_id);
		if (g_data.prefix.empty()) return false;
		uncategorized::UserData data = uncategorized::GetUserData(bot.user_get_sync(event.msg.member.user_id));
		tm* mt = dpp::utility::mtm(data.daily);
		if (to_string(mt->tm_mon + 1) + "/" + to_string(mt->tm_mday) not_eq SomeTimeStuff::time.month_num + "/" + SomeTimeStuff::time.mday)
		{
			int dollar = randomx::Int(30, 92);
			data.daily = time(0);
			data.dollars = data.dollars += dollar;
			uncategorized::SaveUserData(data, bot.user_get_sync(event.msg.member.user_id));
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_title("Thanks for opening my gift! :tada:")
				.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:");
			event.reply(dpp::message(event.msg.channel_id, embed));
		}
		else {
			time_t ct = dpp::utility::mt_t(mt, mt->tm_sec, mt->tm_min, mt->tm_hour, mt->tm_wday += 1, mt->tm_mday += 1, mt->tm_mon);
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_description("You've already claimed todays gift! You can obtain a new gift " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "");
			event.reply(dpp::message(event.msg.channel_id, embed));
		}
		return true;
	}
	static bool leaderboard(const dpp::message_create_t& event)
	{
		if (uncategorized::members.empty()) return false;
		string oriented = "";
		vector<pair<uint64_t, uint64_t>> buffer;
		for (auto& member : uncategorized::members) buffer.emplace_back(member.second.dollars, member.second.user_id);
		sort(buffer.begin(), buffer.end(), first<uint64_t, uint64_t>());
		for (auto& i : buffer) {
			if (i.second == 0) continue;
			oriented += "**" + bot.user_get_sync(i.second).username + "**: " + to_string(i.first) + " :dollar: \n";
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
		auto i = explode(event.msg.content, ' ');
		if (has_char(i[1])) return false;
		if (stoi(i[1]) <= 1 or stoi(i[1]) >= 200) return false;
		else {
			int deleted = stoi(i[1]);
			auto msgs = bot.messages_get_sync(event.msg.channel_id, 0, 0, 0, stoull(i[1]) + 1);
			vector<dpp::snowflake> ids, oids;
			for (auto& msg : msgs) {
				tm* tm = dpp::utility::mtm(msg.second.sent);
				if (SomeTimeStuff::time.track_time->tm_mday < tm->tm_mday) {
					if (tm->tm_mday - SomeTimeStuff::time.track_time->tm_mday > 14 and tm->tm_mon not_eq SomeTimeStuff::time.track_time->tm_mon or
						tm->tm_mday - SomeTimeStuff::time.track_time->tm_mday < 14 and tm->tm_mon not_eq SomeTimeStuff::time.track_time->tm_mon) {
						oids.emplace_back(msg.second.id);
						continue;
					}
				}
				else
					if (SomeTimeStuff::time.track_time->tm_mday - tm->tm_mday > 14 and tm->tm_mon not_eq SomeTimeStuff::time.track_time->tm_mon or
						tm->tm_mday - SomeTimeStuff::time.track_time->tm_mday < 14 and tm->tm_mon not_eq SomeTimeStuff::time.track_time->tm_mon) {
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
			if (not oids.empty()) async(uncategorized::mass_delete, oids, event.msg.channel_id);
		}
		return true;
	}
};

vector<thread> commands_executed;
inline void await_on_message_create(const dpp::message_create_t& event) {
	bool found = false;
	for (auto& find : uncategorized::members) if (find.first == event.msg.member.user_id) found = true;
	if (not found) {
		ofstream w("maps/members.txt", ios::app);
		w << event.msg.member.user_id << '\n';
	}
	uncategorized::GuildData g_data = uncategorized::GetGuildData(event.msg.guild_id);
	uncategorized::UserData data = uncategorized::GetUserData(bot.user_get_sync(event.msg.member.user_id));
	if (data.failed) uncategorized::new_user(bot.user_get_sync(event.msg.member.user_id));
	switch (find_command_with_prefix(event.msg.content, event.msg.guild_id))
	{
	case command::prefix: {
		async(commands::prefix, event);
		break;
	}
	case command::daily: {
		async(commands::daily, event);
		break;
	}
	case command::leaderboard: {
		async(commands::leaderboard, event);
		break;
	}
	case command::purge: {
		auto purge = async(commands::purge, event);
		if (not purge.get()) event.reply("You can only purge 1-200 messages at a time!");
		break;
	}
	default: break;
	}
	{
		uncategorized::UserData data = uncategorized::GetUserData(bot.user_get_sync(event.msg.member.user_id));
		data.last_on = std::time(0);
		uncategorized::SaveUserData(data, bot.user_get_sync(event.msg.member.user_id));
	}
	event.cancel_event();
	return;
}