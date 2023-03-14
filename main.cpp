#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>
#include "memory.hpp"
#include "commands.hpp"
#include "slashcommand.hpp"

int main()
{
	SetConsoleTitleA("");
	bot.on_log([](const dpp::log_t& event) {
		print(event.message,
		event.severity == dpp::ll_trace ? color::gray :
		event.severity == dpp::ll_debug or event.severity == dpp::ll_info ? color::normal :
			event.severity == dpp::ll_warning ? color::gray : event.severity >= dpp::ll_error ? color::red : color::normal
		);
		});
	bot.on_ready([](const dpp::ready_t& event) {
		memory::ready_executed.emplace_back(thread::thread(memory::await_on_ready, event));
		});
	bot.on_guild_create([](const dpp::guild_create_t& event) {
		memory::guild_create_executed.emplace_back(thread::thread(memory::await_on_guild_create, event));
		});
	bot.on_guild_delete([](const dpp::guild_delete_t& event) {
		memory::guild_delete_executed.emplace_back(thread::thread(memory::await_on_guild_delete, event));
		});
	bot.on_message_create([](const dpp::message_create_t& event) {
		if (event.msg.webhook_id.empty() == 0 or event.msg.member.get_user()->is_bot() or event.msg.member.get_user()->is_verified_bot()) return;
		commands_executed.emplace_back(thread::thread(await_on_message_create, event));
		});
	bot.on_slashcommand([](const dpp::slashcommand_t& event) {
		if (event.command.member.get_user()->is_bot() or event.command.member.get_user()->is_verified_bot()) return;
		slashcommands_executed.emplace_back(thread::thread(await_on_slashcommand, event));
		});
	bot.on_button_click([](const dpp::button_click_t& event) {
		button_clicked_executed.emplace_back(thread::thread(await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}