#include <dpp/nlohmann/json.hpp>
#include <dpp/dpp.h>
#include "memory.hpp"
#include "commands.hpp"
#include "slashcommand.hpp"
using namespace std;
using namespace this_thread;

int main()
{
	SetConsoleTitleA("Starting...");
	bot.on_log([&](const dpp::log_t& event) { cout << "[" << event.severity << "]" << event.message << endl; });
	bot.on_ready([&](const dpp::ready_t& event) {
		tasks.emplace(-1, static_cast<thread>(memory::ImportThreads));
		cout << endl << endl << "Package Information:" << endl << dpp::utility::version() << endl << "JSON " << JSON_VERSION << endl << endl;
		SetConsoleTitleA("Neko Running!");
		});
	bot.on_guild_create([&](const dpp::guild_create_t& event) {
		memory::guild_create_executed.emplace_back(thread::thread(memory::await_on_guild_create, event));
		});
	bot.on_guild_delete([&](const dpp::guild_delete_t& event) {
		memory::guild_delete_executed.emplace_back(thread::thread(memory::await_on_guild_delete, event));
		});
	bot.on_message_create([&](const dpp::message_create_t& event) {
		commands_executed.emplace_back(thread::thread(await_on_message_create, event));
		});
	bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
		slashcommands_executed.emplace_back(thread::thread(await_on_slashcommand, event));
		});
	bot.on_button_click([&](const dpp::button_click_t& event) {
		button_clicked_executed.emplace_back(thread::thread(await_on_button_click, event));
		});
	bot.start(dpp::start_type::st_wait);
}