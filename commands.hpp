#pragma once
namespace commands {
	enum command { prefix, daily, profile, shop, buy, sell, fish, repair, leaderboard, purge, membercount, avatar };
	int find_command_with_prefix(string command, dpp::snowflake guild_id)
	{
		uncategorized::GuildData g_data = uncategorized::GetGuildData(guild_id);
		if (command.find(g_data.prefix + "prefix ") not_eq -1) return command::prefix;
		else if (command.find(g_data.prefix + "daily") not_eq -1) return command::daily;
		else if (command.find(g_data.prefix + "profile ") not_eq -1) return command::profile;
		else if (command.find(g_data.prefix + "shop") not_eq -1) return command::shop;
		else if (command.find(g_data.prefix + "leaderboard") not_eq -1 or command.find(g_data.prefix + "top") not_eq -1) return command::leaderboard;
		else if (command.find(g_data.prefix + "purge ") not_eq -1) return command::purge;
		else if (command.find(g_data.prefix + "membercount") not_eq -1) return command::membercount;
		else if (command.find(g_data.prefix + "avatar ") not_eq -1 or command.find(g_data.prefix + "avatar") not_eq -1) return command::avatar;
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
			uncategorized::UserData data = uncategorized::GetUserData(event.msg.member.user_id);
			tm* mt = dpp::utility::mtm(data.daily);
			if (to_string(mt->tm_mon + 1) + "/" + to_string(mt->tm_mday) not_eq SomeTimeStuff::time.month_num + "/" + SomeTimeStuff::time.mday)
			{
				int dollar = randomx::Int(30, 92);
				data.daily = time(0);
				data.dollars = data.dollars += dollar;
				uncategorized::SaveUserData(data, event.msg.member.user_id);
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
					.set_description("You've already claimed today's gift! You can obtain a new gift " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "");
				event.reply(dpp::message(event.msg.channel_id, embed));
			}
			return true;
		}
		static bool profile(const dpp::message_create_t& event)
		{
			auto i = explode(event.msg.content, ' ');
			string name = i[1];
			name.erase(remove(name.begin(), name.end(), '<'), name.end());
			name.erase(remove(name.begin(), name.end(), '>'), name.end());
			name.erase(remove(name.begin(), name.end(), '!'), name.end());
			name.erase(remove(name.begin(), name.end(), '@'), name.end());
			if (has_char(name)) return false; // safely stoull()
			uncategorized::UserData data = uncategorized::GetUserData(stoull(name));
			if (data.failed) uncategorized::new_user(stoull(name));
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue).set_title(":mag_right: Profile Viewer")
				.set_description(("**<@" + name + ">** ") + (data.last_on == 0 ? "inactive" : "last online " + dpp::utility::timestamp(data.last_on, dpp::utility::tf_relative_time)))
				.add_field
				(
					"Tools: ", (
						data.rod == "1" ? "Durability: " + to_string(data.rod_d) + "/15 :fishing_pole_and_fish: \n" :
						data.rod == "-1" ? "Broken :fishing_pole_and_fish:" : "None")
				)
				.add_field
				(
					"Inventory: ",
					static_cast<string>(to_string(data.dollars)) + " :dollar: \n" +
					(data.fish > 0 ? to_string(data.fish) + " :fish: \n" : "")
				);
			event.reply(dpp::message(event.msg.channel_id, embed));
			return true;
		}
		static bool shop(const dpp::message_create_t& event)
		{
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_title(":shopping_cart: Shop")
				.add_field(
					"Permanent Shop: ",
					"`[ID: 1]` **:fishing_pole_and_fish: Wooden Fishing Rod**:\n  - Durability: 15/15\n  - Fishing Level Requirement: None\n  - Cost: 15 :dollar:"
				)
				.add_field(
					"How to Buy?",
					"type /buy {ID} {Amount}");
			event.reply(dpp::message(event.msg.channel_id, embed));
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
		static bool membercount(const dpp::message_create_t& event)
		{
			uint64_t membercount = 0, after = 0;
			while (true) {
				for (auto& members : bot.guild_get_members_sync(event.msg.guild_id, 1000, after)) {
					membercount++;
					if (membercount >= 1000) after = members.second.user_id; else after = 0;
				}
				if (after == 0) goto leave;
			} leave:

			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_description(bot.guild_get_sync(event.msg.guild_id).name + " (**" + to_string(membercount) + "**)")
				.set_timestamp(time(0));
			event.reply(dpp::message(event.msg.channel_id, embed));
			return true;
		}
		static bool avatar(const dpp::message_create_t& event)
		{
			string url = ""; dpp::snowflake mention = 0;
			auto i = explode(event.msg.content, ' ');
			if (i.size() == 1) url = event.msg.member.get_user()->get_avatar_url(256, dpp::image_type::i_png);
			else {
				string name = i[1];
				name.erase(remove(name.begin(), name.end(), '<'), name.end());
				name.erase(remove(name.begin(), name.end(), '>'), name.end());
				name.erase(remove(name.begin(), name.end(), '!'), name.end());
				name.erase(remove(name.begin(), name.end(), '@'), name.end());
				if (has_char(name)) return false; // safely stoull()
				url = bot.user_get_sync(stoull(name)).get_avatar_url(256, dpp::image_type::i_png), mention = stoull(name);
			}
			cout << url << endl;
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue)
				.set_description(("<@") + (url == event.msg.member.get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(event.msg.member.user_id) : to_string(mention)) + (">'s Avatar"))
				.set_image(url);
			event.reply(dpp::message(event.msg.channel_id, embed));
			return true;
		}
	};

	vector<thread> commands_executed;
	inline thread await_on_message_create(const dpp::message_create_t& event) {
		uncategorized::UserData data = uncategorized::GetUserData(event.msg.member.user_id);
		if (data.failed) uncategorized::new_user(event.msg.member.user_id);
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
		case command::profile: {
			auto profile = async(commands::profile, event);
			if (not profile.get()) event.reply("mention or provide there user ID.");
			break;
		}
		case command::shop: {
			async(commands::shop, event);
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
		case command::membercount: {
			async(commands::membercount, event);
			break;
		}
		case command::avatar: {
			async(commands::avatar, event);
			break;
		}
		default: break;
		}
		{
			uncategorized::UserData data = uncategorized::GetUserData(event.msg.member.user_id);
			data.last_on = std::time(0);
			uncategorized::SaveUserData(data, event.msg.member.user_id);
		}
		return thread();
	}
}