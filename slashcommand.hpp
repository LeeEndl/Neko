#pragma once
using namespace std;
class slashcommands {
public:
	static void update(command command, bool remove = false)
	{
		switch (command) {
		case command::daily: {
			dpp::slashcommand daily = dpp::slashcommand()
				.set_name("daily")
				.set_description("get a small gift from niko once a day")
				.set_application_id(bot.me.id);
			bot.global_command_create(daily, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::profile: {
			dpp::slashcommand profile = dpp::slashcommand()
				.set_name("profile")
				.set_description("your profile")
				.add_option(dpp::command_option(dpp::co_string, "name", "mention user", true))
				.set_application_id(bot.me.id);
			bot.global_command_create(profile, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::shop: {
			dpp::slashcommand shop = dpp::slashcommand()
				.set_name("shop")
				.set_description("view the shop")
				.set_application_id(bot.me.id);
			bot.global_command_create(shop, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::buy: {
			dpp::slashcommand buy = dpp::slashcommand()
				.set_name("buy")
				.set_description("buy an item from shop")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.add_option(dpp::command_option(dpp::co_string, "amount", "the amount of the item you wanna buy", true))
				.set_application_id(bot.me.id);
			bot.global_command_create(buy, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::sell: {
			dpp::slashcommand sell = dpp::slashcommand()
				.set_name("sell")
				.set_description("sell an item")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.add_option(dpp::command_option(dpp::co_string, "amount", "the amount of the item you wanna sell", true))
				.set_application_id(bot.me.id);
			bot.global_command_create(sell, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::fish: {
			dpp::slashcommand fish = dpp::slashcommand()
				.set_name("fish")
				.set_description("go fishing")
				.set_application_id(bot.me.id);
			bot.global_command_create(fish, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::repair: {
			dpp::slashcommand repair = dpp::slashcommand()
				.set_name("repair")
				.set_description("the item you wanna repair/fix")
				.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
				.set_application_id(bot.me.id);
			bot.global_command_create(repair, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::leaderboard: {
			dpp::slashcommand leaderboard = dpp::slashcommand()
				.set_name("leaderboard")
				.set_description("see top players")
				.set_application_id(bot.me.id);
			bot.global_command_create(leaderboard, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		case command::purge: {
			dpp::slashcommand purge = dpp::slashcommand()
				.set_name("purge")
				.set_description("mass delete messages in a channel")
				.add_option(dpp::command_option(dpp::co_string, "amount", "amount of messages to which be deleted", true))
				.set_default_permissions(dpp::permissions::p_administrator)
				.set_application_id(bot.me.id);
			bot.global_command_create(purge, [remove](const auto& callback) {
				dpp::slashcommand command = get<dpp::slashcommand>(callback.value);
				if (remove) bot.global_command_delete_sync(command.id);
				});
			break;
		}
		default: break;
		}
	}
	class commands
	{
	public:
		static bool daily(const dpp::slashcommand_t& event)
		{
			memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
			tm* mt = memory::mold_time(data.daily);
			if (to_string(mt->tm_mon + 1) + "/" + to_string(mt->tm_mday) not_eq memory::time.month_num + "/" + memory::time.mday)
			{
				int dollar = randomx::Int(30, 92);
				data.daily = time(0);
				data.dollars = data.dollars += dollar;
				memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::cute_blue)
					.set_title("Thanks for opening my gift! :tada:")
					.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else {
				time_t ct = memory::change_time(mt, mt->tm_sec, mt->tm_min, mt->tm_hour, mt->tm_wday += 1, mt->tm_mday += 1, mt->tm_mon);
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::cute_blue)
					.set_description("You've already claimed todays gift! You can obtain a new gift " + memory::timestamp::relevant(ct) + "");
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
			memory::UserData data = memory::FindUser(static_cast<dpp::snowflake>(stoull(name)));
			if (static_cast<bool>(data.failed)) memory::new_user(bot.user_get_sync(static_cast<dpp::snowflake>(stoull(name))));
			dpp::embed embed = dpp::embed()
				.set_color(dpp::colors::cute_blue).set_title(":mag_right: Profile Viewer")
				.set_description(("**<@" + name + ">** ") + (data.last_on == 0 ? "inactive" : "last online " + memory::timestamp::relevant(data.last_on)))
				.add_field
				(
					"Skills: ",
					":fishing_pole_and_fish: Fishing: LVL " + static_cast<string>(data.fishing) + "\n"
				)
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
			if (static_cast<string>(id) == static_cast<string>("1"))
			{
				memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
				int forecast = 15 * stoull(amount);
				if (static_cast<int>(data.dollars) < static_cast<int>(forecast)) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else if (static_cast<string>(data.rod) == static_cast<string>("1") or static_cast<int>(stoi(amount)) > static_cast<int>(1)) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can only purchase one!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars -= forecast;
					data.rod = "1", data.rod_d = 15;
					memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("You've Purchased a Wooden Fishing Rod!");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
			}
			else if (static_cast<string>(id) == static_cast<string>("2"))
			{
				memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
				int forecast = 2 * stoull(amount);
				if (static_cast<int>(data.dollars) < static_cast<int>(forecast)) {
					dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't afford it.");
					event.reply(dpp::message(event.command.channel_id, embed));
				}
				else {
					data.dollars = data.dollars - forecast;
					data.fish = data.fish + stoi(amount);
					memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
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
			if (static_cast<string>(id) == static_cast<string>("1"))
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You can't sell your fishing rod!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
			else if (static_cast<string>(id) == static_cast<string>("2"))
			{
				memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
				data.dollars += 2 * stoull(amount);
				data.fish = data.fish - stoi(amount);
				memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
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
			memory::TempData Tdata = memory::GetTempData(event.command.member.user_id);
			memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
			if (Tdata.busy_fishing) {
				event.reply(event.command.member.get_user()->username + ". Your still fishing!");
				sleep_for(1s), event.delete_original_response();
				return false;
			}
			else if (data.last_fish + 10 > time(0)) {
				{
					memory::TempData data = memory::GetTempData(event.command.member.user_id);
					if (data.once_fishing) return false;
					data.once_fishing = true;
					memory::SaveTempData(data, event.command.member.user_id);
				}
				tm* mt = memory::mold_time(data.last_fish);
				time_t ct = memory::change_time(mt, mt->tm_sec += 12, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
				dpp::embed embed = dpp::embed()
					.set_color(dpp::colors::cute_red)
					.set_description(event.command.member.get_user()->username + ". You can fish again **" + memory::timestamp::relevant(ct) + "**");
				dpp::message msg = dpp::message();
				msg.channel_id = event.command.channel_id;
				msg.add_embed(embed);
				event.reply(msg);
				while (true) {
					if (data.last_fish + 10 < time(0)) {
						event.delete_original_response();
						{
							memory::TempData data = memory::GetTempData(event.command.member.user_id);
							data.once_fishing = false;
							memory::SaveTempData(data, event.command.member.user_id);
						}
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
				future<void> fishing = async(memory::fishing, event);
				fishing.wait();
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
				memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
				data.repair = event.command.member.user_id;
				memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
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
			if (memory::members.empty()) return false;
			string oriented = "";
			vector<pair<uint64_t, uint64_t>> buffer;
			for (auto& member : memory::members) buffer.emplace_back(member.second.dollars, member.second.user_id);
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
			event.reply("DELETING.."), event.delete_original_response();
			string amount = std::get<std::string>(event.get_parameter("amount"));
			int deleted = stoi(amount);
			if (number(amount) == 0) return true;
			if (stoi(amount) <= 1 or stoi(amount) >= 200) return false;
			else {
				auto msgs = bot.messages_get_sync(event.command.channel_id, 0, 0, 0, stoi(amount));
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
				bot.message_create_sync(dpp::message(event.command.channel_id, embed));
				if (not ids.empty()) bot.message_delete_bulk_sync(ids, event.command.channel_id);
				if (not oids.empty()) auto mass_delete = async(memory::mass_delete, oids, event.command.channel_id);
			}
			return true;
		}
	};
};

vector<thread> slashcommands_executed;
inline void await_on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.member.user_id == bot.me.id or event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return;
	bool found = false;  for (auto it = memory::members.begin(); it != memory::members.end(); ++it) {
		if (it->first == event.command.member.user_id) found = true;
	}
	if (not found) {
		ofstream w("maps/members.txt", ios::app);
		w << event.command.member.user_id << '\n';
	}
	{
		memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
		if (static_cast<bool>(data.failed)) memory::new_user(bot.user_get_sync(event.command.member.user_id));
	}
	switch (find_command(event.command.get_command_name()))
	{
	case command::daily: {
		future<bool> daily = async(slashcommands::commands::daily, event);
		daily.wait();
		if (not daily.get()) return;
		break;
	}
	case command::profile: {
		future<bool> profile = async(slashcommands::commands::profile, event);
		profile.wait();
		if (not profile.get()) return;
		break;
	}
	case command::shop: {
		future<bool> shop = async(slashcommands::commands::shop, event);
		shop.wait();
		if (not shop.get()) return;
		break;
	}
	case command::buy: {
		future<bool> buy = async(slashcommands::commands::buy, event);
		buy.wait();
		if (not buy.get()) return;
		break;
	}
	case command::sell: {
		future<bool> sell = async(slashcommands::commands::sell, event);
		sell.wait();
		if (not sell.get()) return;
		break;
	}
	case command::fish: {
		future<bool> fish = async(slashcommands::commands::fish, event);
		fish.wait();
		if (not fish.get()) return;
		break;
	}
	case command::repair: {
		future<bool> repair = async(slashcommands::commands::repair, event);
		repair.wait();
		if (not repair.get()) return;
		break;
	}
	case command::leaderboard: {
		future<bool> leaderboard = async(slashcommands::commands::leaderboard, event);
		leaderboard.wait();
		if (not leaderboard.get()) return;
		break;
	}
	case command::purge: {
		future<bool> purge = async(slashcommands::commands::purge, event);
		purge.wait();
		if (not purge.get()) {
			event.reply("You can only purge 1-200 messages at a time!");
			return;
		}
		break;
	}
	default: return;
	}
	{
		memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
		data.last_on = time(0);
		memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
	}
}
vector<thread> button_clicked_executed;
inline void await_on_button_click(const dpp::button_click_t& event) {
	memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
	vector<string> i = memory::explode(event.custom_id, '_');
	if (event.custom_id.find("repair_1") not_eq -1 and i[2] == event.command.member.user_id)
	{
		if (data.dollars > 12) {
			data.dollars -= 12;
			data.rod = to_string(1);
			data.rod_d = 15;
			dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("Repair Complete!");
			event.reply(dpp::message(event.command.channel_id, embed));
			memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
		}
		else {
			dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You don't have enough to repair it!");
			event.reply(dpp::message(event.command.channel_id, embed));
		}
	}
}