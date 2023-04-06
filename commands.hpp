#pragma once

inline void await_on_button_click(const dpp::button_click_t& event) {
	UserData data = GetUserData(event.command.member.user_id);
	vector<string> index = dpp::index(event.custom_id, '_');
	/* alright, here's a quick recap, so basically "1" "3", ect are ID(s) which they use to be, now it's the emoji as the id */
	if (event.custom_id.find("repair_1") not_eq -1 and index[2] == to_string(event.command.member.user_id)/*make sure it's the requester who's pressing the button*/) {
		if (data.dollars > 12) { // -> check if they got 12 or more dollars
			data.dollars -= 12; // -> if so then take the 12 dollars, we don't want nagative values.
			/* if they got a broken tool, then repair it else just reset to 15 durability */
			for (auto& tool : data.tools) if (tool.name == "B:fishing_pole_and_fish:") tool.name = ":fishing_pole_and_fish:", tool.durability = 15;
			event.reply(dpp::message(event.command.channel_id, dpp::embed().set_color(dpp::colors::success).set_description("Repair Complete!")));
			SaveUserData(data, event.command.member.user_id);
		}
		else event.reply(dpp::message(event.command.channel_id, dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!")));
	}
	if (event.custom_id.find("repair_3") not_eq -1 and index[2] == to_string(event.command.member.user_id)) {
		if (data.dollars > 66) { // -> check if they got 66 or more dollars
			data.dollars -= 66; // -> if so then take the 66 dollars, we don't want nagative values.
			/* if they got a broken tool, then repair it else just reset to 15 durability */
			for (auto& tool : data.tools) if (tool.name == "B:knife:") tool.name = ":knife:", tool.durability = 15;
			event.reply(dpp::message(event.command.channel_id, dpp::embed().set_color(dpp::colors::success).set_description("Repair Complete!")));
			SaveUserData(data, event.command.member.user_id);
		}
		else event.reply(dpp::message(event.command.channel_id, dpp::embed().set_color(dpp::colors::failed).set_description("You don't have enough to repair it!")));
	}
}
bool prefix_t(const dpp::message_create_t& event)
{
	vector<string> index = dpp::index(event.msg.content, ' ');
	GuildData g_data = GetGuildData(event.msg.guild_id);
	/* if the prefix size is null. Alternativly; dpp::empty_index() */
	if (index[1].size() > 1 or index[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
	else {
		g_data.prefix = index[1]; // -> set the prefix as the first space inside the prefix command
		SaveGuildData(g_data, event.msg.guild_id);
		event.reply("Prefix is now set to: **" + g_data.prefix + "**");
	}
	return true;
}
template<typename event_t> bool daily_t(event_t event)
{
	UserData data = GetUserData(dpp::member(event).user_id);
	tm* claimed = dpp::utility::mtm(data.daily); // -> make daily a tm* for later manipulation
	/* manipulate daily's container (claimed) to set for tomorrow */
	time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour, claimed->tm_wday += 1, claimed->tm_mday += 1, claimed->tm_mon);
	if (ct < time(0)) { // -> if ct (described above) is 1 day or older then proceed, else tell them how long they got to wait below
		uint64_t dollar = randomx::Int(30, 92); // -> random uint64_t from 30-92 (using <random> so srand() isn't nessesary)
		data.daily = time(0); // -> reset the cycle by setting the date to today.
		data.dollars += dollar; // -> give them the dollars
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS)
			.set_title("Thanks for opening my gift! :tada:")
			.set_description("- " + to_string(dollar) + " :dollar:")));
		SaveUserData(data, dpp::member(event).user_id);
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
	if (has_char(username(name))) return false; // -> make sure it was a mention or an array of numbers
	UserData data = GetUserData(stoull(username(name)));
	if (data.failed) new_user(stoull(username(name))); // -> make some filler information if they never used the bot before
	string sort = "";
	/* check what tools they got, also if there broken or not */
	for (auto& tool : data.tools) if (not tool.name.empty()) sort += (tool.name.find("B:") or tool.durability < 1 ? "Broken " + tool.name : "Durability: " + to_string(tool.durability) + " " + tool.name);
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS)
		.set_title(":mag_right: Profile Viewer")
		.set_description(("**<@" + username(name) + ">** ") + (data.last_on == 0 ? "inactive" : "last online " + dpp::utility::timestamp(data.last_on, dpp::utility::tf_relative_time)))
		.add_field(
			"Tools: ",
			sort.empty() ? "None" : sort)
		.add_field(
			"Inventory: ",
			to_string(data.dollars) + " :dollar: \n" +
			(data.fish > 0 ? to_string(data.fish) + " :fish: \n" : ""))));
	return true;
}
template<typename event_t> bool shop_t(event_t event)
{
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS)
		.set_title(":shopping_cart: Shop")
		.set_description(
			"`[ID: 1]` **:fishing_pole_and_fish: Wooden Fishing Rod**:\n  - Durability: 15/15\n  - Cost: 15 :dollar:\n\n\
             `[ID: 3]` **:knife: Knife**:\n  - Durability: 15/15\n  - Cost: 80 :dollar:")
		.add_field(
			"How to Buy?",
			"type !buy {ID} {Amount}")));
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
		for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") found = true; // -> self explained by the var name, it's checking if you got the tool alr
		int forecast = 15 * stoull(amount); // -> forsee if you can afford the amount
		if (data.dollars < forecast) event.reply("You can't afford it.");
		else if (found or stoi(amount) > 1) event.reply("You can only purchase one!"); // -> this part was talked about above
		else {
			data.dollars = data.dollars -= forecast; // -> take away the forsawed amount of dollars
			tools buf; // -> create a temp tool and store it inside the user's tools
			buf.name = ":fishing_pole_and_fish:", buf.durability = 15; // -> set the name to the emoji
			data.tools.emplace_back(buf); // -> emplace back it, the 'buf' will be deleted on it's own, I swear if I see a mem function I'll throw you out the window
			SaveUserData(data, dpp::member(event).user_id);
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::success).set_description("You've Purchased a :fishing_pole_and_fish:")));
		}
	}
	else if (id == "3" or id == "knife") {
		bool found = false;
		UserData data = GetUserData(dpp::member(event).user_id);
		for (auto& tool : data.tools) if (tool.name == ":knife:") found = true; // -> self explained by the var name, it's checking if you got the tool alr
		int forecast = 80 * stoull(amount); // -> forsee if you can afford the amount
		if (data.dollars < forecast) event.reply("You can't afford it.");
		else if (found or stoi(amount) > 1) event.reply("You can only purchase one!"); // -> this part was talked about above
		else {
			data.dollars = data.dollars -= forecast; // -> take away the forsawed amount of dollars
			tools buf; // -> create a temp tool and store it inside the user's tools
			buf.name = ":knife:", buf.durability = 15; // -> set the name to the emoji
			data.tools.emplace_back(buf); // -> emplace back it, the 'buf' will be deleted on it's own, I swear if I see a mem function I'll throw you out the window
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
		data.dollars += 2 * stoull(amount); // -> *deep exhale* ok so basically this gives you dollars for each fish, 2 is the amount for 1 fish.
		data.fish = data.fish - stoi(amount); // -> takes away the fish you wanted to sell. sell 5 fish -> take 5 fish -> give 10 dollars.
		SaveUserData(data, dpp::member(event).user_id);
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("You sold " + amount + " :fish:, and got " + to_string(2 * stoull(amount)) + " :dollar:")));
	}
	else event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("Invalid ID")));
	return true;
}
/* realizing commentary shouldn't be free.... */
template<typename event_t> bool fish_t(event_t event)
{
	bool found = false;
	UserData data = GetUserData(dpp::member(event).user_id);
	for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") found = true; // -> explained a dozen times, checks if you already got tool
	for (auto& find : members) if (find.first == dpp::member(event).user_id) // -> we use map data to modify there state on fishing, feteched during startup
		if (find.second.busy_fishing) return false; // -> this is here to prevent spamming, but soon ratelimit will be added
		else if (find.second.last_fish + 10 > time(0)) { // -> okok so adding 10 to a time_t is basically adding 10 seconds (we ignore ms)
			if (find.second.once_fishing) return false; // -> another anti spammeroni
			find.second.once_fishing = true;
			tm* mt = dpp::utility::mtm(find.second.last_fish); // -> more time manipulation, explained at daily_t, just read that
			time_t ct = dpp::utility::mt_t(mt, mt->tm_sec += 12, mt->tm_min, mt->tm_hour, mt->tm_wday, mt->tm_mday, mt->tm_mon); // -> OK! so this is 12 cause of delays on deletion
			dpp::message msg = dpp::message_create(event, dpp::message(dpp::channel_id(event), dpp::member(event).get_user()->username + ". You can fish again **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**"));
			while (true) {
				if (find.second.last_fish + 10 < time(0)) { // -> if it reached the 10 second marker, this will be looped on a seperate thread so no worries
					dpp::message_delete(event, msg.id, dpp::channel_id(event));
					find.second.once_fishing = false;
					goto escape; // -> leave and get caught up with return value
				}
			}
		escape: return false; // -> return false cause it was a failed attempted to succeed in command, if we don't return then it'll act on (sure that might sound cool but idk LOL)
		}
	/* OMG you probably thought I forgot about found lol */
		else if (not found) event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("You don't have a fishing rod.")));
		else {
			function<void()> fishing = [&]() { // -> this was useful but idk if anymore sense it's all threaded
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
				sleep_for(chrono::seconds(randomx::Int(4, 9)) // -> sleeps for a random amount of time, this adds realizsm to fishing
				for (dpp::embed& e : msg.embeds) e.description = "You caught 1 :fish:!"; // -> gets the embed from previous sent msg and changes description
				dpp::message_edit(event, msg); // -> edits the message from changed embed description
				data.fish += 1;
				for (auto& tool : data.tools) if (tool.name == ":fishing_pole_and_fish:") tool.durability -= 1;
				SaveUserData(data, dpp::member(event).user_id);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.last_fish = time(0);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_fishing = false;
				return true; // -> returns here cause why not.
			}; async(fishing);
		}
	return true;
}
template<typename event_t> bool repair_t(event_t event)
{
	string id = "", value = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) id = dpp::index(event, "id");
	else id = dpp::index(event, "1");
	switch (stoi(id)) { // -> gets values according to the id, this might be changed in the future
	case 1: value = to_string(12);
	case 3: value = to_string(66);
	}
	if (value.empty()) return false;
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_title(":hammer_pick: Repair").add_field(
		"Repairing this will cost :dollar: " + value,
		"Are You Sure?")).add_component( // -> this is kinda like a array of components
			dpp::component().add_component(dpp::component() // -> THE component
				.set_label("Repair") // -> the text displayed on the button
				.set_type(dpp::cot_button) // -> given: component is a button
				.set_style(dpp::cos_success) // -> button coloring/design
				.set_id("repair_" + id + "_" + to_string(dpp::member(event).user_id))) // -> ID for clicking on it
		));
	return true;
}
template<typename event_t> bool leaderboard_t(event_t event)
{
	if (members.empty()) return false; // -> we don't wanna make the embed empty
	string oriented = ""; // -> self explained by the name
	vector<pair<uint64_t, uint64_t>> buffer; // -> temp place to store, I think this was useless before I removed "sort" in creating the map
	for (auto& member : members) buffer.emplace_back(member.second.dollars, member.second.user_id); // -> put all the members inside the buffer
	sort(buffer.begin(), buffer.end(), first<uint64_t, uint64_t>()); // -> sort the buffer -I mean the members
	for (auto& i : buffer) { // -> now store the results of sorted members to display in order by higher to lower
		if (i.second == 0) continue; // -> this was a old bug, but for stability it's kept here
		oriented += "**" + bot.user_get_sync(i.second).username + "**: " + to_string(i.first) + " :dollar: \n"; // -> put inside the string
	}
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_title("Leaderboard: ").set_description(oriented)));
	return true;
}
template<typename event_t> bool purge_t(event_t event)
{
	string amount = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) amount = dpp::index(event, "amount");
	else amount = dpp::index(event, "1");
	if (has_char(amount)) return false; // -> we are seeking a number
	if (stoi(amount) <= 1 or stoi(amount) > 100) return false; // -> allow only 1-100 messages to be deleted per session
	else {
		int deleted = 0; // -> ignore the extra amount requested. lets say you wanna purge 50 messages but channel only contains 10, it'll result as 10 not 50
		auto msgs = bot.messages_get_sync(dpp::channel_id(event), 0, 0, 0, stoi(amount)); // -> get the messages in the channel it was requested
		vector<dpp::snowflake> ids, oids; // -> ids is before 14 days, oids is older then 14 days
		if (msgs.size() <= 1) return false; // -> don't proceed if no messages are there (there as in the channel)
		for (auto& msg : msgs) { // -> loop the messages and put it in the vector (ids or oids)
			if (msg.second.author.username.empty()) continue; // -> ignore webhooks
			deleted++;
			tm* creation_time = dpp::utility::mtm(msg.second.sent); // -> the message age
			tm* now = dpp::utility::mtm(time(0)); // -> the current time in tm* form
			if (now->tm_mday < creation_time->tm_mday) { // -> if the message is in this month
				if (creation_time->tm_mday - now->tm_mday > 14 and creation_time->tm_mon not_eq now->tm_mon or // -> take away each other dates and seeing if greater then 14
					creation_time->tm_mday - now->tm_mday < 14 and creation_time->tm_mon not_eq now->tm_mon) { // -> tbh I don't know what I was thinking here, something todo with yearly
					oids.emplace_back(msg.second.id);
					continue;
				}
			} // -> why add a else? cause we wanna check it during a differant month (message age). above checked the current month
			else if (now->tm_mday - creation_time->tm_mday > 14 and creation_time->tm_mon not_eq now->tm_mon or // -> take away each other dates and seeing if greater then 14
				creation_time->tm_mday - now->tm_mday < 14 and creation_time->tm_mon not_eq now->tm_mon) { // -> tbh I don't know what I was thinking here, something todo with yearly
				oids.emplace_back(msg.second.id);
				continue;
			}
			ids.emplace_back(msg.second.id); // -> ok so this message is obviously before 14 days
		}
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_description("Deleted `" + to_string(deleted) + "` Message(s)")));
		function<void()> mass_delete = [&]() {
			for (auto& msg : oids) bot.message_delete(msg, dpp::channel_id(event)), sleep_for(500ms); // -> delete them by the bot not bulked
		};
		if (not ids.empty()) bot.message_delete_bulk_sync(ids, dpp::channel_id(event)); // -> bulk delete the messages before 14 days of age
		if (not oids.empty()) async(mass_delete); // -> delete them by the bot not bulked (API policy, don't modify)
	}
	return true;
}
template<typename event_t> bool membercount_t(event_t event)
{
	uint64_t membercount = 0, after = 0;
	while (true) { // -> loop if after isn't null
		for (auto& members : bot.guild_get_members_sync(dpp::guild_id(event), 1000, after)) { // -> loop captured members within guild, not 1000 sense were while looping it
			membercount++; // -> ok we got a member so add 1
			if (membercount >= 1000) after = members.second.user_id; else after = 0; // if the 1000 marker succeeds then continue another 1000
			// -> why? cause it's stored in a map with 1000 being a limit for stored values
		}
		if (after == 0) goto leave; // -> cool so it's less then 1000 so lets just move on instead of looping forever
	} leave: // -> escape point
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS)
		.set_description("> " + bot.guild_get_sync(dpp::guild_id(event)).name + " (**" + to_string(membercount) + "**)")
		.set_timestamp(time(0)))); // -> get the time for capturing milestones, even though the message has a displayed date... 
	return true;
}
template<typename event_t> bool avatar_t(event_t event)
{
	string url = ""; dpp::snowflake mention = 0;

	if (is_same_v<decltype(event), const dpp::slashcommand_t&> ? dpp::empty_index(event, "name") : dpp::empty_index(event, "1")) 
		url = dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png); // -> we want the requesters avatar if empty
	else {
		string name = "";
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name");
		else name = dpp::index(event, "1");
		if (has_char(username(name))) return false; // -> checks if it's a valid ID (only numbers)
		url = bot.user_get_sync(stoull(username(name))).get_avatar_url(256, dpp::image_type::i_png), mention = stoull(username(name));
	}
	event.reply(dpp::message(dpp::guild_id(event), dpp::embed()
		.set_color(dpp::colors::PS)
		.set_description(("<@") + (url == dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(dpp::member(event).user_id) : to_string(mention)) + (">'s Avatar"))
		.set_image(url)));
	return true;
}
template<typename event_t> bool invite_t(event_t event)
{
	/* sense this is a open source we wanna make a invite not send mine. so we wanna make sure it's YOUR bot token, no modifications are needed */
	event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::PS).set_author("Click me!", "https://discord.com/api/oauth2/authorize?client_id=" +
		to_string(bot.me.id) + "&permissions=" + to_string(dpp::permissions::p_administrator) + "&scope=bot%20applications.commands", "")));
	return true;
}
/* ok so this one will take years to add comments, so I'll simplify it */
/* if your curious why it's so long and complex is because were working with a 3 byte size string instead of 1 */
/* the ▰ symbol is considered 3 in size. so we must do extra stuff to make sure it knows how to replace values in "replace()" function */
/* NOTE: sense it's a u8 string and longer then 1 byte we can't replace each char instead we must replace the whole string. updated comments will be added later */
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
									e.fields[0].value.end() - strlen(u8"▰") * left - strlen(ignore.c_str()), u8"▱"), left++;
							dpp::message_edit(event, msg);
							p.second.turns--;
						}
						if (randomx::Int(1, 13) < a.second.SPD) {
							if (a.second.SPD not_eq 0 or a.second.ATK not_eq 0) {
								for (short i = 0; i < a.second.ATK; i++) {
									if (randomx::Int(1, 20) > me.DEF + Stat.at(":knife:").DEF) i++;
									if (a.first == ":bee:" and randomx::Int(1, 5) <= 2 and me.passive.find(passive::poison) == me.passive.end()) {
										passives p;
										p.emoji = "<:poisoned:1090095037196550197>", p.passive = passive::poison, p.turns = 2;
										me.passive.emplace(passive::poison, p);
									}
									HP -= strlen(u8"▰"); string passives = "";
									for (auto& p : me.passive) if (not p.second.emoji.empty()) passives += p.second.emoji;
									for (dpp::embed& e : msg.embeds)
										e.fields[2].name = u8"᲼᲼ ᲼᲼", e.fields[2].name = passives.empty() ? u8"᲼᲼ ᲼᲼" : passives,
										e.fields[0].value.replace(
											e.fields[0].value.begin() + HP + strlen("`"),
											e.fields[0].value.end() - strlen(u8"▰") * left - strlen(ignore.c_str()), u8"▱"), left++;
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
										e.fields[1].value.end() - strlen(u8"▰") * eleft - strlen(ignore.c_str()), u8"▱"), eleft++;
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
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.last_hunt = time(0);
				for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.busy_hunting = false;
				return true;
			}; async(hunt).wait();
		}
	return true;
}
template<typename event_t> bool nick_t(event_t event) {
	string name = "", nickname = "";
	try {
		is_same_v<decltype(event), const dpp::slashcommand_t&> ?
			name = dpp::index(event, "name"), nickname = dpp::index(event, "nickname") :
			name = dpp::index(event, "1"), nickname = dpp::index(event, "2");
		if (nickname.size() < 1 or nickname.size() > 32) { // -> discord API policy, keep it here and don't modify; https://github.com/discord/discord-api-docs/blob/main/docs/resources/User.md#usernames-and-nicknames
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed).set_description("> Nickname must contain 1-32 characters")
				.set_footer(dpp::embed_footer().set_text("length: " + to_string(nickname.size())))).set_flags(dpp::message_flags::m_ephemeral));
			return false;
		}
		if (has_char(username(name))) return false;
		dpp::guild_member gm = bot.guild_get_member_sync(dpp::guild_id(event), stoull(username(name)));
		gm.nickname = nickname;
		bot.guild_edit_member_sync(gm);
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::success)
			.set_description("> <@" + username(name) /* can't assume it's a mention */ + "> name changed to **" + nickname + "**")));
	}
	catch (dpp::exception e) {
		if (not e.msg.empty()) event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
			.set_description("> Can't modify <@" + username(name) + ">'s nickname").set_footer(dpp::embed_footer().set_text(e.msg))));
	}
	return true;
}
template<typename event_t> thread queue_ratelimit(event_t event) {
	cout << "queue ratelimit" << endl;
	for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.queue = 1, sleep_for(3s), find.second.ratelimit = 0, find.second.queue = 0;
	cout << "queue ended" << endl;
	return thread(); // -> were done here
}
inline void await_on_slashcommand(const dpp::slashcommand_t& event) {
	for (auto& find : members) if (find.first == event.command.member.user_id) if (find.second.queue == 1) return; // -> we don't want extra detached threads
	while (true) { // -> looping is underrated here but it's my code, my rules.
		for (auto& find : members) if (find.first == event.command.member.user_id)
			if (find.second.ratelimit < 4) goto proceed;
			else thread(queue_ratelimit<const dpp::slashcommand_t&>, event).detach();
		if (bot.rest_ping < 0.6) goto proceed; // -> higher rest ping the more overworked the bot is (overwork as in HTTPS requesting)
	} proceed:
	for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.ratelimit++;
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
	else if (event.command.get_command_name() == "nick") async(nick_t<const dpp::slashcommand_t&>, event);
	else return;
	{ // -> scope and call GetUserData() again cause we don't wanna overwrite anything, good time to store XP/level system here.
		UserData data = GetUserData(event.command.member.user_id);
		data.last_on = time(0);
		SaveUserData(data, event.command.member.user_id);
	}
}
vector<thread> commands_executed;
inline void await_on_message_create(const dpp::message_create_t& event) {
	for (auto& find : members) if (find.first == event.msg.member.user_id) if (find.second.queue == 1) return;  // -> we don't want extra detached threads
	while (true) { // -> looping is underrated here but it's my code, my rules.
		for (auto& find : members) if (find.first == event.msg.member.user_id)
			if (find.second.ratelimit < 3) goto proceed;
			else thread(queue_ratelimit<const dpp::message_create_t&>, event).detach();
		if (bot.rest_ping < 0.6) goto proceed; // -> higher rest ping the more overworked the bot is (overwork as in HTTPS requesting)
	} proceed:
	for (auto& find : members) if (find.first == dpp::member(event).user_id) find.second.ratelimit++;
	UserData data = GetUserData(event.msg.member.user_id);
	if (data.failed) async(new_user, event.msg.member.user_id).wait();
	GuildData g_data = GetGuildData(event.msg.guild_id);
	if (event.msg.content.find(g_data.prefix + "prefix ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_administrator) async(prefix_t, event);
	else if (event.msg.content.find(g_data.prefix + "daily") not_eq -1) async(daily_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "profile ") not_eq -1) async(profile_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "shop") not_eq -1) async(shop_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "buy ") not_eq -1) async(buy_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "sell ") not_eq -1) async(sell_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "fish") not_eq -1) async(fish_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "repair ") not_eq -1) async(repair_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "leaderboard") not_eq -1 or event.msg.content.find(g_data.prefix + "top") not_eq -1) async(leaderboard_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "purge ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_administrator) async(purge_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "membercount") not_eq -1) async(membercount_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "avatar ") not_eq -1 or event.msg.content.find(g_data.prefix + "avatar") not_eq -1) async(avatar_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "invite") not_eq -1) async(invite_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "hunt") not_eq -1) async(hunt_t<const dpp::message_create_t&>, event);
	else if (event.msg.content.find(g_data.prefix + "nick ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_manage_nicknames) async(nick_t<const dpp::message_create_t&>, event);
	else {
		if (event.msg.content.find(g_data.prefix + "nick ") not_eq -1)
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
				.set_description("> Sorry, you need **manage nicknames** permission to preform this command")).set_flags(dpp::message_flags::m_ephemeral));
		else if (event.msg.content.find(g_data.prefix + "purge ") not_eq -1 or event.msg.content.find(g_data.prefix + "prefix ") not_eq -1)
			event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
				.set_description("> Sorry, you need **administrator** permission to preform this command")).set_flags(dpp::message_flags::m_ephemeral));
		return;
	}
	{ // -> scope and call GetUserData() again cause we don't wanna overwrite anything, good time to store XP/level system here.
		UserData data = GetUserData(event.msg.member.user_id);
		data.last_on = time(0);
		SaveUserData(data, event.msg.member.user_id);
	}
}
void load_slashcommands()
{
	class option {
	public:
		dpp::command_option_type v_type;
		string name, description;
		bool required = false;
	};
	class about {
	public:
		string name, description;
		uint64_t permissions;
		vector<option> options;
	};
	vector<about> commands = {
		about{"daily", "get a small gift from " + bot.me.username + " once a day", dpp::permissions::p_send_messages},
		about{"profile", "your profile", dpp::permissions::p_send_messages, vector<option>{option{dpp::command_option_type::co_string, "name", "mention user", true}}},
		about{"shop", "view the shop", dpp::permissions::p_send_messages},
		about{"buy", "buy an item from shop", dpp::permissions::p_send_messages, vector<option>{option{dpp::command_option_type::co_string, "id", "the item id", true}, option{dpp::command_option_type::co_string, "amount", "the amount of the item you wanna buy", true}}},
		about{"sell", "sell an item", dpp::permissions::p_send_messages, vector<option>{option{dpp::command_option_type::co_string, "id", "the item id", true}, option{dpp::command_option_type::co_string, "amount", "the amount of the item you wanna sell", true}}},
		about{"fish", "go fishing", dpp::permissions::p_send_messages},
		about{"repair", "the item you wanna repair", dpp::permissions::p_send_messages, vector<option>{option{dpp::command_option_type::co_string, "id", "the item id", true}}},
		about{"leaderboard", "see top players", dpp::permissions::p_send_messages}, about{"top", "see top players", dpp::permissions::p_send_messages},
		about{"purge", "mass delete messages in a channel", dpp::permissions::p_administrator, vector<option>{option{dpp::command_option_type::co_string, "amount", "amount of messages to which be deleted", true}}},
		about{"membercount", "view all members in server", dpp::permissions::p_send_messages},
		about{"avatar", "view someone's avatar", dpp::permissions::p_send_messages, vector<option>{option{dpp::command_option_type::co_string, "name", "person's avatar you wanna view. Empty if yourself.", false}}},
		about{"invite", "invite " + bot.me.username + " to your server", dpp::permissions::p_send_messages},
		about{"hunt", "hunt down a animal", dpp::permissions::p_send_messages},
		about{"nick", "change someone's nickname or yourself", dpp::permissions::p_manage_nicknames, vector<option>{option{dpp::command_option_type::co_string, "name", "the person you wanna change", false}, option{dpp::command_option_type::co_string, "nickname", "the nickname it'll change too", false}}},
	};
	vector<dpp::slashcommand> slashcommand;
	for (auto& command : commands) {
		dpp::slashcommand cmd = dpp::slashcommand()
			.set_name(command.name)
			.set_description(command.description)
			.set_default_permissions(command.permissions)
			.set_application_id(bot.me.id);
		if (not command.options.empty())
			for (auto& option : command.options)
				cmd.add_option(dpp::command_option(option.v_type, option.name, option.description, option.required));
		slashcommand.emplace_back(cmd);
	}
	sleep_for(10s); // -> avoid ratelimit, sense it's bulked the ratelimit is quite longer
	dpp::slashcommand_map results = bot.global_bulk_command_create_sync(slashcommand);
	if (results.size() == 15) bot.log(dpp::loglevel::ll_trace, "Successfully added all slashcommands"); // -> just saying all the commands had no problem being added
}