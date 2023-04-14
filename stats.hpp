#pragma once
using namespace std;                

enum passive { none, poison };
class passives {
public:
	string emoji = "";          
	passive passive = passive::none;      
	int turns = 0;         
};
class stats {
public:
	int ATK = 0;             
	int DEF = 0;                   
	int SPD = 0;                 
	map<passive, passives> passive;               
};
map<string, stats> Stat = {
	{":worm:", stats{0, 9, 0}},
	{":lady_beetle:", stats{1, 3, 1}},
	{":rat:", stats{1, 2, 3}},
	{":butterfly:", stats{1, 0, 4}},
	{":bee:", stats{1, 3, 4}},

	{":knife:", stats{1, 0, 5}},
};