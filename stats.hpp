#pragma once
using namespace std;

class stats {
public:
	int ATK = 0; /** @brief if the animal hits you, it'll deal that much dmg */
	int DEF = 0; /** @brief for every 1 def has a chance of removing 1 ATK, 20 and above guarantees it. */
	int Agility = 0; /** @brief how fast and efficent a hit will blow, if 0 it can't fight back */
};
map<string, stats> animal = { /** @brief every animal has it's own stats */
	{":worm:", stats{0, 8, 0}},
	{":lady_beetle:", stats{1, 2, 2}},
	{":rat:", stats{1, 2, 3}}
};