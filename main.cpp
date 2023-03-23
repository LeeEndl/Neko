#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>
#include "user.hpp"
#include "guild.hpp"
#include "commands.hpp"
#include "slashcommand.hpp"

int main()
{
	if (not ifstream("token").is_open())
		print<string>(
			{
				"Empty Token."
			}, color::red),
		print<string>(
			{
				"token: "
			}, color::white, true), cin >> bot.token, ofstream("token").write(bot.token.c_str(), streamsize(bot.token.size()));
	getline(ifstream("token"), bot.token);
	StructUserMap(), StructGuildMap();
	bot.on_log([](const dpp::log_t& event) {
		print<string>(
			{
				bot.me.username.empty() ? "" : "[", bot.me.username.empty() ? "" : bot.me.format_username(), bot.me.username.empty() ? "" : "] ", event.message
			},
			event.severity == dpp::ll_trace ? color::gray :
			event.severity == dpp::ll_debug or event.severity == dpp::ll_info ? color::normal :
			event.severity == dpp::ll_warning ? color::gray : event.severity >= dpp::ll_error ? color::red : color::normal
		);
		});
	bot.on_ready([](const dpp::ready_t& event) {
		auto status = [&]() {
			int64_t last_guilds_size = 0;
			while (true) if (guilds.size() not_eq last_guilds_size)
				bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, to_string(guilds.size()) + " servers")), last_guilds_size = guilds.size(),
				sleep_for(1s);
		};
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		ready_executed.emplace_back(thread::thread(status));
		//register_slashcommands.emplace_back(thread::thread(slashcommand::update_all, false));
		});
	bot.on_guild_create([](const dpp::guild_create_t& event) {
		guild_create_executed.emplace_back(thread::thread(await_on_guild_create, event));
		});
	bot.on_guild_delete([](const dpp::guild_delete_t& event) {
		guild_delete_executed.emplace_back(thread::thread(await_on_guild_delete, event));
		});
	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot() or event.msg.member.get_user()->is_verified_bot()) return;
		commands::commands_executed.emplace_back(commands::await_on_message_create, event);
		});
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return;
		slashcommand::slashcommands_executed.emplace_back(thread::thread(slashcommand::await_on_slashcommand, event));
		});
	bot.on_button_click([](const dpp::button_click_t& event) {
		slashcommand::button_clicked_executed.emplace_back(thread::thread(slashcommand::await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}