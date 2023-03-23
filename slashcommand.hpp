#pragma once
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
		default: break;
		}
	}
	void update_all(bool remove = false) {
		update(commands::command::daily, remove);
		update(commands::command::profile, remove);
		update(commands::command::shop, remove);
		update(commands::command::buy, remove);
		update(commands::command::sell, remove);
		update(commands::command::fish, remove);
		update(commands::command::repair, remove);
		update(commands::command::leaderboard, remove);
		update(commands::command::purge, remove);
		update(commands::command::membercount, remove);
		update(commands::command::avatar, remove);
		update(commands::command::invite, remove);
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
					.set_color(dpp::colors::cute_blue)
					.set_title("Thanks for opening my gift! :tada:")
					.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:");
				event.reply(dpp::message(event.command.channel_id, embed));
				SaveUserData(data, event.command.member.user_id);
			}
			else {
				tm* claimed = dpp::utility::mtm(data.daily);
				time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour, claimed->tm_wday += 1, claimed->tm_mday += 1, claimed->tm_mon);
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::cute_blue)
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
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool shop(const dpp::slashcommand_t& event)
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
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool buy(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (id == "1")
			{
				UserData data = GetUserData(event.command.member.user_id);
				int forecast = 15 * stoull(amount);
				if (data.dollars < forecast) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else if (data.rod == "1" or stoi(amount) > 1) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can only purchase one!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					data.rod = "1", data.rod_d = 15;
					SaveUserData(data, event.command.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("You've Purchased a Wooden Fishing Rod!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
			}
			else if (id == "2")
			{
				UserData data = GetUserData(event.command.member.user_id);
				int forecast = 2 * stoull(amount);
				if (static_cast<int>(data.dollars) < static_cast<int>(forecast)) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars - forecast;
					data.fish = data.fish + stoi(amount);
					SaveUserData(data, event.command.member.user_id);
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("You've Purchased a Wooden Fishing Rod!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("Invalid ID");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			return true;
		}
		static bool sell(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (static_cast<string>(id) == "rod" or static_cast<string>(id) == "pole" or static_cast<string>(id) == "fishing_pole" or static_cast<string>(id) == "fishing_rod")
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't sell your fishing rod!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else if (static_cast<string>(id) == "fish")
			{
				UserData data = GetUserData(event.command.member.user_id);
				data.dollars += 2 * stoull(amount);
				data.fish = data.fish - stoi(amount);
				SaveUserData(data, event.command.member.user_id);
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("You sold " + amount + " :fish:, and got " + to_string(2 * stoull(amount)) + " :dollar:");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("Invalid ID");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			return true;
		}
		static bool fish(const dpp::slashcommand_t& event)
		{
			UserData data = GetUserData(event.command.member.user_id);
			for (auto& find : members) if (find.first == event.command.member.user_id)
				if (find.second.busy_fishing) {
					event.reply(event.command.member.get_user()->username + ". Your still fishing!");
					sleep_for(1s), event.delete_original_response();
					return false;
				}
				else if (data.last_fish + 10 > time(0)) {
					{
						for (auto& find : members) if (find.first == event.command.member.user_id) {
							if (find.second.once_fishing) return false;
							find.second.once_fishing = true;
						}
					}
					tm* mt = dpp::utility::mtm(data.last_fish);
					time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 12, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
					event.reply(event.command.member.get_user()->username + ". You can fish again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**");
					while (true) {
						if (data.last_fish + 10 < time(0)) {
							event.delete_original_response();
							for (auto& find : members) if (find.first == event.command.member.user_id) find.second.once_fishing = false;
							goto escape;
						}
					}
				escape: return false;
				}
				else if (data.rod == "" or data.rod == "0" or data.rod == "-1") {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You don't have a fishing rod.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					auto fishing = [&]()
					{
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = true;
						UserData data = GetUserData(event.command.member.user_id);
						if (data.rod not_eq "-1" and data.rod_d == 0)
						{
							dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("Your fishing rod broke! **/repair 1**");
							event.reply(dpp::message(event.command.channel_id, embed));
							data.rod = "-1";
						}
						SaveUserData(data, event.command.member.user_id);
						{
							UserData data = GetUserData(event.command.member.user_id);
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
								SaveUserData(data, event.command.member.user_id);
							}
						}
						for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = false, find.second.busy_fishing = false;
					};
					auto _fishing = async(fishing);
					_fishing.wait();
				}
			return true;
		}
		static bool repair(const dpp::slashcommand_t& event)
		{
			string id = std::get<std::string>(event.get_parameter("id"));
			string value = "";
			switch (stoi(id)) {
			case 1: value = to_string(12);
			}
			if (stoi(id) == 1)
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_title(":hammer_pick: Repair").add_field(
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
			}
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
				.set_color(dpp::colors::cute_blue)
				.set_title("Leaderboard: ")
				.set_description(oriented);
			event.reply(dpp::message(event.command.channel_id, embed));
			return true;
		}
		static bool purge(const dpp::slashcommand_t& event)
		{
			string amount = std::get<std::string>(event.get_parameter("amount"));
			if (has_char(amount)) return false;
			if (stoi(amount) <= 1 or stoi(amount) >= 200) return false;
			else {
				int deleted = 0;
				auto msgs = bot.messages_get_sync(event.command.channel_id, 0, 0, 0, stoi(amount));
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
					.set_color(dpp::colors::cute_blue)
					.set_description("Deleted `" + to_string(deleted) + "` Message(s)");
				event.reply(dpp::message(event.command.channel_id, embed));
				auto mass_delete = [&]() {
					for (auto& msg : oids) bot.message_delete(msg, event.command.channel_id), sleep_for(200ms);
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
				.set_color(dpp::colors::cute_blue)
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
				.set_color(dpp::colors::cute_blue)
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
			async(slashcommands::repair, event);
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
				data.rod = to_string(1);
				data.rod_d = 15;
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("Repair Complete!");
				event.reply(dpp::message(event.command.channel_id, embed));
				SaveUserData(data, event.command.member.user_id);
			}
			else {
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You don't have enough to repair it!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
		}
		return;
	}
}