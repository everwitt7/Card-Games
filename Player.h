#pragma once
//Player.h: This is the header file where the Player struct and relevant functions are declared
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include<string>
#include "Hand.h"
#include <memory>


struct Player {
	std::string name; //has a name
	Hand hand;  //has a hand
	unsigned int wins;  //has wins
	unsigned int losses; //has losses
	unsigned int chips;
	unsigned int bet;
	bool fold; 
	Player(std::string); //constructor 
};

//function to compare players based on the poker rank of their hands
bool pokerRank(const std::shared_ptr<Player> &p1, const std::shared_ptr<Player> &p2);

//declaration of insertion operator of a hand onto an ostream
std::ostream &operator<<(std::ostream &out, const Player &player);