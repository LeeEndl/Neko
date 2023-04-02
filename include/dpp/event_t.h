﻿#pragma once
using namespace std;
using namespace this_thread;
dpp::cluster bot("", static_cast<uint32_t>(dpp::i_default_intents | dpp::i_message_content));
vector<thread> ready_executed;

namespace dpp {
	dpp::guild_member member(const dpp::message_create_t& event) { return event.msg.member; }
	dpp::guild_member member(const dpp::slashcommand_t& event) { return event.command.member; }
	dpp::snowflake channel_id(const dpp::message_create_t& event) { return event.msg.channel_id; }
	dpp::snowflake channel_id(const dpp::slashcommand_t& event) { return event.command.channel_id; }
	dpp::snowflake guild_id(const dpp::message_create_t& event) { return event.msg.guild_id; }
	dpp::snowflake guild_id(const dpp::slashcommand_t& event) { return event.command.guild_id; }
	string index(const dpp::message_create_t& event, string pos) { return explode(event.msg.content, ' ')[stoi(pos)]; }
	string index(const dpp::slashcommand_t& event, string pos) { return get<string>(event.get_parameter(pos)); }
	bool empty_index(const dpp::message_create_t& event, string pos) { if (explode(event.msg.content, ' ').size() == stoi(pos)) return true; else return false; }
	bool empty_index(const dpp::slashcommand_t& event, string pos) { if (event.get_parameter("name").index() == 0) return true; else return false; }
	dpp::message message_create(const dpp::message_create_t& event, dpp::message msg) { return bot.message_create_sync(msg); }
	dpp::message message_create(const dpp::slashcommand_t& event, dpp::message msg) { event.reply(u8"᲼᲼ ᲼᲼"); event.delete_original_response(); return bot.message_create_sync(msg); }
	void message_edit(const dpp::message_create_t& event, dpp::message msg) { bot.message_edit_sync(msg); }
	void message_edit(const dpp::slashcommand_t& event, dpp::message msg) { bot.message_edit_sync(msg); }
	void message_delete(const dpp::message_create_t& event, dpp::snowflake msg_id, dpp::snowflake channel_id) { bot.message_delete(msg_id, channel_id); }
	void message_delete(const dpp::slashcommand_t& event, dpp::snowflake msg_id, dpp::snowflake channel_id) { bot.message_delete(msg_id, channel_id); }
}