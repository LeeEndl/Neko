#pragma once
using namespace std; // -> having the honor of being the highest folder so you get: using namespace std!

/* getting to the point with these comments, pardon me sounding like a robot or RAZOR */ // -> it's a genshin impact thing.
/* passive. self explained. */
enum passive { none, poison };
class passives {
public:
	string emoji = ""; /** @brief the emoji displayed below the HP bar. */
	passive passive = passive::none; /** @brief the passive effect */
	int turns = 0; /** @brief length of the passive (turn base) */
};
/* stats that can give you advantage in hunting/battling, for example flying enemies like bee or butterfly is super fast so they hit more often. */
class stats {
public:
	int ATK = 0; /** @brief if the animal hits you, it'll deal that much dmg */
	int DEF = 0; /** @brief for every 1 def has a chance of removing 1 ATK, 20 and above guarantees it. */
	int SPD = 0; /** @brief how fast and efficent a hit will blow, if 0 it can't fight back */
	map<passive, passives> passive; /** @brief positive or nagative passive that is applied to you or the enemy. */
};
/* oh btw stats for YOU will be added to the base stats, for now it'll always be a flat 1, 1, 1 */
map<string, stats> Stat = {
	/* Enemies */
	{":worm:", stats{0, 9, 0}},
	{":lady_beetle:", stats{1, 3, 1}},
	{":rat:", stats{1, 2, 3}},
	{":butterfly:", stats{1, 0, 4}},
	{":bee:", stats{1, 3, 4}},

	/* Weapons */ //-> weapons boost YOUR stats, thus (weapon stats += your stats) // -> ohh I mentioned it here, I made some of these comments like a week ago
	{":knife:", stats{1, 0, 5}},
};