//FiveCardDraw.h: Header file containing declarations for the FiveCardDraw class
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)
#pragma once
#include "Game.h"

class FiveCardDraw : public Game {
public:
	FiveCardDraw(); //default constructor
	virtual int before_turn(Player &) override; //method for before turn
	virtual int turn(Player &) override; //method for a turn
	virtual int after_turn(Player &) override; //method for after a turn
	virtual int before_round() override; //method for before a round
	virtual int round() override; //method for the round
	void reset(); //method to reset the discard and regular deck
	void leaveGame(); //method to prompt and have people leave the game
	void joinGame(); //method to prompt and have people join the game
	void autoLeaveGame(Player&) override; //method to have a player leave the game without prompting (by passing the player)
	virtual int after_round() override; //method for after a round 
	virtual void print_hand(Player) override; 
 
protected:
	size_t dealer;  //position of the dealer
	Deck discardDeck; //a deck for discarded cards

};