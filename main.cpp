/* Copyright(c) LeeEndl; License Apache License 2.0 */

#include <dpp/nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#if _HAS_CXX23 or _HAS_CXX20
#define neko_std23
#endif
#include <dpp/dpp.h>
#include "database.hpp"
#include "commands.hpp"

int main() {
	if (not ifstream("token").is_open())
		neko::print<string>("Couldn't find Token.", nullptr, state{ newline, color::red }),
		neko::print<string>("Token: ", [](string in) { ofstream("token").write(in.c_str(), streamsize(in.size())); }, state{ Inline, color::white });
	async(wrap_database).wait();

	bot.on_log([](const dpp::log_t& event) {
		neko::print<string>({ bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message }, nullptr,
		state{ newline, event.severity == dpp::ll_trace or event.severity == dpp::ll_debug ? color::gray :
		event.severity == dpp::ll_info ? color::normal :
		event.severity == dpp::ll_warning ? color::yellow :
		event.severity >= dpp::ll_error ? color::red : color::normal
			});
		});

	bot.on_ready([](const dpp::ready_t& event) {
		function<void()> status = [&]() {
			while (true)
				bot.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity()
					.set_name(to_string(bot.current_user_get_guilds_sync().size()) + " servers")
					.set_type(dpp::activity_type::at_streaming)
					.set_url("https://www.twitch.tv/test"))), sleep_for(6s);
		}; thread::thread(status).detach();
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		thread::thread(load_slashcommands).detach();
		});

	bot.on_guild_create([](const dpp::guild_create_t& event) {
		thread::thread(await_on_guild_create, event).detach();
		});

	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.is_dm() or event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot()) return;
		thread::thread(await_on_message_create, event).detach();
		});

	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot()) return;
		thread::thread(await_on_slashcommand, event).detach();
		});

	bot.on_button_click([](const dpp::button_click_t& event) {
		event_threads.emplace_back(thread::thread(await_on_button_click, event));
		});
	try { bot.start(dpp::start_type::st_wait); }
	catch (dpp::exception e) {
		neko::print<string>(e.msg, nullptr, { newline, color::red, false }), filesystem::remove(filesystem::path("token")), sleep_for(3s);
		system("neko.exe"); // -> TODO: find an alternative of system()
	}
}

#ifndef CUSTOM_VERSION
#error "recommended to run neko on D++ custom version: https://github.com/LeeEndl/Neko \
do not worry, we stay up-to-date with D++ version"
#endif