/* PLEASE! for my sanity just set your zoom to 96%; I don't want anyone side scrolling just to see the rest of the code/comment */
/* And around here I should say copyright that, and copyright that. OH.. hmm.. eeehh fine. */
/* Copyright(c) LeeEndl; License Apache License 2.0 */
/* Freely editable but when uploading on github or other sites keep the comments and credibility, K? */ // -> NO! NOT THE CODE! DON'T EAT IT!
#include <dpp/nlohmann/json.hpp> // -> JSON V3.11.2
#include <dpp/dpp.h> // -> D++ V10.0.23

/* presenting header files, that change every week! */
#include "stats.hpp" // -> stats for hunting/battling, this feature was forgotten about T_T
#include "database.hpp" // -> self explained, database related stuff is here. hm. like JSON and file streaming
#include "commands.hpp" // -> also self explained, commands are here.. OH?! you expected me to say more? well.. NOPE!

#pragma warning(disable: 4326) // -> I'm gonna put this here and no one's gonna talk about it, okay?!
void main() { // -> who said I was returning a value.
#pragma warning(default: 4326) // -> It's like it was never here...

	/* just some folder recovery. */
	if (not filesystem::exists("database") or not filesystem::exists("./database/guilds") or not filesystem::exists("./database/users"))
		filesystem::create_directory("database"), filesystem::create_directory("./database/guilds"), filesystem::create_directory("./database/users");

	/* secure way to store your token, it's also a file so you may freely edit */
	if (not ifstream("token").is_open())
		print<string>("Empty Token.", nullptr, state{ newline, color::red, false }),                                    /* ahm- I present a single line ofstream! */
		print<string>("token: ", [](string it) { bot.token = it; }, state{ Inline, color::white, false }), ofstream("token").write(bot.token.c_str(), streamsize(bot.token.size()));
	getline(ifstream("token"), bot.token);

	/* gather intel from database folder for creating a map */
	async(wrap_database).wait(); // -> we wanna wait for this information before going to bot.on_ready

	/* logging, but exceptions are also through out the code. DON'T CALL PRINT<>(), SAVE TIME BY DOING: bot.log(severity, msg);  */
	bot.on_log([](const dpp::log_t& event) {
		print<string>({ bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message }, nullptr,
		state{ newline, event.severity == dpp::ll_trace or event.severity == dpp::ll_debug ? color::gray :
		event.severity == dpp::ll_info ? color::normal :
		event.severity == dpp::ll_warning ? color::yellow :
		event.severity >= dpp::ll_error ? color::red : color::normal, false // -> debating if I should freeze dpp::ll_error and above
			});
		});

	/* safe to put mostly every bot related stuff on startup here.. */
	bot.on_ready([](const dpp::ready_t& event) {
		function<void()> status = [&]() {
			int64_t last = 0;
			while (true) { // -> we loop to keep up-to-date with the guild.size() changes
				/* we don't sleep here cause it's a statement that never goes through unless changed value of guilds.size() */
				if (guilds.size() not_eq last)
					bot.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity()
						.set_name(to_string(guilds.size()) + " servers")
						.set_type(dpp::activity_type::at_streaming)
						.set_url("https://www.twitch.tv/test"))), last = guilds.size(), sleep_for(500ms);
			}
		}; thread::thread(status).detach();
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str())); // -> display the cluster's name, it's here to remove annoying path name also find process easier in taskbar
		thread::thread(load_slashcommands).detach(); // -> register slashcommands, I recommend removing this line else it'll do it every startup.
		});

	/* god these 2 name's bother me. its basically "on_guild_join" : "on_guild_leave". */
	bot.on_guild_create([](const dpp::guild_create_t& event) {
		thread::thread(await_on_guild_create, event).detach();
		});
	bot.on_guild_delete([](const dpp::guild_delete_t& event) {
		thread::thread(await_on_guild_delete, event).detach();
		});

	/* when a standard message passes through, blocking webhooks or other bot's messages */
	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot() or event.msg.member.get_user()->is_verified_bot()) return; // -> ignore bots
		thread::thread(await_on_message_create, event).detach();
		});

	/* same above but for application commands, we ignore webhooks cause they can't use application commands so ya. */
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return; // -> ignore bots
		thread::thread(await_on_slashcommand, event).detach();
		});

	/* whenever a button attached to a message is clicked on */
	bot.on_button_click([](const dpp::button_click_t& event) {
		event_threads.emplace_back(thread::thread(await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}

/* had DPP files deal with my clutered part of the code, I wanna keep my main part organized */
/* you'll get some error if you don't use the D++ provided in my repo */ // -> "some" LOL MORE LIKE A LOT PFFFFTTTT
#ifndef CUSTOM_VERSION
#error "recommended to run neko on D++ custom version: https://github.com/LeeEndl/Neko \
do not worry, we stay up-to-date with D++ version"
#endif