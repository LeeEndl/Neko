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
		slashcommands::update(command::purge);
		tasks.emplace(-1, static_cast<thread>(memory::ImportThreads));
		cout << endl << endl << "Package Information:" << endl << dpp::utility::version() << endl << "JSON " << JSON_VERSION << endl << endl;
		SetConsoleTitleA("Neko Running!");
		});
	bot.on_guild_create([&](const dpp::guild_create_t& event) {
		memory::GuildData data = memory::GetGuildData(event.created->id);
		if (static_cast<bool>(data.failed)) memory::new_guild(event.created->id);
		if (data.joined) return;
		dpp::snowflake a_channel = 0;
		for (const auto& channels : bot.channels_get_sync(event.created->id)) {
			if (channels.second.is_category()) continue;
			a_channel = channels.second.id;
			break;
		}
		if (bot.guild_get_member_sync(event.created->id, bot.me.id).user_id == bot.me.id) {
			dpp::message msg = dpp::message();
			msg.content = "**__Support Our Server__**\nhttps://discord.gg/AGkGb7d4Wr \n~ Thank you for inviting me!";
			msg.channel_id = a_channel;
			bot.message_create(msg);
			data.joined = true;
			memory::SaveGuildData(data, event.created->id);
			return;
		}
		});
	bot.on_guild_delete([&](const dpp::guild_delete_t& event) { memory::delete_guild(event.deleted->id); });
	bot.on_message_create([&](const dpp::message_create_t& event) {
		commands_executed.emplace_back(thread::thread(await_on_message_create, event));
		});
	bot.on_slashcommand([&](const dpp::slashcommand_t& event) {
		slashcommands_executed.emplace_back(thread::thread(await_on_slashcommand, event));
		});
	bot.on_button_click([&](const dpp::button_click_t& event) {
		memory::UserData data = memory::GetUserData(bot.user_get_sync(event.command.member.user_id));
		vector<string> i = memory::explode(event.custom_id, '_');
		if (event.custom_id.find("repair_1"/*given: repair_1_userid*/) not_eq -1 and i[1] == event.command.member.user_id)
		{
			if (data.dollars > 12) {
				data.dollars -= 12;
				data.rod = to_string(1);
				data.rod_d = 15;
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("Repair Complete!");
				event.reply(dpp::message(event.command.channel_id, embed));
				memory::SaveUserData(data, bot.user_get_sync(event.command.member.user_id));
			}
			else {
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("You don't have enough to repair it!");
				event.reply(dpp::message(event.command.channel_id, embed));
			}
		}
		});
	bot.start(dpp::start_type::st_wait);
}