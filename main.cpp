﻿/* Copyright(c) LeeEndl; License Apache License 2.0 */

#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>
#include "stats.hpp"
#include "database.hpp"
#include "commands.hpp"

void main() {
	if (not ifstream("token").is_open())
		print<string>("Empty Token.", nullptr, state{ newline, color::red, false }),
		print<string>("token: ", [](string it) { bot.token = it; }, state{ Inline, color::white, false }), ofstream("token").write(bot.token.c_str(), streamsize(bot.token.size()));
	getline(ifstream("token"), bot.token);

	async(wrap_database).wait();
	bot.on_log([](const dpp::log_t& event) {
		print<string>({ bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message }, nullptr,
		state{ newline, event.severity == dpp::ll_trace or event.severity == dpp::ll_debug ? color::gray :
		event.severity == dpp::ll_info ? color::normal :
		event.severity == dpp::ll_warning ? color::yellow :
		event.severity >= dpp::ll_error ? color::red : color::normal, false
			});
		});

	bot.on_ready([](const dpp::ready_t& event) {
		function<void()> status = [&]() {
			int64_t last = 0;
			while (true) {
				if (guilds.size() not_eq last)
					bot.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity()
						.set_name(to_string(guilds.size()) + " servers")
						.set_type(dpp::activity_type::at_streaming)
						.set_url("https://www.twitch.tv/test"))), last = guilds.size(), sleep_for(500ms);
			}
		}; thread::thread(status).detach();
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		thread::thread(load_slashcommands).detach();
		});

	bot.on_guild_create([](const dpp::guild_create_t& event) {
		thread::thread(await_on_guild_create, event).detach();
		});
	bot.on_guild_delete([](const dpp::guild_delete_t& event) {
		thread::thread(await_on_guild_delete, event).detach();
		});

	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot() or event.msg.member.get_user()->is_verified_bot()) return;
		thread::thread(await_on_message_create, event).detach();
		});

	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return;
		thread::thread(await_on_slashcommand, event).detach();
		});

	bot.on_button_click([](const dpp::button_click_t& event) {
		event_threads.emplace_back(thread::thread(await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}

#ifndef CUSTOM_VERSION
#error "recommended to run neko on D++ custom version: https://github.com/LeeEndl/Neko \
do not worry, we stay up-to-date with D++ version"
#endif