#pragma once
using namespace std;

class stats {
public:
	int ATK = 0; /** @brief if the animal hits you, it'll deal that much dmg */
	int DEF = 0; /** @brief for every 1 def has a chance of removing 1 ATK, 20 and above guarantees it. */
	int SPD = 0; /** @brief how fast and efficent a hit will blow, if 0 it can't fight back */
};
map<string, stats> Stat = {
	/* Enemies */
	{":worm:", stats{0, 9, 0}},
	{":lady_beetle:", stats{1, 3, 1}},
	{":rat:", stats{1, 2, 3}},
	{":butterfly:", stats{1, 0, 4}},

	/* Weapons */ //-> weapons boost YOUR stats, thus (weapon stats += your stats)
	{":knife:", stats{1, 0, 5}},
};