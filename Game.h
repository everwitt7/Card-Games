//Game.h: this is the header file where the Game class is declared
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#pragma once
#include <memory>
#include <algorithm>
#include "Deck.h"
#include "Player.h"

class FiveCardDraw;
class SevenCardStud;
class TexasHoldEm; 

class Game {
public:
	static std::shared_ptr<Game> instance();  //method to return pointer to the game
	static void start_game(const std::string &);  //method to start a game
	static void stop_game();  //method to stop a game
	void add_player(const std::string &); //method to add a player
	std::shared_ptr<Player> find_player(const std::string &);  //method to find a player
	virtual int before_turn(Player &) = 0; //method for before a turn
	virtual int turn(Player &) = 0; //method for a turn
	virtual int after_turn(Player &) = 0; //method for after a turn
	virtual int before_round() = 0; //method for before a round
	virtual int round() = 0; //method for a round
	virtual int after_round() = 0; //method for after a round
	virtual void print_hand(Player) = 0; 
	virtual void autoLeaveGame(Player &) = 0; 
	void ante();
	void bet_phase(size_t);
	void unfold(); 
	int numPlayers(); //method to get the number of players

protected:
	static std::shared_ptr<Game> gamePtr; //points to the game
	unsigned int pot;
	Deck d;  //deck for the game
	std::vector<std::shared_ptr<Player>> players;  //players in the game
};
