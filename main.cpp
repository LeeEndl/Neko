/* Copyright(c) LeeEndl; License Apache License 2.0 */

#include <vcruntime.h>
#if _HAS_CXX17 or _HAS_CXX20 or _HAS_CXX23
#include <dpp/nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <dpp/dpp.h>
#include "database.hpp"
#include "commands.hpp"

int main() {
	if (not ifstream("token").is_open())
		neko::print<string>("Couldn't find Token.", nullptr, state{ newline, red }),
		neko::print<string>("Token: ", [](string in) { ofstream("token").write(in.c_str(), in.size()); }, state{ Inline, white });
	async(wrap_database).wait();

	bot.on_log([](const dpp::log_t& event) {
		neko::print<string>({ bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message }, 
		nullptr,
		state{ newline, event.severity == dpp::ll_trace or event.severity == dpp::ll_debug ? gray :
		event.severity == dpp::ll_info ? normal :
		event.severity == dpp::ll_warning ? yellow :
		event.severity >= dpp::ll_error ? red : normal
			});
		});

	bot.on_ready([](const dpp::ready_t& event) {
		function<void()> status = [&]() {
			while (true)
				bot.set_presence(dpp::presence(dpp::ps_online, dpp::activity()
					.set_name(to_string(bot.current_user_get_guilds_sync().size()) + " servers")
					.set_type(dpp::at_streaming)
					.set_url("https://www.twitch.tv/test"))), sleep_for(8s);
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
#else
#error please use /std:c++17 or above.
#endif