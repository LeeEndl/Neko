/* Copyright(c) LeeEndl; License Apache License 2.0 */
#pragma once

namespace command { map<string, dpp::snowflake> name_to_id; }

struct UserData {
	JINT daily = time(0), last_on = 0, last_exp = 0, exp = 0, level = 1, user_id = 0, dollars = 0;
	JSTRING username = "";

	bool failed = false;
}; map<dpp::snowflake, UserData> members;
struct GuildData {
	JBOOLEAN joined = false;
	bool failed = false;

	string prefix = "";
};
inline UserData GetUserData(dpp::snowflake user_id)
{
	UserData data;
	if (not ifstream("database/users/" + to_string(user_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	ifstream("database/users/" + to_string(user_id) + ".txt") >> J;
	data.daily = ELEMENT_JI("daily")
		data.dollars = ELEMENT_JI("dollars")
		data.last_on = ELEMENT_JI("last_on")
		data.username = ELEMENT_JS("username")
		data.user_id = ELEMENT_JI("user_id")
		data.exp = ELEMENT_JI("exp")
		data.level = ELEMENT_JI("level")
		J = json();
	return data;
}
inline void SaveUserData(UserData data, dpp::snowflake user)
{
	J.dump(1);
	J["daily"] = data.daily;
	J["dollars"] = data.dollars;
	J["last_on"] = data.last_on;
	J["username"] = data.username;
	J["user_id"] = data.user_id;
	J["exp"] = data.exp;
	J["level"] = data.level;
	ofstream("database/users/" + to_string(user) + ".txt") << setw(2) << J;
	J = json();
}
inline void new_user(dpp::snowflake user_id)
{
	UserData data = GetUserData(user_id);
	data.username = bot.user_get_sync(user_id).username;
	data.daily = 0;
	data.dollars = 0;
	data.last_on = 0;
	data.user_id = bot.user_get_sync(user_id).id;
	data.exp = 0, data.level = 1;
	data.failed = false;
	SaveUserData(data, user_id); {
		UserData data = GetUserData(user_id);
		members.emplace(bot.user_get_sync(user_id).id, data);
	}
}
inline GuildData GetGuildData(dpp::snowflake guild_id)
{
	GuildData data;
	if (not ifstream("database/guilds/" + to_string(guild_id) + ".txt").is_open()) {
		data.failed = true;
		return data;
	}
	ifstream("database/guilds/" + to_string(guild_id) + ".txt") >> J;
	data.joined = ELEMENT_JB("joined");
	data.prefix = ELEMENT_JS("prefix");
	J = json();
	return data;
}
inline void SaveGuildData(GuildData data, dpp::snowflake guild_id)
{
	J.dump(1);
	J["joined"] = data.joined;
	J["prefix"] = data.prefix;
	ofstream("database/guilds/" + to_string(guild_id) + ".txt") << setw(2) << J;
	J = json();
}
inline void new_guild(dpp::snowflake guild_id)
{
	GuildData data = GetGuildData(guild_id);
	data.joined = false;
	data.prefix = "$";
	data.failed = false;
	SaveGuildData(data, guild_id);
}

inline void await_on_guild_create(const dpp::guild_create_t& event) {
	GuildData data = GetGuildData(event.created->id);
	if (static_cast<bool>(data.failed)) new_guild(event.created->id);
}

inline void wrap_database() {
	getline(ifstream("token"), bot.token);
	if (not filesystem::exists("CDN")) filesystem::create_directory("CDN");
	if (not filesystem::exists("database")) filesystem::create_directory("database");
	if (not filesystem::exists("./database/guilds")) filesystem::create_directory("./database/guilds");
	if (not filesystem::exists("./database/users")) filesystem::create_directory("./database/users");

	for (const auto& i : filesystem::directory_iterator("database/users")) {
		vector<string> index = dpp::index(i.path().filename().string(), '.');
		members.emplace(static_cast<dpp::snowflake>(stoull(index[0])), GetUserData(stoull(index[0])));
	}
}

auto Beg = chrono::high_resolution_clock::now(), End = chrono::high_resolution_clock::now();

struct blackjack {
	blackjack& set_msg(dpp::message omsg) { msg = omsg; return *this; };
	blackjack& set_bet(JINT Bet) { bet = Bet; return *this; };
	dpp::message msg = dpp::message();
	JINT bet = 0;
	short turn = 0;

	map<pair<int, int>, pair<string, string>> p1;
	map<int, string> p2;

	string p1_deck = "", p1POV_deck = "";
	int p1_value = 0, p1POV_value = 0;

	string p2_deck = "";
	int p2_value = 0;
};
map<pair<string, string>, blackjack> bj_callback;
string card_back = "<:card_back:1105985144147415080>";
map<int, pair<string, int>> cards = {
	{ 1, {"<:AS:1105972868954722326>", 1} },
	{ 2, {"<:AH:1105972866899521677>", 1} },
	{ 3, {"<:AD:1105972865137922129>", 1} },
	{ 4, {"<:AC:1105972863904792606>", 1} },

	{ 5, {"<:2S:1105972107491430510>", 2} },
	{ 6, {"<:2H:1105972106036006932>", 2} },
	{ 7, {"<:2D:1105972104698007663>", 2} },
	{ 8, {"<:2C:1105972103645245611>", 2} },

	{ 9, {"<:3S:1105972295115227137>", 3} },
	{ 10, {"<:3H:1105972101187375126>", 3} },
	{ 11, {"<:3D:1105972109471129651>", 3} },
	{ 12, {"<:3C:1105972108502241430>", 3} },

	{ 13, {"<:4S:1105972299947061390>", 4} },
	{ 14, {"<:4H:1105972298692956201>", 4} },
	{ 15, {"<:4D:1105972296864243813>", 4} },
	{ 16, {"<:4C:1105972296058929264>", 4} },

	{ 17, {"<:5S:1105972464372170784>", 5} },
	{ 18, {"<:5H:1105972462744784936>", 5} },
	{ 19, {"<:5D:1105972292518957199>", 5} },
	{ 20, {"<:5C:1105972301528318035>", 5} },

	{ 21, {"<:6S:1105972469749264435>", 6} },
	{ 22, {"<:6H:1105972468574863460>", 6} },
	{ 23, {"<:6D:1105972467417231480>", 6} },
	{ 24, {"<:6C:1105972465693364234>", 6} },

	{ 25, {"<:7S:1105972607666364497>", 7} },
	{ 26, {"<:7H:1105972606244499537>", 7} },
	{ 27, {"<:7D:1105972604583551076>", 7} },
	{ 28, {"<:7C:1105972460400152606>", 7} },

	{ 29, {"<:8S:1105972603308474492>", 8} },
	{ 30, {"<:8H:1105972612460449832>", 8} },
	{ 31, {"<:8D:1105972611307020388>", 8} },
	{ 32, {"<:8C:1105972609100824647>", 8} },

	{ 33, {"<:9S:1105972739057135728>", 9} },
	{ 34, {"<:9H:1105972736993538119>", 9} },
	{ 35, {"<:9D:1105972735789768845>", 9} },
	{ 36, {"<:9C:1105972734095278200>", 9 }},

	{ 37, {"<:10S:1105972862013149296>", 10 }},
	{ 38, {"<:10H:1105972732824400013>", 10} },
	{ 39, {"<:10D:1105972741363994709>", 10 }},
	{ 40, {"<:10C:1105972740613226506>", 10 }},

	{ 41, {"<:JS:1105973142083608728>", 11} },
	{ 42, {"<:JH:1105973154721042553>", 11} },
	{ 43, {"<:JD:1105973148853211168>", 11} },
	{ 44, {"<:JC:1105973145166430348>", 11 }},

	{ 45, {"<:QS:1105975849741713559>", 12 }},
	{ 46, {"<:QH:1105975848114343956>", 12 }},
	{ 47, {"<:QD:1105975846151401612>", 12 }},
	{ 48, {"<:QC:1105975850899341383>", 12} },

	{ 49, {"<:KS:1105975466357174304>", 13 }},
	{ 50, {"<:KH:1105975470509535292>", 13 }},
	{ 51, {"<:KD:1105975469242855495>", 13} },
	{ 52, {"<:KC:1105975467623854160>", 13 }}
};

class giveaway_traffic { /* manage and maintain multiple giveaways. */
public:
	giveaway_traffic& set_msg(dpp::message omsg) { msg = omsg; return *this; };
	dpp::message msg = dpp::message();
	vector<dpp::snowflake> entries;
	int id = 0;
};
map<int64_t, giveaway_traffic> giveaway_entries;
map<int64_t, giveaway_traffic> giveaway_callback;
