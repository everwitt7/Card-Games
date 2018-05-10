//TexasHoldEm.h: Header file containing declarations for the TexasHoldEm class
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)


#pragma once
#include "Game.h"
#include <map>
#include <vector>

class TexasHoldEm : public Game {
public:
	TexasHoldEm(); //default constructor
	virtual int before_turn(Player &) override; //method for before turn you get two face down, one face up
	virtual int turn(Player &) override; //method for a turn get one face up card
	virtual int after_turn(Player &) override; //method for after a turn you get one face down card 
	virtual int before_round() override; //method for before a round we call unfold and ante
	virtual int round() override; //method for the round we call before turn once for each player, call bet phase, turn four times (in a cycle, calling betphase after each cycle) for each player
	
	int flop(std::vector<std::shared_ptr<Player>> players);
	int turn_river(std::vector<std::shared_ptr<Player>> players); 
	
	void setHand(Player &); //method to set the players hand to the best five of their seven cards
	void reset(); //method to reset the discard and regular deck
	void leaveGame(); //method to prompt and have people leave the game
	void joinGame(); //method to prompt and have people join the game
	void autoLeaveGame(Player&) override; //method to have a player leave the game without prompting (by passing the player)
	virtual int after_round() override; //method for after a round determine winner
	virtual void print_hand(Player) override;

protected:
	size_t dealer;  //position of the dealer
	std::map<std::string, std::vector<Card>> playerToCards;
	std::vector<bool> face; //true = faceup, false = facedown

};