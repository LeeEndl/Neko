﻿#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>
#include "stats.hpp"
#include "database.hpp"
#include "commands-alpha.hpp"

int main()
{
	if (not filesystem::exists("database") or not filesystem::exists("./database/guilds") or not filesystem::exists("./database/users")) filesystem::create_directory("database"), filesystem::create_directory("./database/guilds"), filesystem::create_directory("./database/users");
	if (not filesystem::exists("maps")) filesystem::create_directory("maps");
	if (not ifstream("token").is_open())
		print<string>({ "Empty Token." }, color::red),
		print<string>({ "token: " }, color::white, true), cin >> bot.token, ofstream("token").write(bot.token.c_str(), streamsize(bot.token.size()));
	getline(ifstream("token"), bot.token);
	StructUserMap(), StructGuildMap();
	bot.on_log([](const dpp::log_t& event) {
		print<string>({ bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message },
		event.severity == dpp::ll_trace or event.severity == dpp::ll_debug ? color::gray :
		event.severity == dpp::ll_info ? color::normal :
			event.severity == dpp::ll_warning ? color::yellow :
			event.severity >= dpp::ll_error ? color::red : color::normal
		);
		});
	bot.on_ready([](const dpp::ready_t& event) {
		function<void()> status = [&]() {
			int64_t last = 0;
			while (true)
				if (guilds.size() not_eq last)
					bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, to_string(guilds.size()) + " servers")), last = guilds.size(), sleep_for(500ms);
		};
		function<void()> reset_ratelimit = [&]() {
			while (true) {
				sleep_for(200ms);
				for (auto& find : members) if (find.second.ratelimit > 0) sleep_for(4s), find.second.ratelimit = 0, find.second.once_retelimit = false;
			}
		};
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		ready_executed.emplace_back(thread::thread(status));
		ready_executed.emplace_back(thread::thread(reset_ratelimit));
		if (not ifstream("SLASHCOMMAND_VERSION").is_open())
			ofstream("SLASHCOMMAND_VERSION").write(CURRENT_VERSION.c_str(), streamsize(CURRENT_VERSION.size()));
		else getline(ifstream("SLASHCOMMAND_VERSION"), CURRENT_VERSION);
		if (stoi(CURRENT_VERSION) < stoi(SLASHCOMMAND_VERSION)) register_slashcommands.emplace_back(thread::thread(update_all, false));
		});
	bot.on_guild_create([](const dpp::guild_create_t& event) {
		guild_create_executed.emplace_back(thread::thread(await_on_guild_create, event));
		});
	bot.on_guild_delete([](const dpp::guild_delete_t& event) {
		guild_delete_executed.emplace_back(thread::thread(await_on_guild_delete, event));
		});
	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot() or event.msg.member.get_user()->is_verified_bot()) return;
		if (isCommand(event)) {
			ratelimit<const dpp::message_create_t&>(event);
			for (auto& find : members) if (find.second.ratelimit < 3) commands_executed.emplace_back(thread::thread(await_on_message_create, event));
		}
		});
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return;
		ratelimit<const dpp::slashcommand_t&>(event);
		for (auto& find : members) if (find.second.ratelimit < 3) slashcommands_executed.emplace_back(thread::thread(await_on_slashcommand, event));
		});

	bot.on_button_click([](const dpp::button_click_t& event) {
		button_clicked_executed.emplace_back(thread::thread(await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}

#ifndef CUSTOM_VERSION
#error "recommended to run neko on D++ custom version: https://github.com/LeeEndl/Neko \
do not worry, we stay up-to-date with D++ version"
#endif