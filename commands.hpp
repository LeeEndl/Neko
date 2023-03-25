#pragma once
namespace commands {
	enum command { prefix, daily, profile, shop, buy, sell, fish, repair, leaderboard, purge, membercount, avatar, invite };
	int find_command_with_prefix(string command, dpp::snowflake guild_id)
	{
		GuildData g_data = GetGuildData(guild_id);
		if (command.find(g_data.prefix + "prefix ") not_eq -1) return command::prefix;
		else if (command.find(g_data.prefix + "daily") not_eq -1) return command::daily;
		else if (command.find(g_data.prefix + "profile ") not_eq -1) return command::profile;
		else if (command.find(g_data.prefix + "shop") not_eq -1) return command::shop;
		else if (command.find(g_data.prefix + "buy ") not_eq -1) return command::buy;
		else if (command.find(g_data.prefix + "sell ") not_eq -1) return command::sell;
		else if (command.find(g_data.prefix + "leaderboard") not_eq -1 or command.find(g_data.prefix + "top") not_eq -1) return command::leaderboard;
		else if (command.find(g_data.prefix + "purge ") not_eq -1) return command::purge;
		else if (command.find(g_data.prefix + "membercount") not_eq -1) return command::membercount;
		else if (command.find(g_data.prefix + "avatar ") not_eq -1 or command.find(g_data.prefix + "avatar") not_eq -1) return command::avatar;
		else if (command.find(g_data.prefix + "invite") not_eq -1) return command::invite;
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
				GuildData g_data = GetGuildData(event.msg.guild_id);
				if (i[1].size() > 1 or i[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
				else {
					g_data.prefix = i[1];
					SaveGuildData(g_data, event.msg.guild_id);
					event.reply("Prefix is now set to: **" + g_data.prefix + "**");
				}
			}
			else event.reply("Sorry, but only administrator can change guild prefix!");
			return true;
		}
		static bool daily(const dpp::message_create_t& event)
		{
			UserData data = GetUserData(event.msg.member.user_id);
			tm* now = dpp::utility::mtm(time(0));
			tm* claimed = dpp::utility::mtm(data.daily);
			if (claimed->tm_mon + claimed->tm_mday not_eq now->tm_mon + now->tm_mday or data.daily == 0)
			{
				int dollar = randomx::Int(30, 92);
				data.daily = time(0);
				data.dollars = data.dollars += dollar;
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::PS)
					.set_title("Thanks for opening my gift! :tada:")
					.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:");
				event.reply(dpp::message(event.msg.channel_id, embed));
				SaveUserData(data, event.msg.member.user_id);
			}
			else {
				tm* claimed = dpp::utility::mtm(data.daily);
				time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour, claimed->tm_wday += 1, claimed->tm_mday += 1, claimed->tm_mon);
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::PS)
					.set_description("You've already claimed todays gift! You can obtain a new gift " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "");
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
			UserData data = GetUserData(stoull(name));
			if (data.failed) new_user(stoull(name));
			string sort = "";
			for (auto& tool : data.tools) {
				sort += (tool.type == 1 ? "Durability: " + to_string(tool.durability) + "/15 :fishing_pole_and_fish: \n" :
					tool.type == -1 ? "Broken :fishing_pole_and_fish:" : "") +
					(tool.type == 2 ? "Durability: " + to_string(tool.durability) + "/15 :knife: \n" :
						tool.type == -2 ? "Broken :knife:" : "");
			}
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS).set_title(":mag_right: Profile Viewer")
				.set_description(("**<@" + name + ">** ") + (data.last_on == 0 ? "inactive" : "last online " + dpp::utility::timestamp(data.last_on, dpp::utility::tf_relative_time)))
				.add_field
				(
					"Tools: ",
					sort.empty() ? "None" : sort
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
				.set_color(dpp::colors::PS)
				.set_title(":shopping_cart: Shop")
				.set_description(
					"`[ID: 1]` **:fishing_pole_and_fish: Wooden Fishing Rod**:\n  - Durability: 15/15\n  - Cost: 15 :dollar:\n\
                     `[ID: 3]` **:knife: Knife**:\n  - Durability: 15/15\n  - Cost: 80 :dollar:"
				)
				.add_field(
					"How to Buy?",
					"type !buy {ID} {Amount}");
			event.reply(dpp::message(event.msg.channel_id, embed));
			return true;
		}
		static bool buy(const dpp::message_create_t& event)
		{
			auto i = explode(event.msg.content, ' ');
			string id = i[1];
			string amount = i[2];
			if (id == "1" or id == "pole" or id == "fishing_pole")
			{
				bool found = false;
				UserData data = GetUserData(event.msg.member.user_id);
				for (auto& tool : data.tools) if (tool.type == 1) found = true;
				int forecast = 15 * stoull(amount);
				if (data.dollars < forecast) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't afford it.");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
				else if (found or stoi(amount) > 1) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can only purchase one!");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					tools buf;
					buf.type = 1, buf.durability = 15;
					data.tools.emplace_back(buf);
					SaveUserData(data, event.msg.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :fishing_pole_and_fish:");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
			}
			if (id == "3" or id == "knife")
			{
				bool found = false;
				UserData data = GetUserData(event.msg.member.user_id);
				for (auto& tool : data.tools) if (tool.type == 2) found = true;
				int forecast = 80 * stoull(amount);
				if (data.dollars < forecast) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't afford it.");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
				else if (found or stoi(amount) > 1) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can only purchase one!");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					tools buf;
					buf.type = 2, buf.durability = 15;
					data.tools.emplace_back(buf);
					SaveUserData(data, event.msg.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :knife:");
					event.reply(dpp::message(event.msg.channel_id, embed));
				}
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID");
				event.reply(dpp::message(event.msg.channel_id, embed));
			}
			return true;
		}
		static bool sell(const dpp::message_create_t& event)
		{
			auto i = explode(event.msg.content, ' ');
			string id = i[1];
			string amount = i[2];
			if (id == "1" or id == "pole" or id == "fishing_pole" or id == "3" or id == "knife")
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't sell your fishing rod!");
				event.reply(dpp::message(event.msg.channel_id, embed));
			}
			else if (id == "fish")
			{
				UserData data = GetUserData(event.msg.member.user_id);
				data.dollars += 2 * stoull(amount);
				data.fish = data.fish - stoi(amount);
				SaveUserData(data, event.msg.member.user_id);
				dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You sold " + amount + " :fish:, and got " + to_string(2 * stoull(amount)) + " :dollar:");
				event.reply(dpp::message(event.msg.channel_id, embed));
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID");
				event.reply(dpp::message(event.msg.channel_id, embed));
			}
			return true;
		}
		static bool leaderboard(const dpp::message_create_t& event)
		{
			if (members.empty()) return false;
			string oriented = "";
			vector<pair<uint64_t, uint64_t>> buffer;
			for (auto& member : members) buffer.emplace_back(member.second.dollars, member.second.user_id);
			sort(buffer.begin(), buffer.end(), first<uint64_t, uint64_t>());
			for (auto& i : buffer) {
				if (i.second == 0) continue;
				oriented += "**" + bot.user_get_sync(i.second).username + "**: " + to_string(i.first) + " :dollar: \n";
			}
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS)
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
				int deleted = 0;
				auto msgs = bot.messages_get_sync(event.msg.channel_id, 0, 0, 0, stoull(i[1]));
				vector<dpp::snowflake> ids, oids;
				if (msgs.size() <= 1) return false;
				for (auto& msg : msgs) {
					if (not msg.second.webhook_id.empty()) continue; // TODO mass delete webhook
					deleted++;
					tm* creation_time = dpp::utility::mtm(msg.second.sent);
					tm* now = dpp::utility::mtm(time(0));
					if (now->tm_mday < creation_time->tm_mday) {
						if (creation_time->tm_mday - now->tm_mday > 14 and creation_time->tm_mon not_eq now->tm_mon or
							creation_time->tm_mday - now->tm_mday < 14 and creation_time->tm_mon not_eq now->tm_mon) {
							oids.emplace_back(msg.second.id);
							continue;
						}
					}
					else
						if (now->tm_mday - creation_time->tm_mday > 14 and creation_time->tm_mon not_eq now->tm_mon or
							creation_time->tm_mday - now->tm_mday < 14 and creation_time->tm_mon not_eq now->tm_mon) {
							oids.emplace_back(msg.second.id);
							continue;
						}
					ids.emplace_back(msg.second.id);
				}
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::PS)
					.set_description("Deleted `" + to_string(deleted) + "` Message(s)");
				event.reply(dpp::message(event.msg.channel_id, embed));
				function<void()> mass_delete = [&]() {
					for (auto& msg : oids) bot.message_delete(msg, event.msg.channel_id), sleep_for(500ms);
				};
				if (not ids.empty()) bot.message_delete_bulk_sync(ids, event.msg.channel_id);
				if (not oids.empty()) async(mass_delete);
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
				.set_color(dpp::colors::PS)
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
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS)
				.set_description(("<@") + (url == event.msg.member.get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(event.msg.member.user_id) : to_string(mention)) + (">'s Avatar"))
				.set_image(url);
			event.reply(dpp::message(event.msg.channel_id, embed));
			return true;
		}
		static bool invite(const dpp::message_create_t& event)
		{
			event.reply("https://discord.com/api/oauth2/authorize?client_id=" +
				to_string(bot.me.id) +
				"&permissions=" + to_string(dpp::permissions::p_administrator) + "&scope=bot%20applications.commands");
			return true;
		}
	};

	vector<thread> commands_executed;
	inline thread await_on_message_create(const dpp::message_create_t& event) {
		UserData data = GetUserData(event.msg.member.user_id);
		if (data.failed) new_user(event.msg.member.user_id);
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
		case command::buy: {
			async(commands::buy, event);
			break;
		}
		case command::sell: {
			async(commands::sell, event);
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
		case command::invite: {
			async(commands::invite, event);
			break;
		}
		default: break;
		}
		{
			UserData data = GetUserData(event.msg.member.user_id);
			data.last_on = std::time(0);
			SaveUserData(data, event.msg.member.user_id);
		}
		return thread();
	}
}