#pragma once
enum command { prefix, daily, profile, shop, buy, sell, fish, repair, leaderboard, purge, membercount, avatar, invite, hunt };
vector<thread> button_clicked_executed;
inline void await_on_button_click(const dpp::button_click_t& event) {
	UserData data = GetUserData(event.command.member.user_id);
	vector<string> index = dpp::index(event.custom_id, '_');
	if (event.custom_id.find("repair_1") not_eq -1 and index[2] == to_string(event.command.member.user_id)) {
		if (data.dollars > 12) {
			data.dollars -= 12;
			for (auto& tool : data.tools) if (tool.name == "B:fishing_pole_and_fish:") tool.name = ":fishing_pole_and_fish:", tool.durability = 15;
			dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Repair Complete!");
			event.reply(dpp::message(event.command.channel_id, embed));
			SaveUserData(data, event.command.member.user_id);
		}
		else {
			dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!");
			event.reply(dpp::message(event.command.channel_id, embed));
		}
	}
	if (event.custom_id.find("repair_3") not_eq -1 and index[2] == to_string(event.command.member.user_id)) {
		if (data.dollars > 66) {
			data.dollars -= 66;
			for (auto& tool : data.tools) if (tool.name == "B:knife:") tool.name = ":knife:", tool.durability = 15;
			dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Repair Complete!");
			event.reply(dpp::message(event.command.channel_id, embed));
			SaveUserData(data, event.command.member.user_id);
		}
		else {
			dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!");
			event.reply(dpp::message(event.command.channel_id, embed));
		}
	}
}
bool prefix_t(const dpp::message_create_t& event)
{
	vector<string> index = dpp::index(event.msg.content, ' ');
	dpp::user* user = dpp::find_user(event.msg.member.user_id);
	dpp::guild* guild = dpp::find_guild(event.msg.guild_id);
	if (guild->base_permissions(user) & dpp::p_administrator) {
		GuildData g_data = GetGuildData(event.msg.guild_id);
		if (index[1].size() > 1 or index[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
		else {
			g_data.prefix = index[1];
			SaveGuildData(g_data, event.msg.guild_id);
			event.reply("Prefix is now set to: **" + g_data.prefix + "**");
		}
	}
	else event.reply("Sorry, but only administrator can change guild prefix!");
	return true;
}
template<typename event_t> bool daily_t(event_t event)
{
	UserData data = GetUserData(dpp::member(event).user_id);
	tm* now = dpp::utility::mtm(time(0));
	tm* claimed = dpp::utility::mtm(data.daily);
	time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour, claimed->tm_wday += 1, claimed->tm_mday += 1, claimed->tm_mon);
	if (ct < time(0)) {
		uint64_t dollar = randomx::Int(30, 92);
		data.daily = time(0);
		data.dollars += dollar;
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed()
			.set_color(dpp::colors::PS)
			.set_title("Thanks for opening my gift! :tada:")
			.set_description("- " + static_cast<string>(to_string(dollar)) + " :dollar:")));
		SaveUserData(data, dpp::member(event).user_id);
		for (auto& member : members) cout << member.second.dollars << endl;
	}
	else
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed()
			.set_color(dpp::colors::PS)
			.set_description("You've already claimed todays gift! You can obtain a new gift " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "")));
	return true;
}
template<typename event_t> bool profile_t(event_t event)
{
	string name = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name");
	else name = dpp::index(event, "1");
	name.erase(remove(name.begin(), name.end(), '<'), name.end()), name.erase(remove(name.begin(), name.end(), '>'), name.end());
	name.erase(remove(name.begin(), name.end(), '!'), name.end());
	name.erase(remove(name.begin(), name.end(), '@'), name.end());
	if (has_char(name)) return false;
	UserData data = GetUserData(stoull(name));
	if (data.failed) new_user(stoull(name));
	string sort = "";
	for (auto& tool : data.tools) if (not tool.name.empty()) sort += (tool.name.find("B:") or tool.durability < 1 ? "Broken " + tool.name : "Durability: " + to_string(tool.durability) + " " + tool.name);
	dpp::embed embed = dpp::embed()
		.set_color(dpp::colors::PS).set_title(":mag_right: Profile Viewer")
		.set_description(("**<@" + name + ">** ") + (data.last_on == 0 ? "inactive" : "last online " + dpp::utility::timestamp(data.last_on, dpp::utility::tf_relative_time)))
		.add_field(
			"Tools: ",
			sort.empty() ? "None" : sort
		)
		.add_field(
			"Inventory: ",
			to_string(data.dollars) + " :dollar: \n" +
			(data.fish > 0 ? to_string(data.fish) + " :fish: \n" : "")
		);
	event.reply(dpp::message(dpp::channel_id(event), embed));
	return true;
}
template<typename event_t> bool shop_t(event_t event)
{
	dpp::embed embed = dpp::embed()
		.set_color(dpp::colors::PS)
		.set_title(":shopping_cart: Shop")
		.set_description(
			"`[ID: 1]` **:fishing_pole_and_fish: Wooden Fishing Rod**:\n  - Durability: 15/15\n  - Cost: 15 :dollar:\n\n\
             `[ID: 3]` **:knife: Knife**:\n  - Durability: 15/15\n  - Cost: 80 :dollar:"
		)
		.add_field(
			"How to Buy?",
			"type !buy {ID} {Amount}");
	event.reply(dpp::message(dpp::channel_id(event), embed));
	return true;
}
template<typename event_t> bool buy_t(event_t event)
{
	string id = "", amount = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) id = dpp::index(event, "id"), amount = dpp::index(event, "amount");
	else id = dpp::index(event, "1"), amount = dpp::index(event, "2");
	if (id == "1" or id == "fishing_pole" or id == "fishing_pole_and_fish") {
		bool found = false;
		UserData data = GetUserData(dpp::member(event).user_id);
		for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") found = true;
		int forecast = 15 * stoull(amount);
		if (data.dollars < forecast) event.reply("You can't afford it.");
		else if (found or stoi(amount) > 1) event.reply("You can only purchase one!");
		else {
			data.dollars = data.dollars -= forecast;
			tools buf;
			buf.name = ":fishing_pole_and_fish:", buf.durability = 15;
			data.tools.emplace_back(buf);
			SaveUserData(data, dpp::member(event).user_id);
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :fishing_pole_and_fish:")));
		}
	}
	else if (id == "3" or id == "knife") {
		bool found = false;
		UserData data = GetUserData(dpp::member(event).user_id);
		for (auto& tool : data.tools) if (tool.name == ":knife:") found = true;
		int forecast = 80 * stoull(amount);
		if (data.dollars < forecast) event.reply("You can't afford it.");
		else if (found or stoi(amount) > 1) event.reply("You can only purchase one!");
		else {
			data.dollars = data.dollars -= forecast;
			tools buf;
			buf.name = ":knife:", buf.durability = 15;
			data.tools.emplace_back(buf);
			SaveUserData(data, dpp::member(event).user_id);
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("You've Purchased a :knife:")));
		}
	}
	else event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID")));
	return true;
}
template<typename event_t> bool sell_t(event_t event)
{
	string id = "", amount = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) id = dpp::index(event, "id"), amount = dpp::index(event, "amount");
	else id = dpp::index(event, "1"), amount = dpp::index(event, "2");
	if (id == "1" or id == "pole" or id == "fishing_pole" or id == "3" or id == "knife")
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("You can't sell your fishing rod!")));
	else if (id == "fish") {
		UserData data = GetUserData(dpp::member(event).user_id);
		data.dollars += 2 * stoull(amount);
		data.fish = data.fish - stoi(amount);
		SaveUserData(data, dpp::member(event).user_id);
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("You sold " + amount + " :fish:, and got " + to_string(2 * stoull(amount)) + " :dollar:")));
	}
	else event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID")));
	return true;
}
template<typename event_t> bool fish_t(event_t event)
{
	bool found = false;
	UserData data = GetUserData(dpp::member(event).user_id);
	for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") found = true;
	for (auto& find : members) if (find.first == dpp::member(event).user_id)
		if (find.second.busy_fishing) return false;
		else if (find.second.last_fish + 10 > time(0)) {
			if (find.second.once_fishing) return false;
			find.second.once_fishing = true;
			tm* mt = dpp::utility::mtm(find.second.last_fish);
			time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 12, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
			dpp::message msg = dpp::message_create(event, dpp::message(dpp::channel_id(event), dpp::member(event).get_user()->username + ". You can fish again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**"));
			while (true) {
				if (find.second.last_fish + 10 < time(0)) {
					dpp::message_delete(event, msg.id, dpp::channel_id(event));
					find.second.once_fishing = false;
					goto escape;
				}
			}
		escape: return false;
		}
		else if (not found) event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("You don't have a fishing rod.")));
		else {
			function<void()> fishing = [&]() {
				for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:")
					if (tool.durability == 0) {
						dpp::embed embed = dpp::embed().set_color(dpp::colors::failed).set_description("Your fishing rod broke! **/repair 1**");
						event.reply(dpp::message(dpp::channel_id(event), embed));
						for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") tool.name = "B:fishing_pole_and_fish:";
						SaveUserData(data, dpp::member(event).user_id);
						return false;
					}
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_fishing = true;
				dpp::embed embed = dpp::embed().set_color(dpp::colors::PS).set_description("Waiting for the fish to bite...");
				dpp::message msg = dpp::message_create(event, dpp::message(dpp::channel_id(event), embed));
				async(Sleep, randomx::Int(6000, 11000));
				for (dpp::embed& e : msg.embeds) e.description = "You caught 1 :fish:!";
				dpp::message_edit(event, msg);
				data.fish += 1;
				for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") tool.durability -= 1;
				SaveUserData(data, dpp::member(event).user_id);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.last_fish = std::time(0);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_fishing = false;
				return true;
			}; async(fishing);
		}
	return true;
}
template<typename event_t> bool repair_t(event_t event)
{
	string id = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) id = dpp::index(event, "id");
	else id = dpp::index(event, "1");
	string value = "";
	switch (stoi(id)) {
	case 1: value = to_string(12);
	case 3: value = to_string(66);
	}
	if (value.empty()) return false;
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_title(":hammer_pick: Repair").add_field(
		"Repairing this will cost :dollar: " + value,
		"Are You Sure?")).add_component(
			dpp::component().add_component(dpp::component()
				.set_label("Repair")
				.set_type(dpp::cot_button)
				.set_style(dpp::cos_success)
				.set_id("repair_" + id + "_" + to_string(dpp::member(event).user_id))
			)
		));
	return true;
}
template<typename event_t> bool leaderboard_t(event_t event)
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
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_title("Leaderboard: ").set_description(oriented)));
	return true;
}
template<typename event_t> bool purge_t(event_t event)
{
	string amount = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) amount = dpp::index(event, "amount");
	else amount = dpp::index(event, "1");
	if (has_char(amount)) return false;
	if (stoi(amount) <= 1 or stoi(amount) > 100) return false;
	else {
		int deleted = 0;
		auto msgs = bot.messages_get_sync(dpp::channel_id(event), 0, 0, 0, stoi(amount));
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
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("Deleted `" + to_string(deleted) + "` Message(s)")));
		function<void()> mass_delete = [&]() {
			for (auto& msg : oids) bot.message_delete(msg, dpp::channel_id(event)), sleep_for(500ms);
		};
		if (not ids.empty()) bot.message_delete_bulk_sync(ids, dpp::channel_id(event));
		if (not oids.empty()) async(mass_delete);
	}
	return true;
}
template<typename event_t> bool membercount_t(event_t event)
{
	uint64_t membercount = 0, after = 0;
	while (true) {
		for (auto& members : bot.guild_get_members_sync(dpp::guild_id(event), 1000, after)) {
			membercount++;
			if (membercount >= 1000) after = members.second.user_id; else after = 0;
		}
		if (after == 0) goto leave;
	} leave:
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS)
		.set_description(bot.guild_get_sync(dpp::guild_id(event)).name + " (**" + to_string(membercount) + "**)")
		.set_timestamp(time(0))));
	return true;
}
template<typename event_t> bool avatar_t(event_t event)
{
	string url = ""; dpp::snowflake mention = 0;
	if (is_same_v<decltype(event), const dpp::slashcommand_t&> ? dpp::empty_index(event, "name") : dpp::empty_index(event, "1")) url = dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png);
	else {
		string name = "";
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name");
		else name = dpp::index(event, "1");
		name.erase(remove(name.begin(), name.end(), '<'), name.end());
		name.erase(remove(name.begin(), name.end(), '>'), name.end());
		name.erase(remove(name.begin(), name.end(), '!'), name.end());
		name.erase(remove(name.begin(), name.end(), '@'), name.end());
		if (has_char(name)) return false;
		url = bot.user_get_sync(stoull(name)).get_avatar_url(256, dpp::image_type::i_png), mention = stoull(name);
	}
	dpp::embed embed = dpp::embed()
		.set_color(dpp::colors::PS)
		.set_description(("<@") + (url == dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(dpp::member(event).user_id) : to_string(mention)) + (">'s Avatar"))
		.set_image(url);
	event.reply(dpp::message(dpp::guild_id(event), embed));
	return true;
}
template<typename event_t> bool invite_t(event_t event)
{
	event.reply("https://discord.com/api/oauth2/authorize?client_id=" +
		to_string(bot.me.id) +
		"&permissions=" + to_string(dpp::permissions::p_administrator) + "&scope=bot%20applications.commands");
	return true;
}
template<typename event_t> bool hunt_t(event_t event)
{
	bool found = false;
	UserData data = GetUserData(dpp::member(event).user_id);
	for (auto& tool : data.tools) if (tool.name == ":knife:") found = true;
	for (auto& find : members) if (find.first == dpp::member(event).user_id)
		if (find.second.busy_hunting) return false;
		else if (find.second.last_hunt + 20 > time(0)) {
			if (find.second.once_hunting) return false;
			find.second.once_hunting = true;
			tm* mt = dpp::utility::mtm(find.second.last_hunt);
			time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 22, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon);
			dpp::message msg = dpp::message_create(event, dpp::message(dpp::channel_id(event), dpp::member(event).get_user()->username + ". You can hunt again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**"));
			while (true) {
				if (find.second.last_hunt + 20 < time(0)) {
					dpp::message_delete(event, msg.id, dpp::channel_id(event));
					find.second.once_hunting = false;
					goto escape;
				}
			}
		escape: return false;
		}
		else if (not found) event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("You don't have a weapon.")));
		else {
			function<void()> hunt = [&]() {
				for (auto& tool : data.tools) if (tool.name == ":knife:")
					if (tool.durability == 0)
					{
						event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("Your knife broke! **/repair 3**")));
						for (auto& tool : data.tools) if (tool.name == ":knife:") tool.name = "B:knife:";
						SaveUserData(data, dpp::member(event).user_id);
						return false;
					}
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_hunting = true;
				map<int, string> animals{ {1, ":worm:"}, {2, ":lady_beetle:"}, {3, ":rat:"}, {4, ":butterfly:"}, {5, ":bee:"} };
				string enemy = animals[randomx::Int(1, 5)];
				stats me{ 1, 1, 1 };
				int eHP = strlen(u8"▰▰▰▰▰▰▰▰▰▰"), HP = strlen(u8"▰▰▰▰▰▰▰▰▰▰"), eleft = 0, left = 0;
				dpp::message msg = dpp::message_create(event, dpp::message(dpp::channel_id(event), dpp::embed().set_title("a wild " + enemy + " appeared." + u8"᲼᲼                                  ᲼᲼\n")
					.add_field(dpp::member(event).get_user()->username + " HP:", u8"`▰▰▰▰▰▰▰▰▰▰` \nATK: " + to_string(me.ATK + Stat.at(":knife:").ATK) + " (:knife: +" + to_string(Stat.at(":knife:").ATK) + ")\nDEF: " + to_string(me.DEF + Stat.at(":knife:").DEF) + "\nSPD: " + to_string(me.SPD + Stat.at(":knife:").SPD) + " (:knife: +" + to_string(Stat.at(":knife:").SPD) + ")", true)
					.add_field(enemy + " HP:", u8"`▰▰▰▰▰▰▰▰▰▰` \nATK: " + to_string(Stat.at(enemy).ATK) + "\nDEF: " + to_string(Stat.at(enemy).DEF) + "\nSPD: " + to_string(Stat.at(enemy).SPD), true)
					.add_field(u8"᲼᲼ ᲼᲼", u8"᲼᲼ ᲼᲼")));
				for (auto& a : Stat) if (a.first == enemy)
					while (true) {
					end_turn:
						if (eleft >= 10 or left >= 10) goto end;
						sleep_for(400ms);
						string ignore = "` \nATK: " + to_string(me.ATK + Stat.at(":knife:").ATK) + " (:knife: +" + to_string(Stat.at(":knife:").ATK) + ")\nDEF: " + to_string(me.DEF + Stat.at(":knife:").DEF) + "\nSPD: " + to_string(me.SPD + Stat.at(":knife:").SPD) + " (:knife: +" + to_string(Stat.at(":knife:").SPD) + ")";
						for (auto& p : me.passive) if (p.second.turns < 1) p.second.passive = passive::none, p.second.emoji = "";
						for (auto& p : me.passive) if (p.second.passive == passive::poison) {
							HP -= strlen(u8"▰");
							for (dpp::embed& e : msg.embeds)
								e.fields[0].value.replace(
									e.fields[0].value.begin() + HP + strlen("`"),
									e.fields[0].value.end() - strlen(u8"▰") * left - strlen(ignore.c_str()), u8"▱"
								), left++;
							dpp::message_edit(event, msg);
							p.second.turns--;
						}
						if (randomx::Int(1, 13) < a.second.SPD) {
							if (a.second.SPD not_eq 0 or a.second.ATK not_eq 0) {
								for (short i = 0; i < a.second.ATK; i++) {
									if (randomx::Int(1, 20) > me.DEF + Stat.at(":knife:").DEF) i++;
									if (a.first == ":bee:" and randomx::Int(1, 5) <= 2 and me.passive.find(passive::poison) == me.passive.end()) {
										passives p;
										p.emoji = "<:poisoned:1090095037196550197>",
											p.passive = passive::poison,
											p.turns = 2;
										me.passive.emplace(passive::poison, p);
									}
									HP -= strlen(u8"▰"); string passives = "";
									for (auto& p : me.passive) if (not p.second.emoji.empty()) passives += p.second.emoji;
									for (dpp::embed& e : msg.embeds)
										e.fields[2].name = u8"᲼᲼ ᲼᲼", e.fields[2].name = passives.empty() ? u8"᲼᲼ ᲼᲼" : passives,
										e.fields[0].value.replace(
											e.fields[0].value.begin() + HP + strlen("`"),
											e.fields[0].value.end() - strlen(u8"▰") * left - strlen(ignore.c_str()), u8"▱"
										), left++;
								}
								dpp::message_edit(event, msg);
							}
							goto end_turn;
						}
						if (randomx::Int(1, 13) < me.SPD + Stat.at(":knife:").SPD) {
							for (short i = 0; i < me.ATK + Stat.at(":knife:").ATK; i++) {
								if (randomx::Int(1, 20) > a.second.DEF) i++;
								eHP -= strlen(u8"▰"); string ignore = "` \nATK: " + to_string(Stat.at(enemy).ATK) + "\nDEF: " + to_string(Stat.at(enemy).DEF) + "\nSPD: " + to_string(Stat.at(enemy).SPD);
								for (dpp::embed& e : msg.embeds)
									e.fields[1].value.replace(
										e.fields[1].value.begin() + eHP + strlen("`"),
										e.fields[1].value.end() - strlen(u8"▰") * eleft - strlen(ignore.c_str()), u8"▱"
									), eleft++;
							}
							dpp::message_edit(event, msg);
							goto end_turn;
						}
					} end:
				int dollar = randomx::Int(6, 20);
				if (left < 10) msg.content = "You killed it and got " + to_string(dollar) + " :dollar:";
				else msg.content = "You were killed :skull_crossbones:";
				dpp::message_edit(event, msg);
				data.dollars += dollar;
				for (auto& tool : data.tools) if (tool.name == ":knife:") tool.durability -= 1;
				SaveUserData(data, dpp::member(event).user_id);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.last_hunt = std::time(0);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_hunting = false;
				return true;
			}; async(hunt).wait();
		}
	return true;
}
vector<thread> slashcommands_executed;
inline void await_on_slashcommand(const dpp::slashcommand_t& event) {
	UserData data = GetUserData(event.command.member.user_id);
	if (data.failed) async(new_user, event.command.member.user_id).wait();
	if (event.command.get_command_name() == "daily") async(daily_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "daily") async(daily_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "profile") async(profile_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "shop") async(shop_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "buy") async(buy_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "sell") async(sell_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "fish") async(fish_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "repair") async(repair_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "leaderboard" or event.command.get_command_name() == "top") async(leaderboard_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "purge") async(purge_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "membercount") async(membercount_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "avatar") async(avatar_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "invite") async(invite_t<const dpp::slashcommand_t&>, event);
	else if (event.command.get_command_name() == "hunt") async(hunt_t<const dpp::slashcommand_t&>, event);
	else return;
	{
		UserData data = GetUserData(event.command.member.user_id);
		data.last_on = std::time(0);
		SaveUserData(data, event.command.member.user_id);
	}
}
vector<thread> commands_executed;
inline void await_on_message_create(const dpp::message_create_t& event) {
	UserData data = GetUserData(event.msg.member.user_id);
	if (data.failed) async(new_user, event.msg.member.user_id).wait();
	GuildData g_data = GetGuildData(event.msg.guild_id);
	if (event.msg.content.find(g_data.prefix + "prefix ") not_eq -1) async(prefix_t, event);
	else if (event.msg.content.find(g_data.prefix + "daily") not_eq -1) async(daily_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "profile ") not_eq -1) async(profile_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "shop") not_eq -1) async(shop_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "buy ") not_eq -1) async(buy_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "sell ") not_eq -1) async(sell_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "fish") not_eq -1) async(fish_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "repair ") not_eq -1) async(repair_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "leaderboard") not_eq -1 or event.msg.content.find(g_data.prefix + "top") not_eq -1) async(leaderboard_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "purge ") not_eq -1) async(purge_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "membercount") not_eq -1) async(membercount_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "avatar ") not_eq -1 or event.msg.content.find(g_data.prefix + "avatar") not_eq -1) async(avatar_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "invite") not_eq -1) async(invite_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "hunt") not_eq -1) async(hunt_t<const dpp::message_create_t&>, event);
	else return;
	{
		UserData data = GetUserData(event.msg.member.user_id);
		data.last_on = std::time(0);
		SaveUserData(data, event.msg.member.user_id);
	}
}
vector<thread> register_slashcommands;
void update_slashcommands(command sent, bool remove = false)
{
	switch (sent) {
	case command::daily: {
		dpp::slashcommand daily = dpp::slashcommand()
			.set_name("daily")
			.set_description("get a small gift from niko once a day")
			.set_application_id(bot.me.id);
		daily = bot.global_command_create_sync(daily);
		if (remove) bot.global_command_delete_sync(daily.id);
		break;
	}
	case command::profile: {
		dpp::slashcommand profile = dpp::slashcommand()
			.set_name("profile")
			.set_description("your profile")
			.add_option(dpp::command_option(dpp::co_string, "name", "mention user", true))
			.set_application_id(bot.me.id);
		profile = bot.global_command_create_sync(profile);
		if (remove) bot.global_command_delete_sync(profile.id);
		break;
	}
	case command::shop: {
		dpp::slashcommand shop = dpp::slashcommand()
			.set_name("shop")
			.set_description("view the shop")
			.set_application_id(bot.me.id);
		shop = bot.global_command_create_sync(shop);
		if (remove) bot.global_command_delete_sync(shop.id);
		break;
	}
	case command::buy: {
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
	case command::sell: {
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
	case command::fish: {
		dpp::slashcommand fish = dpp::slashcommand()
			.set_name("fish")
			.set_description("go fishing")
			.set_application_id(bot.me.id);
		fish = bot.global_command_create_sync(fish);
		if (remove) bot.global_command_delete_sync(fish.id);
		break;
	}
	case command::repair: {
		dpp::slashcommand repair = dpp::slashcommand()
			.set_name("repair")
			.set_description("the item you wanna repair/fix")
			.add_option(dpp::command_option(dpp::co_string, "id", "the item id", true))
			.set_application_id(bot.me.id);
		repair = bot.global_command_create_sync(repair);
		if (remove) bot.global_command_delete_sync(repair.id);
		break;
	}
	case command::leaderboard: {
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
	case command::purge: {
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
	case command::membercount: {
		dpp::slashcommand membercount = dpp::slashcommand()
			.set_name("membercount")
			.set_description("view all members in server")
			.set_application_id(bot.me.id);
		membercount = bot.global_command_create_sync(membercount);
		if (remove) bot.global_command_delete_sync(membercount.id);
		break;
	}
	case command::avatar: {
		dpp::slashcommand avatar = dpp::slashcommand()
			.set_name("avatar")
			.set_description("view someone's avatar")
			.add_option(dpp::command_option(dpp::co_string, "name", "person's avatar you wanna view, empty if yourself.", false))
			.set_application_id(bot.me.id);
		avatar = bot.global_command_create_sync(avatar);
		if (remove) bot.global_command_delete_sync(avatar.id);
		break;
	}
	case command::invite: {
		dpp::slashcommand invite = dpp::slashcommand()
			.set_name("invite")
			.set_description("invite " + bot.me.username + " to your server")
			.set_application_id(bot.me.id);
		invite = bot.global_command_create_sync(invite);
		if (remove) bot.global_command_delete_sync(invite.id);
		break;
	}
	case command::hunt: {
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
	if (version < 1) async(update_slashcommands, command::daily, remove).wait();
	if (version < 2) async(update_slashcommands, command::profile, remove).wait();
	if (version < 3) async(update_slashcommands, command::shop, remove).wait();
	if (version < 4) async(update_slashcommands, command::buy, remove).wait();
	if (version < 5) async(update_slashcommands, command::sell, remove).wait();
	if (version < 6) async(update_slashcommands, command::fish, remove).wait();
	if (version < 7) async(update_slashcommands, command::repair, remove).wait();
	if (version < 8) async(update_slashcommands, command::leaderboard, remove).wait();
	if (version < 9) async(update_slashcommands, command::purge, remove).wait();
	if (version < 10) async(update_slashcommands, command::membercount, remove).wait();
	if (version < 11) async(update_slashcommands, command::avatar, remove).wait();
	if (version < 12) async(update_slashcommands, command::invite, remove).wait();
	if (version < 13) async(update_slashcommands, command::hunt, remove).wait();
	ofstream("SLASHCOMMAND_VERSION") << SLASHCOMMAND_VERSION;
	print<string>({
		bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ",
		"updated ", to_string(stoi(SLASHCOMMAND_VERSION) - version), " slashcommand", stoi(SLASHCOMMAND_VERSION) - version <= 1 ? "" : "s"
		}, nullptr, state{ newline, color::white });
}