/* Copyright(c) LeeEndl; License Apache License 2.0 */
#pragma once

inline void await_on_button_click(const dpp::button_click_t& event) {
	UserData data = GetUserData(event.command.member.user_id);
	vector<string> index = dpp::index(event.custom_id, '_');
	if (event.custom_id.find("bjd_") not_eq -1 and index[1] == to_string(event.command.member.user_id))
		for (auto& games : bj_callback) {
			games.second.msg.embeds[0].description = "<@" + index[1] + "> has declined your invitation";
			games.second.msg.embeds[0].color = dpp::colors::failed;
			games.second.msg.components.clear();
			if (games.first.first == index[1]) bot.message_edit_sync(games.second.msg);
			bj_callback.erase(games.first);
		}
	if (event.custom_id.find("bja_") not_eq -1 and index[1] == to_string(event.command.member.user_id))
		for (auto& games : bj_callback) {
			if (games.first.first == index[1]) {
				UserData data = GetUserData(stoull(games.first.second));
				data.dollars -= games.second.bet;
				SaveUserData(data, stoull(games.first.second));
				games.second.msg.embeds[0].description = "<@" + index[1] + "> has accepted your invitation";
				games.second.msg.embeds[0].color = dpp::colors::success;
				games.second.msg.components.clear();
				bot.message_edit_sync(games.second.msg);
				{ randomx draw = randomx().i32(1, 52); games.second.p1.emplace(make_pair(0, cards[draw.val32].second), make_pair(card_back, cards[draw.val32].first)); }
				{ randomx draw = randomx().i32(1, 52); games.second.p1.emplace(make_pair(cards[draw.val32].second, cards[draw.val32].second), make_pair(cards[draw.val32].first, cards[draw.val32].first)); }
				for (auto& deck : games.second.p1) games.second.p1_deck += deck.second.first + " ", games.second.p1_value += deck.first.first;
				for (auto& deck : games.second.p1) games.second.p1POV_deck += deck.second.second + " ", games.second.p1POV_value += deck.first.second;

				{ randomx draw = randomx().i32(1, 52); games.second.p2.emplace(cards[draw.val32].second, cards[draw.val32].first); }
				{ randomx draw = randomx().i32(1, 52); games.second.p2.emplace(cards[draw.val32].second, cards[draw.val32].first); }
				for (auto& deck : games.second.p2) games.second.p2_deck += deck.second + " ", games.second.p2_value += deck.first;

				if (games.second.p2_value > 21 or games.second.p1POV_value == 21) {
					dpp::message msg = dpp::message(games.second.msg.channel_id, dpp::embed()
						.set_color(dpp::colors::success)
						.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
						.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1POV_deck + " [**" + to_string(games.second.p1POV_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]")
						.set_footer(dpp::embed_footer().set_text(GetUserData(stoull(games.first.second)).username + " won " + to_string(games.second.bet * 2) + u8" 💵")));
					event.reply(msg);
					UserData data = GetUserData(stoull(games.first.second));
					data.dollars += games.second.bet;
					SaveUserData(data, stoull(games.first.second));
					bj_callback.erase(games.first);
					return;
				}
				if (games.second.p1POV_value > 21 or games.second.p2_value == 21) {
					dpp::message msg = dpp::message(games.second.msg.channel_id, dpp::embed()
						.set_color(dpp::colors::success)
						.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
						.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1POV_deck + " [**" + to_string(games.second.p1POV_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]")
						.set_footer(dpp::embed_footer().set_text(GetUserData(stoull(games.first.first)).username + " won " + to_string(games.second.bet * 2) + u8" 💵")));
					event.reply(msg);
					UserData data = GetUserData(stoull(games.first.first));
					data.dollars += games.second.bet * 2;
					SaveUserData(data, stoull(games.first.first));
					bj_callback.erase(games.first);
					return;
				}

				dpp::message msg = bot.message_create_sync(dpp::message(games.second.msg.channel_id, dpp::embed()
					.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
					.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1_deck + " [**" + to_string(games.second.p1_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]")));

				games.second.turn = 2;
				msg.embeds[0].set_footer(dpp::embed_footer().set_text((games.second.turn == 1 ? GetUserData(stoull(games.first.second)).username : GetUserData(stoull(games.first.first)).username) + (" turn")));
				msg.add_component(dpp::component()
					.add_component(dpp::component()
						.set_emoji(u8"👊")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id("bjhit_" + games.first.first))
					.add_component(dpp::component()
						.set_emoji(u8"🛑")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id("bjstand_" + games.first.first))
					.add_component(dpp::component()
						.set_emoji("card_back", 1105985144147415080)
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id("bjview_" + games.first.second)));
				bot.message_edit_sync(msg);
				games.second.msg = msg;
			}
			break;
		}
	if (event.custom_id.find("bjview_") not_eq -1 and index[1] == to_string(event.command.member.user_id))
		for (auto& games : bj_callback)
			if (games.first.second == index[1]) event.reply(dpp::message(event.command.channel_id, games.second.p1POV_deck + "\ntotal: " + to_string(games.second.p1POV_value)).set_flags(dpp::message_flags::m_ephemeral));
	if (event.custom_id.find("bjstand_") not_eq -1 and index[1] == to_string(event.command.member.user_id)) {
		for (auto& games : bj_callback) {
			if (games.first.first == index[1] and games.second.turn == 2 or games.first.second == index[1] and games.second.turn == 1) {
				event.reply(dpp::message("<@" + (games.second.turn == 2 ? games.first.second : games.first.first) + (">. ") + (games.second.turn == 1 ? GetUserData(stoull(games.first.second)).username : GetUserData(stoull(games.first.first)).username) + (" passed there turn")).set_allowed_mentions(false, false, false, false, vector<dpp::snowflake>{ games.second.turn == 2 ? stoull(games.first.second) : stoull(games.first.first) }, { 0 }));
				games.second.msg.components.clear();
				games.second.turn == 1 ? games.second.turn = 2 : games.second.turn = 1;
				games.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text((games.second.turn == 1 ? GetUserData(stoull(games.first.second)).username : GetUserData(stoull(games.first.first)).username) + (" turn")));
				games.second.msg.add_component(dpp::component()
					.add_component(dpp::component()
						.set_emoji(u8"👊")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id(("bjhit_") + (games.second.turn == 1 ? games.first.second : games.first.first)))
					.add_component(dpp::component()
						.set_emoji(u8"🛑")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id(("bjstand_") + (games.second.turn == 1 ? games.first.second : games.first.first)))
					.add_component(dpp::component()
						.set_emoji("card_back", 1105985144147415080)
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id("bjview_" + games.first.second)));
				bot.message_edit_sync(games.second.msg);
				games.second.msg = games.second.msg;
			}
			break;
		}
		sleep_for(6s);
		event.delete_original_response();
	}
	if (event.custom_id.find("bjhit_") not_eq -1 and index[1] == to_string(event.command.member.user_id)) {
		for (auto& games : bj_callback) {
			if (games.first.first == index[1] and games.second.turn == 2 or games.first.second == index[1] and games.second.turn == 1) {
				if (games.second.turn == 1) {
					randomx draw = randomx().i32(1, 52); games.second.p1.emplace(make_pair(cards[draw.val32].second, cards[draw.val32].second), make_pair(cards[draw.val32].first, cards[draw.val32].first));
					games.second.p1_deck += cards[draw.val32].first, games.second.p1POV_deck += cards[draw.val32].first;
					games.second.p1_value += cards[draw.val32].second, games.second.p1POV_value += cards[draw.val32].second;
					event.reply(dpp::message(event.command.channel_id, cards[draw.val32].first).set_flags(dpp::message_flags::m_ephemeral));
				}
				else {
					randomx draw = randomx().i32(1, 52); games.second.p2.emplace(cards[draw.val32].second, cards[draw.val32].first);
					games.second.p2_deck += cards[draw.val32].first + " ";
					games.second.p2_value += cards[draw.val32].second;
					event.reply(dpp::message(event.command.channel_id, cards[draw.val32].first).set_flags(dpp::message_flags::m_ephemeral));
				}

				games.second.msg.components.clear();
				games.second.msg.embeds.clear();
				if (games.second.p2_value > 21 or games.second.p1POV_value == 21) {
					games.second.msg.add_embed(dpp::embed()
						.set_color(dpp::colors::failed)
						.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
						.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1POV_deck + " [**" + to_string(games.second.p1POV_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]"));
					games.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text(GetUserData(stoull(games.first.second)).username + " won " + to_string(games.second.bet * 2) + u8" 💵"));
					UserData data = GetUserData(stoull(games.first.second));
					data.dollars += games.second.bet * 2;
					SaveUserData(data, stoull(games.first.second));
					bot.message_edit_sync(games.second.msg);
					bj_callback.erase(games.first);
					return;
				}
				if (games.second.p1POV_value > 21 or games.second.p2_value == 21) {
					games.second.msg.add_embed(dpp::embed()
						.set_color(dpp::colors::failed)
						.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
						.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1POV_deck + " [**" + to_string(games.second.p1POV_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]"));
					games.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text(GetUserData(stoull(games.first.first)).username + " won " + to_string(games.second.bet * 2) + u8" 💵"));
					UserData data = GetUserData(stoull(games.first.first));
					data.dollars += games.second.bet;
					SaveUserData(data, stoull(games.first.first));
					bot.message_edit_sync(games.second.msg);
					bj_callback.erase(games.first);
					return;
				}
				games.second.msg.add_embed(dpp::embed()
					.set_title(GetUserData(stoull(games.first.second)).username + " VS " + GetUserData(stoull(games.first.first)).username)
					.set_description("**<@" + games.first.second + "> Deck: **\n> " + games.second.p1_deck + " [**" + to_string(games.second.p1_value) + "**]\n\n\
                                          **<@" + games.first.first + "> Deck: **\n> " + games.second.p2_deck + " [**" + to_string(games.second.p2_value) + "**]"));
				games.second.turn == 1 ? games.second.turn = 2 : games.second.turn = 1;
				games.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text((games.second.turn == 1 ? GetUserData(stoull(games.first.second)).username : GetUserData(stoull(games.first.first)).username) + (" turn")));
				games.second.msg.add_component(dpp::component()
					.add_component(dpp::component()
						.set_emoji(u8"👊")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id(("bjhit_") + (games.second.turn == 1 ? games.first.second : games.first.first)))
					.add_component(dpp::component()
						.set_emoji(u8"🛑")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id(("bjstand_") + (games.second.turn == 1 ? games.first.second : games.first.first)))
					.add_component(dpp::component()
						.set_emoji("card_back", 1105985144147415080)
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_secondary)
						.set_id("bjview_" + games.first.second)));
				bot.message_edit_sync(games.second.msg);
				games.second.msg = games.second.msg;
			}
			break;
		}
	}
	if (event.custom_id.find("GID_") not_eq -1) {
		int GID = stoi(index[1]);
		for (auto& entry : giveaway_entries)
			if (entry.first == GID)
				for (auto it = giveaway_entries[entry.first].entries.begin(); it != giveaway_entries[entry.first].entries.end(); ++it)
					if (*it == event.command.member.user_id) {
						giveaway_entries[entry.first].entries.erase(it);
						/* UPDATE */
						for (auto& GIDS : giveaway_callback)
							if (GIDS.first == GID) {
								GIDS.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text(to_string(giveaway_entries[GID].entries.size()) + " entries."));
								bot.message_edit_sync(GIDS.second.msg);
							}
						event.reply(dpp::message("> You resigned from the giveaway.").set_flags(dpp::message_flags::m_ephemeral));
						return;
					}

		giveaway_traffic traffic;
		traffic.entries.emplace_back(event.command.member.user_id);
		traffic.id = GID;
		giveaway_entries.emplace(stoi(index[1]), traffic);
		for (auto& GIDS : giveaway_callback)
			if (GIDS.first == GID) {
				GIDS.second.msg.embeds[0].set_footer(dpp::embed_footer().set_text(to_string(giveaway_entries[GID].entries.size()) + " entries."));
				bot.message_edit_sync(GIDS.second.msg);
			}
		double entries = static_cast<double>(giveaway_entries[GID].entries.size());
		event.reply(dpp::message("> You entered the giveaway; odds of winning are **" + dpp::remove_tail(stod(dpp::comma(static_cast<double>(100 / entries)))) + "%**").set_flags(dpp::message_flags::m_ephemeral));
	}
}
bool prefix_t(const dpp::message_create_t& event)
{
	vector<string> index = dpp::index(event.msg.content, ' ');
	GuildData g_data = GetGuildData(event.msg.guild_id);
	if (index[1].size() > 1 or index[1].size() < 1) event.reply("Invalid Format. Try: **" + g_data.prefix + "prefix {prefix}**\n**NOTE**: A prefix must only contain 1 char");
	else {
		g_data.prefix = index[1];
		SaveGuildData(g_data, event.msg.guild_id);
		event.reply("Prefix is now set to: **" + g_data.prefix + "**");
	}
	return true;
}
template<typename event_t> bool daily_t(event_t event, dpp::message msg)
{
	msg.set_content("");
	UserData data = GetUserData(dpp::member(event).user_id);
	tm* claimed = dpp::utility::mtm(data.daily);
	time_t ct = dpp::utility::mt_t(claimed, claimed->tm_sec, claimed->tm_min, claimed->tm_hour + 12, claimed->tm_wday, claimed->tm_mday, claimed->tm_mon);
	if (ct < time(0)) {
		randomx rand = randomx().i64(30, 92);
		data.daily = time(0);
		data.dollars += rand.val64;
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::success)
			.set_description("> claimed " + to_string(rand.val64) + " :dollar:"));
		SaveUserData(data, dpp::member(event).user_id);
	}
	else msg.add_embed(dpp::embed()
		.set_color(dpp::colors::failed)
		.set_description("> Next Daily **" + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + "**"));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool profile_t(event_t event, dpp::message msg)
{
	string name = "";
	msg.set_content("");
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name");
	else name = dpp::index(event, "1");
	if (has_char(username(name))) return false;
	UserData data = GetUserData(stoull(username(name)));
	if (data.failed) new_user(stoull(username(name)));
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::PS)
		.set_title(":mag_right: Profile Viewer")
		.set_description(("<@" + username(name) + "> ") + (data.last_on == 0 ? "inactive" : "last online " + dpp::utility::timestamp(data.last_on, dpp::utility::tf_relative_time)))
		.add_field(
			"Inventory: ",
			"> :dollar: " + to_string(data.dollars)));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool leaderboard_t(event_t event, dpp::message msg)
{
	msg.set_content("");
	string oriented = ""; short pos = 0;
	vector<pair<uint64_t, uint64_t>> buffer;
	for (auto& member : members) buffer.emplace_back(member.second.dollars, member.second.user_id);
	sort(buffer.begin(), buffer.end(), first<uint64_t, uint64_t>());
	for (auto& i : buffer) {
		if (i.second == 0) continue;
		pos++, oriented += "> #" + to_string(pos) + ". **" + bot.user_get_sync(i.second).username + "**: " + to_string(i.first) + " :dollar: \n";
		if (pos > 10) break;
	} pos = 0; // reset
	for (auto& i : buffer) {
		if (i.second == 0) continue;
		pos++;
		if (i.second == dpp::member(event).user_id) {
			oriented += "\n*your* **#" + to_string(pos) + "** *on the leaderboard.*";
			break;
		}
	}
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::PS)
		.set_title("Leaderboard: ")
		.set_description(oriented));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool purge_t(event_t event, dpp::message msg = dpp::message())
{
	string amount = "";
	msg.set_content("");
	try {
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) amount = dpp::index(event, "amount");
		else amount = dpp::index(event, "1");
		if (has_char(amount)) return false;
		if (stoi(amount) <= 1 or stoi(amount) > 100) return false;
		else {
			vector<dpp::snowflake> ids;
			auto msgs = bot.messages_get_sync(dpp::channel_id(event), 0, 0, 0, stoull(amount));
			if (msgs.size() <= 1) return false;
			for (auto& msg : msgs) {
				if (msg.second.author.username.empty()) continue;
				ids.emplace_back(msg.second.id);
			}
			if (not ids.empty()) bot.message_delete_bulk_sync(ids, dpp::channel_id(event));
		}
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::failed)
			.set_description("> Deleted `" + amount + "` Message(s)"));
	}
	catch (dpp::exception e) {
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::failed)
			.set_description("> you cannot delete messages older then ``14 days``"));
	}
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> string membercount_t(event_t event, bool send_embed = true, dpp::message msg = dpp::message())
{
	double membercount = 0, bots = 0;
	dpp::snowflake after = 0;
	while (true) {
		for (auto& members : bot.guild_get_members_sync(dpp::guild_id(event), 1000, after)) {
			if (members.second.get_user()->is_bot()) bots++;
			membercount++;
			membercount + bots >= 1000.0 ? after = members.second.user_id : after = 0;
		}
		if (after == dpp::snowflake(0)) break;
	}
	if (send_embed) {
		msg.set_content("");
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::success)
			.set_description("> " + bot.guild_get_sync(dpp::guild_id(event)).name + " (**" + dpp::remove_tail(membercount) + "**)")
			.set_footer(dpp::embed_footer().set_text("did you know? " + dpp::remove_tail(stod(dpp::comma(static_cast<double>(bots / membercount * 100)))) + "% of members are bots.")));
		dpp::message_edit(event, msg);
	}
	return dpp::remove_tail(membercount);
}
template<typename event_t> bool avatar_t(event_t event, dpp::message msg)
{
	msg.set_content("");
	string url = ""; dpp::snowflake mention = 0;
	if (is_same_v<decltype(event), const dpp::slashcommand_t&> ? dpp::empty_index(event, "name") : dpp::empty_index(event, "1"))
		url = dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png);
	else {
		string name = "";
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name");
		else name = dpp::index(event, "1");
		if (has_char(username(name))) return false;
		url = bot.user_get_sync(stoull(username(name))).get_avatar_url(256, dpp::image_type::i_png), mention = stoull(username(name));
	}
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::success)
		.set_description(("<@") + (url == dpp::member(event).get_user()->get_avatar_url(256, dpp::image_type::i_png) ? to_string(dpp::member(event).user_id) : to_string(mention)) + (">'s Avatar"))
		.set_image(url));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool invite_t(event_t event, dpp::message msg)
{
	msg.set_content("");
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::success)
		.set_author("Click me!", "https://discord.com/api/oauth2/authorize?client_id=" +
			to_string(bot.me.id) + "&permissions=" + to_string(dpp::permissions::p_administrator) + "&scope=bot%20applications.commands", ""));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool nick_t(event_t event, dpp::message msg) {
	msg.set_content("");
	string name = "", nickname = "";
	try {
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name"), nickname = dpp::index(event, "nickname");
		else name = dpp::index(event, "1"), nickname = dpp::index(event, "2");
		if (nickname.size() < 1 or nickname.size() > 32) {
			msg.add_embed(dpp::embed()
				.set_color(dpp::colors::failed)
				.set_description("> Nickname must contain 1-32 characters")
				.set_footer(dpp::embed_footer().set_text("length: " + to_string(nickname.size()))))
				.set_flags(dpp::message_flags::m_ephemeral);
			dpp::message_edit(event, msg);
			return false;
		}
		if (has_char(username(name))) return false;
		dpp::guild_member gm = bot.guild_get_member_sync(dpp::guild_id(event), stoull(username(name)));
		gm.nickname = nickname;
		bot.guild_edit_member_sync(gm);
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::success)
			.set_description("> <@" + username(name) + "> name changed to **" + nickname + "**"));
	}
	catch (dpp::exception e) {
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::failed)
			.set_description("> Cannot modify <@" + username(name) + ">'s nickname")
			.set_footer(dpp::embed_footer()
				.set_text(e.msg)));
	}
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool ping_t(event_t event, dpp::message msg) {
	msg.set_content("");
	End = chrono::high_resolution_clock::now();
	msg.add_embed(dpp::embed().set_color(dpp::colors::success)
		.set_title(":ping_pong: Pong!")
		.set_description("> Response time: " + to_string(chrono::duration_cast<chrono::milliseconds>(End - Beg).count()) + "." + to_string(chrono::duration_cast<chrono::microseconds>(End - Beg).count()) + "ms\n\
                          > API Ping: " + to_string(bot.rest_ping)));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool serverinfo_t(event_t event, dpp::message msg) {
	msg.set_content("");
	dpp::guild guild = bot.guild_get_sync(dpp::guild_id(event));
	if (guild.has_banner())
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::success)
			.set_title(guild.name + ":")
			.set_description("> **Banner**: ")
			.set_image(guild.get_banner_url(128, dpp::i_png, guild.has_animated_icon() ? true : false)));
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::success)
		.set_title(guild.name + ":")
		.set_description("> **Icon**: ")
		.set_image(guild.get_icon_url(128, dpp::i_png, guild.has_animated_icon() ? true : false)));
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::success)
		.set_description("> **Ownership**: <@" + to_string(guild.owner_id) + "> \n\
                          > **Members**: " + membercount_t(event, false) + " \n\
                          > **Roles**: " + to_string(bot.roles_get_sync(dpp::guild_id(event)).size()) + " \n\
                          > **Created**: " + dpp::utility::timestamp(static_cast<time_t>(guild.get_creation_time()), dpp::utility::tf_relative_time) + " at " + dpp::utility::timestamp(static_cast<time_t>(guild.get_creation_time()), dpp::utility::tf_short_time) + ""));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool help_t(event_t event, dpp::message msg) {
	msg.set_content("");
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::success)
		.set_title(bot.me.username + " Commands")
		.set_description(":moneybag: **__Economy__**:\n\
                          > </daily:" + to_string(command::name_to_id["daily"]) + ">: daily reward.\n\
                          > </blackjack:" + to_string(command::name_to_id["blackjack"]) + ">: play a game of blackjack. `{Mention} {Bet}` *--* Alias: </bj:" + to_string(command::name_to_id["bj"]) + "> \n\n\
                             :busts_in_silhouette: **__Social__**:\n\
                          > </profile:" + to_string(command::name_to_id["profile"]) + ">: check out someone's profile. `{Mention}`\n\
                          > </leaderboard:" + to_string(command::name_to_id["leaderboard"]) + ">: show richest players. *--* Alias: </top:" + to_string(command::name_to_id["top"]) + "> \n\
                          > </avatar:" + to_string(command::name_to_id["avatar"]) + ">: check out someone's avatar. `{Mention}`\n\
                          > </level:" + to_string(command::name_to_id["level"]) + ">: view your level. *--* Alias: </lvl:" + to_string(command::name_to_id["lvl"]) + ">\n\n\
                             <:mod:1105971852888776816> **__Moderation__**:\n\
                          > </purge:" + to_string(command::name_to_id["purge"]) + ">: mass delete messages in a channel. `{Amount}`\n\
                          > </nick:" + to_string(command::name_to_id["nick"]) + ">: change a member's nickname. `{Mention} {Nickname}`\n\
                          > </timeout:" + to_string(command::name_to_id["timeout"]) + ">: timeout someone. `{Mention} {Legnth}`\n\n\
                             :grey_question: **__Miscellaneous__**:\n\
                          > </membercount:" + to_string(command::name_to_id["membercount"]) + ">: number of members in server.\n\
                          > </serverinfo:" + to_string(command::name_to_id["serverinfo"]) + ">: server information.\n\
                          > </invite:" + to_string(command::name_to_id["invite"]) + ">: invite " + bot.me.username + " to your server.\n\
                          > </ping:" + to_string(command::name_to_id["ping"]) + ">: check the bot's ping time & (API) REST time\n"));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool timeout_t(event_t event, dpp::message msg) {
	msg.set_content("");
	string name = "", length = "";
	try {
		if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name"), length = dpp::index(event, "length");
		else name = dpp::index(event, "1"), length = dpp::index(event, "2");
		if (has_char(username(name))) return false;
		time_t ct = time(0);
		string duration_type = "";
		tm* timeout = dpp::utility::mtm(ct);
		if (length.find("s") not_eq -1) ct = dpp::utility::mt_t(timeout, timeout->tm_sec + stoi(length), timeout->tm_min, timeout->tm_hour, timeout->tm_wday, timeout->tm_mday, timeout->tm_mon),
			length.erase(remove(length.begin(), length.end(), 's'), length.end()), duration_type = " second" + string(stoi(length) <= 1 ? "" : "s") + "**";
		else if (length.find("m") not_eq -1) ct = dpp::utility::mt_t(timeout, timeout->tm_sec, timeout->tm_min + stoi(length), timeout->tm_hour, timeout->tm_wday, timeout->tm_mday, timeout->tm_mon),
			length.erase(remove(length.begin(), length.end(), 'm'), length.end()), duration_type = " minute" + string(stoi(length) <= 1 ? "" : "s") + "**";
		else if (length.find("h") not_eq -1) ct = dpp::utility::mt_t(timeout, timeout->tm_sec, timeout->tm_min, timeout->tm_hour + stoi(length), timeout->tm_wday, timeout->tm_mday, timeout->tm_mon),
			length.erase(remove(length.begin(), length.end(), 'h'), length.end()), duration_type = " hour" + string(stoi(length) <= 1 ? "" : "s") + "**";
		else if (length.find("d") not_eq -1) ct = dpp::utility::mt_t(timeout, timeout->tm_sec, timeout->tm_min, timeout->tm_hour, timeout->tm_wday + stoi(length), timeout->tm_mday + stoi(length), timeout->tm_mon),
			length.erase(remove(length.begin(), length.end(), 'd'), length.end()), duration_type = " day" + string(stoi(length) <= 1 ? "" : "s") + "**";
		bot.guild_member_timeout_sync(dpp::guild_id(event), stoull(username(name)), ct);
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::success)
			.set_description("> <@" + username(name) + "> has been timed out for **" + length + duration_type));
	}
	catch (dpp::exception e) {
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::failed)
			.set_description("> Cannot timeout <@" + username(name) + ">")
			.set_footer(dpp::embed_footer()
				.set_text(e.msg)));
	}
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool blackjack_t(event_t event, dpp::message msg) {
	msg.set_content("");
	string name = "", bet = "";
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) name = dpp::index(event, "name"), bet = dpp::index(event, "bet");
	else name = dpp::index(event, "1"), bet = dpp::index(event, "2");
	if (has_char(username(name)) or has_char(bet) or username(name) == to_string(dpp::member(event).user_id)) return false;
	for (auto& games : bj_callback) {
		if (username(name) == games.first.first) {
			msg.add_embed(dpp::embed()
				.set_color(dpp::colors::failed)
				.set_description("> <@" + username(name) + "> is already invited to a game."));
			dpp::message_edit(event, msg);
			return false;
		}
	}
	dpp::snowflake after = 0; bool is_bot = false;
	while (true) {
		for (auto& members : bot.guild_get_members_sync(dpp::guild_id(event), 1000, after))
			if (members.second.user_id == stoull(username(name))) {
				if (members.second.get_user()->is_bot()) { is_bot = true; break; }
				msg.add_embed(dpp::embed()
					.set_color(dpp::colors::PS)
					.set_description("> **<@" + username(to_string(dpp::member(event).user_id)) + "> has invited <@" + username(name) + ">**\n> blackjack bet: **" + bet + "** :dollar:"));
				msg.add_component(dpp::component()
					.add_component(dpp::component()
						.set_label("Accept")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_success)
						.set_id("bja_" + username(name)))
					.add_component(dpp::component()
						.set_label("Decline")
						.set_type(dpp::cot_button)
						.set_style(dpp::cos_danger)
						.set_id("bjd_" + username(name)))
				);
				bj_callback.emplace(make_pair(username(name), to_string(dpp::member(event).user_id)), blackjack().set_msg(msg).set_bet(stoull(bet)));
			}
		if (after == dpp::snowflake(0)) break;
	}
	if (msg.embeds.empty())
		msg.add_embed(dpp::embed()
			.set_color(dpp::colors::failed)
			.set_description(string(is_bot ? "> <@" + username(name) + "> is not human." : "> <@" + username(name) + "> is not in this server.")));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool level_t(event_t event, dpp::message msg) {
	UserData data = GetUserData(dpp::member(event).user_id);
	cv::Mat im(256, 640, CV_8UC4);
	CV_Assert(im.channels() == 4);
	for (int i = 0; i < im.rows; ++i)
		for (int j = 0; j < im.cols; ++j) {
			im.at<cv::Vec4b>(i, j)[0] = UCHAR_MAX;
			im.at<cv::Vec4b>(i, j)[1] = cv::saturate_cast<uchar>(UCHAR_MAX);
			im.at<cv::Vec4b>(i, j)[2] = cv::saturate_cast<uchar>(UCHAR_MAX);
		}
	cv::line(im, cv::Point(65, static_cast<int>(640 / 4.2)), cv::Point(565, static_cast<int>(640 / 4.2)), cv::Scalar(150, 150, 150, 100), 50, 6);
	cv::putText(im, "Level " + to_string(data.level), cv::Point(246, static_cast<int>(640 / 6.1)), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(150, 150, 150, 100), 2);
	cv::putText(im, static_cast<cv::String>(to_string(data.exp)), cv::Point(67, static_cast<int>(640 / 5.6)), cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(150, 150, 150, 100), 2);
	cv::putText(im, to_string(data.level * 200), cv::Point(543, static_cast<int>(640 / 5.6)), cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(150, 150, 150, 100), 2);
	cv::line(im,
		cv::Point(static_cast<int>(65 + data.exp * (data.level * 200 < 565 ? 565 / (data.level * 200) : (data.level * 200) / 565)), static_cast<int>(640 / 4.2)),
		cv::Point(565, static_cast<int>(640 / 4.2)), cv::Scalar(300, 300, 300, 100), 50, 6);
	try {
		imwrite("CDN\\" + username(to_string(dpp::member(event).user_id)) + ".png", im, { cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, 9 });
	}
	catch (runtime_error& e) {
		bot.log(dpp::ll_warning, "OpenCV error in /level; " + string(e.what()));
		return false;
	}
	msg.set_content("");
	msg.add_file("CDN\\" + username(to_string(dpp::member(event).user_id)) + ".png", dpp::utility::read_file("CDN\\" + username(to_string(dpp::member(event).user_id)) + ".png"));
	dpp::message_edit(event, msg);
	return true;
}
template<typename event_t> bool giveaway_t(event_t event, dpp::message msg) {
	msg.set_content("");
	string prize = "", length = "";
	int64_t winners = 0, This_GID = randomx().i64(100000, 999999).val64;
	if (is_same_v<decltype(event), const dpp::slashcommand_t&>) prize = dpp::index(event, "prize"), winners = dpp::indexi64(event, "winners"), length = dpp::index(event, "length");
	else prize = dpp::index(event, "1"), winners = dpp::indexi64(event, "2"), length = dpp::index(event, "3");
	time_t ct = time(0);
	tm* giveaway = dpp::utility::mtm(ct);
	string chrono = "";
	if (length.find("s") not_eq -1) ct = dpp::utility::mt_t(giveaway, giveaway->tm_sec + stoi(length), giveaway->tm_min, giveaway->tm_hour, giveaway->tm_wday, giveaway->tm_mday, giveaway->tm_mon),
		length.erase(remove(length.begin(), length.end(), 's'), length.end()), chrono = "s";
	else if (length.find("m") not_eq -1) ct = dpp::utility::mt_t(giveaway, giveaway->tm_sec, giveaway->tm_min + stoi(length), giveaway->tm_hour, giveaway->tm_wday, giveaway->tm_mday, giveaway->tm_mon),
		length.erase(remove(length.begin(), length.end(), 'm'), length.end()), chrono = "m";
	else if (length.find("h") not_eq -1) ct = dpp::utility::mt_t(giveaway, giveaway->tm_sec, giveaway->tm_min, giveaway->tm_hour + stoi(length), giveaway->tm_wday, giveaway->tm_mday, giveaway->tm_mon),
		length.erase(remove(length.begin(), length.end(), 'h'), length.end()), chrono = "h";
	else if (length.find("d") not_eq -1) ct = dpp::utility::mt_t(giveaway, giveaway->tm_sec, giveaway->tm_min, giveaway->tm_hour, giveaway->tm_wday + stoi(length), giveaway->tm_mday + stoi(length), giveaway->tm_mon),
		length.erase(remove(length.begin(), length.end(), 'd'), length.end()), chrono = "d";
	msg.add_embed(dpp::embed()
		.set_color(dpp::colors::PS)
		.set_description(u8"**🎉 ∙ Giveaway Ends " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + " ** \n\n> **Host:** <@" + to_string(dpp::member(event).user_id) + "> \n> **Winners:** " + to_string(winners) + " \n> **Prize**: " + prize + " ")
		.set_footer(dpp::embed_footer().set_text("0 entries.")));
	msg.add_component(dpp::component()
		.add_component(dpp::component()
			.set_emoji("giveaway", 1107511917544738939, true)
			.set_label("Join")
			.set_disabled(false)
			.set_style(dpp::cos_primary)
			.set_id("GID_" + to_string(This_GID))));
	dpp::message_edit(event, msg);
	giveaway_callback.emplace(This_GID, giveaway_traffic().set_msg(msg));
	sleep_for(chrono == "s" ? chrono::seconds(stoull(length) - 1) : chrono == "m" ? chrono::minutes(stoull(length) - 1) :
		chrono == "h" ? chrono::hours(stoull(length) - 1) : chrono == "d" ? chrono::hours(stoull(length) * 24 - 1) : 0s);
	msg.components[0].components[0].set_disabled(true);
	msg.embeds[0].set_description(u8"**🎉 ∙ Giveaway Ended " + dpp::utility::timestamp(ct, dpp::utility::tf_relative_time) + " ** \n\n> **Host:** <@" + to_string(dpp::member(event).user_id) + "> \n> **Winners:** " + to_string(winners) + " \n> **Prize**: " + prize + " ");
	msg.embeds[0].set_footer(dpp::embed_footer().set_text(to_string(giveaway_entries[This_GID].entries.size()) + " entries."));
	string winner_list = ""; vector<dpp::snowflake> winner_vec;// orianted
	for (int i = 0; i < winners; i++) winner_vec.emplace_back(giveaway_entries[This_GID].entries[randomx().i64(1, giveaway_entries[This_GID].entries.size()).val64 - 1]);
	for (dpp::snowflake& winner : winner_vec) winner_list = " <@" + to_string(winner) + ">,";
	winner_list.pop_back();
	msg.set_content("Winners: " + winner_list);
	msg.set_allowed_mentions(false, false, false, false, winner_vec, { 0 });
	dpp::message_edit(event, msg);
	return true;
}

