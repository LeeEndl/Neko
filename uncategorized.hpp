#pragma once
using namespace std;
using namespace this_thread;

namespace uncategorized
{
	inline void fishing(const dpp::slashcommand_t& event)
	{
		for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = true;
		{
			UserData data = GetUserData(event.command.member.user_id);
			if (data.rod not_eq "-1" and data.rod_d == 0)
			{
				dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_red).set_description("Your fishing rod broke! **/repair 1**");
				event.reply(dpp::message(event.command.channel_id, embed));
				data.rod = "-1";
			}
			SaveUserData(data, event.command.member.user_id);
		}
		UserData data = GetUserData(event.command.member.user_id);
		if (data.rod_d not_eq 0)
		{
			dpp::embed embed = dpp::embed().set_color(dpp::colors::cute_blue).set_description("Waiting for the fish to bite...");
			dpp::message msg = dpp::message();
			msg.channel_id = event.command.channel_id;
			msg.add_embed(embed);
			event.reply(msg);
			async(Sleep, randomx::Int(6000, 11000));
			for (dpp::embed& e : msg.embeds) e.description = "You caught 1 :fish:!";
			event.edit_response(msg);
			data.rod_d -= 1;
			data.fish += 1;
			data.last_fish = std::time(0);
			SaveUserData(data, event.command.member.user_id);
		}
		for (auto& find : members) if (find.first == event.command.member.user_id) find.second.busy_fishing = false, find.second.busy_fishing = false;
	}
	inline thread mass_delete(vector<dpp::snowflake> oids, dpp::snowflake channel_id) {
		for (auto& msg : oids) bot.message_delete(msg, channel_id), sleep_for(200ms);
		return thread();
	}
	inline thread update_status() {
		while (true)
			bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, to_string(guilds.size()) + " servers")), sleep_for(10s);
		return thread();
	}
	inline thread background_tasks()
	{
		async(update_status);
		return thread();
	}
	vector<thread> ready_executed;
	inline void await_on_ready(const dpp::ready_t& event) {
		SetConsoleTitleA(LPCSTR(bot.me.format_username().c_str()));
		ready_executed.emplace_back(thread::thread(background_tasks));
	}
}