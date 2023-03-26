﻿#pragma once
vector<thread> register_slashcommands;
namespace slashcommand {
	void update(commands::command sent, bool remove = false)
	{
		switch (sent) {
		case commands::command::daily: {
			dpp::slashcommand daily = dpp::slashcommand()
				.set_name("daily")
				.set_description("get a small gift from niko once a day")
				.set_application_id(bot.me.id);
			daily = bot.global_command_create_sync(daily);
			if (remove) bot.global_command_delete_sync(daily.id);
			break;
		}
		case commands::command::profile: {
			dpp::slashcommand profile = dpp::slashcommand()
				.set_name("profile")
				.set_description("your profile")
				.add_option(dpp::command_option(dpp::co_string, "name", "mention user", true))
				.set_application_id(bot.me.id);
			profile = bot.global_command_create_sync(profile);
			if (remove) bot.global_command_delete_sync(profile.id);
			break;
		}
		case commands::command::shop: {
			dpp::slashcommand shop = dpp::slashcommand()
				.set_name("shop")
				.set_description("view the shop")
				.set_application_id(bot.me.id);
			shop = bot.global_command_create_sync(shop);
			if (remove) bot.global_command_delete_sync(shop.id);
			break;
		}
		case commands::command::buy: {
			dpp::slashcommand buy = dpp::slashcommand()
				.set_name("buy")
				.set_description("buy an item from shop")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.add_option(dpp::command_option(dpp::co_string, "amount", "the amount of the item you wanna buy", true))
				.set_application_id(bot.me.id);
			buy = bot.global_command_create_sync(buy);
			if (remove) bot.global_command_delete_sync(buy.id);
			break;
		}
		case commands::command::sell: {
			dpp::slashcommand sell = dpp::slashcommand()
				.set_name("sell")
				.set_description("sell an item")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.add_option(dpp::command_option(dpp::co_string, "amount", "the amount of the item you wanna sell", true))
				.set_application_id(bot.me.id);
			sell = bot.global_command_create_sync(sell);
			if (remove) bot.global_command_delete_sync(sell.id);
			break;
		}
		case commands::command::fish: {
			dpp::slashcommand fish = dpp::slashcommand()
				.set_name("fish")
				.set_description("go fishing")
				.set_application_id(bot.me.id);
			fish = bot.global_command_create_sync(fish);
			if (remove) bot.global_command_delete_sync(fish.id);
			break;
		}
		case commands::command::repair: {
			dpp::slashcommand repair = dpp::slashcommand()
				.set_name("repair")
				.set_description("the item you wanna repair/fix")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.set_application_id(bot.me.id);
			repair = bot.global_command_create_sync(repair);
			if (remove) bot.global_command_delete_sync(repair.id);
			break;
		}
		case commands::command::leaderboard: {
			dpp::slashcommand leaderboard = dpp::slashcommand()
				.set_name("leaderboard")
				.set_description("see top players")
				.set_application_id(bot.me.id);
			leaderboard = bot.global_command_create_sync(leaderboard);
			dpp::slashcommand top = dpp::slashcommand()
				.set_name("top")
				.set_description("see top players")
				.set_application_id(bot.me.id);
			top = bot.global_command_create_sync(top);

			if (remove) bot.global_command_delete_sync(leaderboard.id);
			if (remove) bot.global_command_delete_sync(top.id);
			break;
		}
		case commands::command::purge: {
			dpp::slashcommand purge = dpp::slashcommand()
				.set_name("purge")
				.set_description("mass delete messages in a channel")
				.add_option(dpp::command_option(dpp::co_string, "amount", "amount of messages to which be deleted", true))
				.set_default_permissions(dpp::permissions::p_administrator)
				.set_application_id(bot.me.id);
			purge = bot.global_command_create_sync(purge);
			if (remove) bot.global_command_delete_sync(purge.id);
			break;
		}
		case commands::command::membercount: {
			dpp::slashcommand membercount = dpp::slashcommand()
				.set_name("membercount")
				.set_description("view all members in server")
				.set_application_id(bot.me.id);
			membercount = bot.global_command_create_sync(membercount);
			if (remove) bot.global_command_delete_sync(membercount.id);
			break;
		}
		case commands::command::avatar: {
			dpp::slashcommand avatar = dpp::slashcommand()
				.set_name("avatar")
				.set_description("view someone's avatar")
				.add_option(dpp::command_option(dpp::co_string, "name", "person's avatar you wanna view, empty if yourself.", false))
				.set_application_id(bot.me.id);
			avatar = bot.global_command_create_sync(avatar);
			if (remove) bot.global_command_delete_sync(avatar.id);
			break;
		}
		case commands::command::invite: {
			dpp::slashcommand invite = dpp::slashcommand()
				.set_name("invite")
				.set_description("invite " + bot.me.username + " to your server")
				.set_application_id(bot.me.id);
			invite = bot.global_command_create_sync(invite);
			if (remove) bot.global_command_delete_sync(invite.id);
			break;
		}
		case commands::command::hunt: {
			dpp::slashcommand hunt = dpp::slashcommand()
				.set_name("hunt")
				.set_description("hunt down a animal")
				.set_application_id(bot.me.id);
			hunt = bot.global_command_create_sync(hunt);
			if (remove) bot.global_command_delete_sync(hunt.id);
			break;
		}
		default: break;
		}
	}
	void update_all(bool remove = false) {
		int version = stoi(CURRENT_VERSION);
		if (version < 1) async(update, commands::command::daily, remove).wait();
		if (version < 2) async(update, commands::command::profile, remove).wait();
		if (version < 3) async(update, commands::command::shop, remove).wait();
		if (version < 4) async(update, commands::command::buy, remove).wait();
		if (version < 5) async(update, commands::command::sell, remove).wait();
		if (version < 6) async(update, commands::command::fish, remove).wait();
		if (version < 7) async(update, commands::command::repair, remove).wait();
		if (version < 8) async(update, commands::command::leaderboard, remove).wait();
		if (version < 9) async(update, commands::command::purge, remove).wait();
		if (version < 10) async(update, commands::command::membercount, remove).wait();
		if (version < 11) async(update, commands::command::avatar, remove).wait();
		if (version < 12) async(update, commands::command::invite, remove).wait();
		if (version < 13) async(update, commands::command::hunt, remove).wait();
		ofstream("SLASHCOMMAND_VERSION") << SLASHCOMMAND_VERSION;
		print<string>({
			bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ",
			"updated ", to_string(stoi(SLASHCOMMAND_VERSION) - version), " slashcommand", stoi(SLASHCOMMAND_VERSION) - version <= 1 ? "" : "s"
			}, color::normal);
	}
	class slashcommands
	{
	public:
		static bool daily(const dpp::slashcommand_t& event)
		{
			UserData data = GetUserData(event.command.member.user_id);
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
				event.reply(dpp::message(event.command.channel_id, embed));
				SaveUserData(data, event.command.member.user_id);
			}
			else {
				tm* claimed = dpp::utility::mtm(data.daily);
				time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour, claimed->tm_wday += 1, claimed->tm_mday += 1, claimed->tm_mon);
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::PS)
					.set_description("You've already claimed todays gift! You can obtain a new gift " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			return true;
		}
		static bool profile(const dpp::slashcommand_t& event)
		{
			string name = std::get<std::string>(event.get_parameter("name"));
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
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool shop(const dpp::slashcommand_t& event)
		{
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS)
				.set_title(":shopping_cart: Shop")
				.set_description(
					"`[ID: 1]` **:fishing_pole_and_fish: Wooden Fishing Rod**:\n  - Durability: 15/15\n  - Cost: 15 :dollar:\n\
                     `\n[ID: 3]` **:knife: Knife**:\n  - Durability: 15/15\n  - Cost: 80 :dollar:"
				)
				.add_field(
					"How to Buy?",
					"type /buy {ID} {Amount}");
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool buy(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (id == "1" or id == "pole" or id == "fishing_pole")
			{
				bool found = false;
				UserData data = GetUserData(event.command.member.user_id);
				for (auto& tool : data.tools) if (tool.type == 1) found = true;
				int forecast = 15 * stoull(amount);
				if (data.dollars < forecast) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else if (found or stoi(amount) > 1) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can only purchase one!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					tools buf;
					buf.type = 1, buf.durability = 15;
					data.tools.emplace_back(buf);
					SaveUserData(data, event.command.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :fishing_pole_and_fish:");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
			}
			if (id == "3" or id == "knife")
			{
				bool found = false;
				UserData data = GetUserData(event.command.member.user_id);
				for (auto& tool : data.tools) if (tool.type == 2) found = true;
				int forecast = 80 * stoull(amount);
				if (data.dollars < forecast) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else if (found or stoi(amount) > 1) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can only purchase one!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					tools buf;
					buf.type = 2, buf.durability = 15;
					data.tools.emplace_back(buf);
					SaveUserData(data, event.command.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :knife:");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			return true;
		}
		static bool sell(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (id == "1" or id == "pole" or id == "fishing_pole" or id == "3" or id == "knife")
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You can't sell your fishing rod!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else if (id == "fish")
			{
				UserData data = GetUserData(event.command.member.user_id);
				data.dollars += 2 * stoull(amount);
				data.fish = data.fish - stoi(amount);
				SaveUserData(data, event.command.member.user_id);
				dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("You sold " + amount + " :fish:, and got " + to_string(2 * stoull(amount)) + " :dollar:");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			return true;
		}
		static bool fish(const dpp::slashcommand_t& event)
		{
			bool found = false;
			UserData data = GetUserData(event.command.member.user_id);
			for (auto& tool : data.tools) if (tool.type == 1) found = true;
			for (auto& find : members) if (find.first == event.command.member.user_id)
				if (find.second.busy_fishing) {
					event.reply(event.command.member.get_user()->username + ". Your still fishing!");
					sleep_for(2s), event.delete_original_response();
					return false;
				}
				else if (find.second.last_fish + 10 > time(0)) {
					if (find.second.once_fishing) return false;
					find.second.once_fishing = true;
					tm* mt = dpp::utility::mtm(find.second.last_fish);
					time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 12, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
					event.reply(event.command.member.get_user()->username + ". You can fish again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**");
					while (true) {
						if (find.second.last_fish + 10 < time(0)) {
							event.delete_original_response();
							find.second.once_fishing = false;
							goto escape;
						}
					}
				escape: return false;
				}
				else if (not found) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have a fishing rod.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					function<void()> fishing = [&]()
					{
						for (auto& tool : data.tools) if (tool.type == 1)
							if (tool.durability == 0)
							{
								dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Your fishing rod broke! **/repair 1**");
								event.reply(dpp::message(event.command.channel_id, embed));
								for (auto& tool : data.tools) if (tool.type == 1) tool.type = -1;
								SaveUserData(data, event.command.member.user_id);
								return false;
							}
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = true;
						dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Waiting for the fish to bite...");
						dpp::message msg = dpp::message();
						msg.channel_id = event.command.channel_id;
						msg.add_embed(embed);
						event.reply(msg);
						async(Sleep, randomx::Int(6000, 11000));
						for (dpp::embed& e : msg.embeds) e.description = "You caught 1 :fish:!";
						event.edit_response(msg);
						data.fish += 1;
						for (auto& tool : data.tools) if (tool.type == 1) tool.durability -= 1;
						SaveUserData(data, event.command.member.user_id);
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.last_fish = std::time(0);
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = false;
					}; async(fishing);
				}
			return true;
		}
		static bool repair(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string value = "";
			switch (stoi(id)) {
			case 1: value = to_string(12);
			case 3: value = to_string(66);
			}
			if (value.empty()) return false;
			dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_title(":hammer_pick: Repair").add_field(
				"Repairing this will cost :dollar: " + value,
				"Are You Sure?");
			event.reply(dpp::message(event.command.channel_id, embed).add_component(
				dpp::component().add_component(dpp::component()
					.set_label("Repair")
					.set_type(dpp::cot_button)
					.set_style(dpp::cos_success)
					.set_id("repair_" + id + "_" + to_string(event.command.member.user_id))
				)
			));
			return true;
		}
		static bool leaderboard(const dpp::slashcommand_t& event)
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
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool purge(const dpp::slashcommand_t& event)
		{
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (has_char(amount)) return false;
			if (stoi(amount) <= 1 or stoi(amount) > 100) return false;
			else {
				int deleted = 0;
				auto msgs = bot.messages_get_sync(event.command.channel_id, 0, 0, 0, stoi(amount));
				vector<dpp::snowflake> ids, oids;
				if (msgs.size() <= 1) return false;
				for (auto& msg : msgs) {
					if (msg.second.author.username.empty()) continue;
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
				event.reply(dpp::message(event.command.channel_id, embed));
				function<void()> mass_delete = [&]() {
					for (auto& msg : oids) bot.message_delete(msg, event.command.channel_id), sleep_for(500ms);
				};
				if (not ids.empty()) bot.message_delete_bulk_sync(ids, event.command.channel_id);
				if (not oids.empty()) async(mass_delete);
			}
			return true;
		}
		static bool membercount(const dpp::slashcommand_t& event)
		{
			uint64_t membercount = 0, after = 0;
			while (true) {
				for (auto& members : bot.guild_get_members_sync(event.command.guild_id, 1000, after)) {
					membercount++;
					if (membercount >= 1000) after = members.second.user_id; else after = 0;
				}
				if (after == 0) goto leave;
			} leave:

			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS)
				.set_description(bot.guild_get_sync(event.command.guild_id).name + " (**" + to_string(membercount) + "**)")
				.set_timestamp(time(0));
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool avatar(const dpp::slashcommand_t& event)
		{
			string url = ""; dpp::snowflake mention = 0;
			if (event.get_parameter("name").index() == 0) url = event.command.member.get_user()->get_avatar_url(256, dpp::image_type::i_png);
			else {
				string name = get<string>(event.get_parameter("name"));
				name.erase(remove(name.begin(), name.end(), '<'), name.end());
				name.erase(remove(name.begin(), name.end(), '>'), name.end());
				name.erase(remove(name.begin(), name.end(), '!'), name.end());
				name.erase(remove(name.begin(), name.end(), '@'), name.end());
				if (has_char(name)) return false; // safely stoull()
				url = bot.user_get_sync(stoull(name)).get_avatar_url(256, dpp::image_type::i_png), mention = stoull(name);
			}
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::PS)
				.set_description(("<@") + (url == event.command.member.get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(event.command.member.user_id) : to_string(mention)) + (">'s Avatar"))
				.set_image(url);
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool invite(const dpp::slashcommand_t& event)
		{
			event.reply("https://discord.com/api/oauth2/authorize?client_id=" +
				to_string(bot.me.id) +
				"&permissions=" + to_string(dpp::permissions::p_administrator) + "&scope=bot%20applications.commands");
			return true;
		}
		static bool hunt(const dpp::slashcommand_t& event)
		{
			bool found = false;
			UserData data = GetUserData(event.command.member.user_id);
			for (auto& tool : data.tools) if (tool.type == 2) found = true;
			for (auto& find : members) if (find.first == event.command.member.user_id)
				if (find.second.busy_hunting) {
					event.reply(event.command.member.get_user()->username + ". Your still hunting!");
					sleep_for(2s), event.delete_original_response();
					return false;
				}
				else if (find.second.last_hunt + 20 > time(0)) {
					if (find.second.once_hunting) return false;
					find.second.once_hunting = true;
					tm* mt = dpp::utility::mtm(find.second.last_hunt);
					time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 22, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
					event.reply(event.command.member.get_user()->username + ". You can hunt again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**");
					while (true) {
						if (find.second.last_hunt + 20 < time(0)) {
							event.delete_original_response();
							find.second.once_hunting = false;
							goto escape;
						}
					}
				escape: return false;
				}
				else if (not found) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have a weapon.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					function<void()> hunt = [&]() {
						for (auto& tool : data.tools) if (tool.type == 2)
							if (tool.durability == 0)
							{
								dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Your knife broke! **/repair 3**");
								event.reply(dpp::message(event.command.channel_id, embed));
								for (auto& tool : data.tools) if (tool.type == 2) tool.type = -2;
								SaveUserData(data, event.command.member.user_id);
								return false;
							}
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_hunting = true;
						map<int, string> animals{ {1, ":worm:"}, {2, ":lady_beetle:"}, {3, ":rat:"} };
						string enemy = animals[randomx::Int(1, 3)];
						for (auto& a : animal) if (a.first == enemy);
						stats me{ 1, 0, 5 }; // TODO store in JSON
						int eHP = strlen(u8"▰▰▰▰▰▰▰▰▰▰"), HP = strlen(u8"▰▰▰▰▰▰▰▰▰▰"), eleft = 0, left = 0;
						dpp::embed embed = dpp::embed().set_title("a wild " + enemy + " appeared.\n")
							.add_field(event.command.member.get_user()->username + " HP:", u8"`▰▰▰▰▰▰▰▰▰▰`", true)
							.add_field(enemy + " HP:", u8"`▰▰▰▰▰▰▰▰▰▰`", true);
						dpp::message msg = dpp::message();
						msg.channel_id = event.command.channel_id;
						msg.add_embed(embed);
						event.reply(msg);
						while (true) {
							if (eHP < 1) goto end;
							sleep_for(300ms);
							for (auto& a : animal) if (a.first == enemy)
								if (randomx::Int(1, 13) < a.second.Agility) {
									if (a.second.Agility not_eq 0 or a.second.ATK not_eq 0) {
										for (short i = 0; i < a.second.ATK; i++) {
											if (randomx::Int(1, 20) > me.DEF) i++;
											HP -= strlen(u8"▰");
											for (dpp::embed& e : msg.embeds)
												e.fields[0].value.replace(
													e.fields[0].value.begin() + HP + strlen("`"),
													e.fields[0].value.end() - strlen(u8"▰") * left - strlen("`"), u8"▱"
												), left++;
										}
										event.edit_original_response(msg);
									}
								}
							for (auto& a : animal) if (a.first == enemy)
								if (randomx::Int(1, 13) < me.Agility) {
									for (short i = 0; i < me.ATK; i++) {
										if (randomx::Int(1, 20) > a.second.DEF) i++;
										eHP -= strlen(u8"▰");
										for (dpp::embed& e : msg.embeds)
											e.fields[1].value.replace(
												e.fields[1].value.begin() + eHP + strlen("`"),
												e.fields[1].value.end() - strlen(u8"▰") * eleft - strlen("`"), u8"▱"
											), eleft++;
									}
									event.edit_original_response(msg);
								}
						} end:
						int dollar = randomx::Int(6, 20);
						if (HP > 1) msg.content = "You killed it and got " + to_string(dollar) + " :dollar:";
						event.edit_original_response(msg);
						data.dollars += dollar;
						for (auto& tool : data.tools) if (tool.type == 2) tool.durability -= 1;
						SaveUserData(data, event.command.member.user_id);
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.last_hunt = std::time(0);
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_hunting = false;
					}; async(hunt);
				}
			return true;
		}
	};
	int find_command(string command)
	{
		if (command == "prefix") return commands::command::prefix;
		else if (command == "daily") return commands::command::daily;
		else if (command == "profile") return commands::command::profile;
		else if (command == "shop") return commands::command::shop;
		else if (command == "buy") return commands::command::buy;
		else if (command == "sell") return commands::command::sell;
		else if (command == "fish") return commands::command::fish;
		else if (command == "repair") return commands::command::repair;
		else if (command == "leaderboard" or command == "top") return commands::command::leaderboard;
		else if (command == "purge") return commands::command::purge;
		else if (command == "membercount") return commands::command::membercount;
		else if (command == "avatar") return commands::command::avatar;
		else if (command == "invite") return commands::command::invite;
		else if (command == "hunt") return commands::command::hunt;
		else return -1;
	}
	vector<thread> slashcommands_executed;
	inline thread await_on_slashcommand(const dpp::slashcommand_t& event) {
		UserData data = GetUserData(event.command.member.user_id);
		if (data.failed) new_user(event.command.member.user_id);
		switch (find_command(event.command.get_command_name()))
		{
		case commands::command::daily: {
			async(slashcommands::daily, event);
			break;
		}
		case commands::command::profile: {
			auto profile = async(slashcommands::profile, event);
			if (not profile.get()) event.reply("mention or provide there user ID.");
			break;
		}
		case commands::command::shop: {
			async(slashcommands::shop, event);
			break;
		}
		case commands::command::buy: {
			async(slashcommands::buy, event);
			break;
		}
		case commands::command::sell: {
			async(slashcommands::sell, event);
			break;
		}
		case commands::command::fish: {
			async(slashcommands::fish, event);
			break;
		}
		case commands::command::repair: {
			auto repair = async(slashcommands::repair, event);
			if (not repair.get()) event.reply("invalid ID");
			break;
		}
		case commands::command::leaderboard: {
			async(slashcommands::leaderboard, event);
			break;
		}
		case commands::command::purge: {
			auto purge = async(slashcommands::purge, event);
			if (not purge.get()) event.reply("You can only purge 1-200 messages at a time!");
			break;
		}
		case commands::command::membercount: {
			async(slashcommands::membercount, event);
			break;
		}
		case commands::command::avatar: {
			async(slashcommands::avatar, event);
			break;
		}
		case commands::command::invite: {
			async(slashcommands::invite, event);
			break;
		}
		case commands::command::hunt: {
			async(slashcommands::hunt, event);
			break;
		}
		default: break;
		}
		{
			UserData data = GetUserData(event.command.member.user_id);
			data.last_on = time(0);
			SaveUserData(data, event.command.member.user_id);
		}
		return thread();
	}
	vector<thread> button_clicked_executed;
	inline void await_on_button_click(const dpp::button_click_t& event) {
		UserData data = GetUserData(event.command.member.user_id);
		vector<string> i = explode(event.custom_id, '_');
		if (event.custom_id.find("repair_1") not_eq -1 and i[2] == to_string(event.command.member.user_id))
		{
			if (data.dollars > 12) {
				data.dollars -= 12;
				for (auto& tool : data.tools) if (tool.type == -1) tool.type = 1, tool.durability = 15;
				dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Repair Complete!");
				event.reply(dpp::message(event.command.channel_id, embed));
				SaveUserData(data, event.command.member.user_id);
			}
			else {
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
		}
		if (event.custom_id.find("repair_3") not_eq -1 and i[2] == to_string(event.command.member.user_id))
		{
			if (data.dollars > 66) {
				data.dollars -= 66;
				for (auto& tool : data.tools) if (tool.type == -2) tool.type = 2, tool.durability = 15;
				dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Repair Complete!");
				event.reply(dpp::message(event.command.channel_id, embed));
				SaveUserData(data, event.command.member.user_id);
			}
			else {
				dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
		}
		return;
	}
}