inline void await_on_slashcommand(const dpp::slashcommand_t& event) {
	if (event.command.get_command_name() == "ping") Beg = chrono::high_resolution_clock::now();
	UserData data = GetUserData(event.command.member.user_id);
	if (data.failed) async(new_user, event.command.member.user_id).wait();
	if (event.command.get_command_name() == "daily") async(daily_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "profile") async(profile_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "leaderboard" or event.command.get_command_name() == "top") async(leaderboard_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "purge") async(purge_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "membercount") async(membercount_t<const dpp::slashcommand_t&>, event, true, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "avatar") async(avatar_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "invite") async(invite_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "nick") async(nick_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "ping") async(ping_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "serverinfo") async(serverinfo_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "help") async(help_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "timeout") async(timeout_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "blackjack" or event.command.get_command_name() == "bj") async(blackjack_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "level" or event.command.get_command_name() == "lvl") async(level_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.command.get_command_name() == "giveaway") async(giveaway_t<const dpp::slashcommand_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else return;
	{
		UserData data = GetUserData(event.command.member.user_id);
		data.last_on = time(0);
		SaveUserData(data, event.command.member.user_id);
	}
}
vector<thread> commands_executed;
inline void await_on_message_create(const dpp::message_create_t& event) {
	GuildData g_data = GetGuildData(event.msg.guild_id);
	if (event.msg.content.find(g_data.prefix + "ping") not_eq -1) Beg = chrono::high_resolution_clock::now();
	{
		UserData data = GetUserData(event.msg.member.user_id);
		if (data.failed) async(new_user, event.msg.member.user_id).wait();
		for (auto& member : members) if (member.first == event.msg.member.user_id) {
			tm* last = dpp::utility::mtm(member.second.last_exp);
			time_t ct = dpp::utility::mt_t(last, last->tm_sec + 15, last->tm_min, last->tm_hour, last->tm_wday, last->tm_mday, last->tm_mon);
			if (ct < time(0)) data.exp++, data.last_exp = time(0);
		}
		SaveUserData(data, event.msg.member.user_id);
	}
	if (event.msg.content.find(g_data.prefix + "prefix ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_administrator) async(prefix_t, event);
	else if (event.msg.content.find(g_data.prefix + "daily") not_eq -1) async(daily_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "profile ") not_eq -1) async(profile_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "leaderboard") not_eq -1 or event.msg.content.find(g_data.prefix + "top") not_eq -1) async(leaderboard_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "purge ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_administrator) async(purge_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "membercount") not_eq -1) async(membercount_t<const dpp::message_create_t&>, event, true, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "avatar ") not_eq -1 or event.msg.content.find(g_data.prefix + "avatar") not_eq -1) async(avatar_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "invite") not_eq -1) async(invite_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "nick ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_manage_nicknames) async(nick_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "ping") not_eq -1) async(ping_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "serverinfo") not_eq -1) async(serverinfo_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "help") not_eq -1) async(help_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "timeout ") not_eq -1 and dpp::find_guild(event.msg.guild_id)->base_permissions(dpp::find_user(event.msg.member.user_id)) & dpp::p_moderate_members) async(timeout_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "blackjack ") not_eq -1 or event.msg.content.find(g_data.prefix + "bj ") not_eq -1) async(blackjack_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "level") not_eq -1 or event.msg.content.find(g_data.prefix + "lvl") not_eq -1) async(level_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "giveaway ") not_eq -1) async(giveaway_t<const dpp::message_create_t&>, event, dpp::message_create(event, dpp::message(dpp::channel_id(event), "> Processing Request...")));
	else if (event.msg.content.find(g_data.prefix + "nick ") not_eq -1)
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
			.set_description("> Sorry, you need **manage nicknames** permission to preform this command")).set_flags(dpp::message_flags::m_ephemeral));
	else if (event.msg.content.find(g_data.prefix + "purge ") not_eq -1 or event.msg.content.find(g_data.prefix + "prefix ") not_eq -1 or event.msg.content.find(g_data.prefix + "giveaway ") not_eq -1)
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
			.set_description("> Sorry, you need **administrator** permission to preform this command")).set_flags(dpp::message_flags::m_ephemeral));
	else if (event.msg.content.find(g_data.prefix + "timeout ") not_eq -1)
		event.reply(dpp::message(dpp::channel_id(event), dpp::embed().set_color(dpp::colors::failed)
			.set_description("> Sorry, you need **timeout members** permission to preform this command")).set_flags(dpp::message_flags::m_ephemeral));
	else return;
	{
		UserData data = GetUserData(event.msg.member.user_id);
		data.last_on = time(0);
		SaveUserData(data, event.msg.member.user_id);
	}
}
void load_slashcommands()
{
	struct option {
		dpp::command_option_type v_type;
		string name, description;
		bool required = false;
	}; using options = vector<option>;
	struct about {
		string name, description;
		uint64_t permissions;
		options options;
	};
	vector<about> commands = {
		about{"daily", "get a small gift from " + bot.me.username + " once a day", dpp::permissions::p_send_messages},
		about{"profile", "your profile", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "name", "mention user", true}}},
		about{"shop", "view the shop", dpp::permissions::p_send_messages},
		about{"buy", "buy an item from shop", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "id", "the item id", true}, option{dpp::command_option_type::co_string, "amount", "the amount of the item you wanna buy", true}}},
		about{"sell", "sell an item", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "id", "the item id", true}, option{dpp::command_option_type::co_string, "amount", "the amount of the item you wanna sell", true}}},
		about{"fish", "go fishing", dpp::permissions::p_send_messages},
		about{"repair", "the item you wanna repair", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "id", "the item id", true}}},
		about{"leaderboard", "see top players", dpp::permissions::p_send_messages}, about{"top", "see top players", dpp::permissions::p_send_messages},
		about{"purge", "mass delete messages in a channel", dpp::permissions::p_administrator, options{option{dpp::command_option_type::co_string, "amount", "amount of messages to which be deleted", true}}},
		about{"membercount", "view all members in server", dpp::permissions::p_send_messages},
		about{"avatar", "view someone's avatar", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "name", "person's avatar you wanna view. Empty if yourself.", false}}},
		about{"invite", "invite " + bot.me.username + " to your server", dpp::permissions::p_send_messages},
		about{"hunt", "hunt down a animal", dpp::permissions::p_send_messages},
		about{"nick", "change someone's nickname or yourself", dpp::permissions::p_manage_nicknames, options{option{dpp::command_option_type::co_string, "name", "the person you wanna change", false}, option{dpp::command_option_type::co_string, "nickname", "the nickname it'll change too", false}}},
		about{"ping", "pong!", dpp::permissions::p_send_messages},
		about{"serverinfo", "view information about this server", dpp::permissions::p_send_messages},
		about{"help", "list of all commands", dpp::permissions::p_send_messages},
		about{"timeout", "timeout a member", dpp::permissions::p_moderate_members, options{option{dpp::command_option_type::co_string, "name", "the person you wanna timeout", true}, option{dpp::command_option_type::co_string, "length", "the duration the timeout will last for. example: 12h", true}}},
		about{"blackjack", "play a game of blackjack", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "name", "whom your playing against", true}, option{dpp::command_option_type::co_string, "bet", "the betting amount", true}}},
		about{"bj", "play a game of blackjack", dpp::permissions::p_send_messages, options{option{dpp::command_option_type::co_string, "name", "whom your playing against", true}, option{dpp::command_option_type::co_string, "bet", "the betting amount", true}}},
		about{"level", "view your level", dpp::permissions::p_send_messages},
		about{"lvl", "view your level", dpp::permissions::p_send_messages},
		about{"giveaway", "make a giveaway", dpp::permissions::p_moderate_members, options{option{dpp::command_option_type::co_string, "prize", "the prize", true}, option{dpp::command_option_type::co_integer, "winners", "amount of winners", true}, option{dpp::command_option_type::co_string, "length", "the duration the timeout will last for. example: 12h", true}}},
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
	sleep_for(12s);
	dpp::slashcommand_map results = bot.global_bulk_command_create_sync(slashcommand);
	for (auto& command : results) command::name_to_id.emplace(command.second.name, command.second.id);
	if (results.size() == commands.size()) bot.log(dpp::loglevel::ll_trace, "Successfully added all slashcommands");
}