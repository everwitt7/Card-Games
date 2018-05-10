//Game.cpp: This is the source file where the Game class and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "stdafx.h"
#include "Game.h"
#include "Lab4.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"

using namespace std;

shared_ptr<Game> Game::gamePtr = nullptr; //set the gamePtr to null

shared_ptr<Game> Game::instance() 
{
	if (gamePtr) //if the pointer is not singular return a copy of the gamePtr
	{
		return gamePtr; 
	}
	else //otherwise throw instance not available
	{
		throw instance_not_available;
	}
}

//method to start a game
void Game::start_game(const string & gameName)
{
	cout << "game started" << endl;
	if (gamePtr)  //if the game already exists, throw an error
	{
		throw game_already_started;
	}
	else //otherwise initizialize it (right now we only have one game option)
	{
		if (gameName == "FiveCardDraw") 
		{
			gamePtr = make_shared<FiveCardDraw>(); 
		}
		else if (gameName == "SevenCardStud")
		{
			gamePtr = make_shared<SevenCardStud>();
		}
		else if (gameName == "TexasHoldEm")
		{
			gamePtr = make_shared<TexasHoldEm>();
		}
		else 
		{
			throw unknown_game; 
		}
	}
}

//method to stop the game
void Game::stop_game()
{
	if (gamePtr) //if the pointer is not singular 
	{
		gamePtr = nullptr; //set it to null
	}
	else //otherwise throw not in progress
	{
		throw no_game_in_progress;
	}

}

//method to add a player to the game
void Game::add_player(const string & playerName)
{
	for (shared_ptr<Player> player : players) //iterate through vector of players
	{
		string name = playerName; //remake name variable so it is not const

		if (player->name == name) { //if the player is in it, throw an error
			throw already_playing; 
		}
	}
	try 
	{
		players.push_back(make_shared<Player>(playerName)); //if the player isn't in it, add the player to the vector
	}
	catch (errorCodes)
	{
		cout << playerName << " did not buy back in" << endl;
	}
	
}

//method to find a player 
shared_ptr<Player> Game::find_player(const string & playerName) 
{
	for (shared_ptr<Player> player : players) //iterate through vector of players
	{
		if (player->name == playerName) { 
			return player; //if player exists, return a shared pointer to it 
		}
	}
	shared_ptr<Player> sp = nullptr;
	return sp; //if the player doesn't exist, return a null pointer
}

void Game::ante()
{
	const int nochips = 0;
	for (shared_ptr<Player> player : players)
	{
		if (player->chips > nochips) 
		{
			--player->chips;
			++pot;
		}
	}
}

void Game::bet_phase(size_t dealer)
{
	const int zeroChips = 0;


	int currBet = 0; 

	for (shared_ptr<Player> player : players)
	{
		player->bet = currBet;
	}

	bool initialBet = false; 
	bool hasChecked = false; 

	size_t currPlayer = dealer;
	const int oneChip = 1;
	const int twoChips = 2;
	int checkCounter = 0;
	int betPosition = 0;


	//RESET ALL PLAYERS BETS
	while ((betPosition < (players.size() - 1)) && (checkCounter != players.size()))
	{
		if ((players[currPlayer]->fold) || (players[currPlayer]->chips == zeroChips))
		{
			currPlayer = (currPlayer + 1) % players.size();
			++betPosition; 
			continue;
		}
		print_hand(*players[currPlayer]);
		if (initialBet)
		{
			string response;
			while (response != "call" && response != "raise" && response != "fold")
			{
				cout << players[currPlayer]->name << " would you like to call, raise, or fold? Type call, raise, or fold" << endl;
				cin >> ws;
				cin >> response;
			}
			if (response == "call")
			{
				int amount = currBet - players[currPlayer]->bet;
				if (players[currPlayer]->chips >= (unsigned)amount) {
					players[currPlayer]->chips -= amount;
					players[currPlayer]->bet += amount;
					pot += amount;
				}
				else {
					pot += players[currPlayer]->chips;
					players[currPlayer]->chips = zeroChips;
					players[currPlayer]->bet = currBet;
				}
				currPlayer = (currPlayer + 1) % players.size();
				++betPosition;
			}
			else if (response == "raise")
			{
				string response2;
				while (response2 != "1" && response2 != "2")
				{
					cout << players[currPlayer]->name << " would you like to bet 1 or 2? Type 1 or 2" << endl;
					cin >> ws;
					cin >> response2;
				}

				int raiseAmount = stoi(response2);
				int amount = raiseAmount + currBet;

				if (players[currPlayer]->chips >= (unsigned)amount)
				{
					players[currPlayer]->chips -= amount;
					players[currPlayer]->bet += amount;
					pot += amount;
					currBet = amount;
					currPlayer = (currPlayer + 1) % players.size();
					betPosition = 0;
				}
				else
				{

					cout << "Invalid Chip Bet - Only Have " << players[currPlayer]->chips << " And Current Bet " << currBet << endl;
				}
			}
			else
			{
				players[currPlayer]->fold = true;
				currPlayer = (currPlayer + 1) % players.size();
				++betPosition;
			}
		}
		else
		{
			string response;
			while (response != "check" && response != "bet")
			{
				cout << players[currPlayer]->name << " would you like to check or bet? Type check or bet" << endl;
				cin >> ws;
				cin >> response;
			}
			if (response == "check")
			{
				++checkCounter;
				currPlayer = (currPlayer + 1) % players.size();
			}
			else
			{
				initialBet = true;
				string response2;
				while (response2 != "1" && response2 != "2")
				{
					cout << players[currPlayer]->name << " would you like to bet 1 or 2? Type 1 or 2" << endl;
					cin >> ws;
					cin >> response2;
				}
				if (response2 == "1")
				{
					++players[currPlayer]->bet;
					--players[currPlayer]->chips;
					++pot;
					++currBet;
					currPlayer = (currPlayer + 1) % players.size();
					betPosition = 0;
				}
				else
				{
					if (players[currPlayer]->chips == oneChip)
					{
						++players[currPlayer]->bet;
						--players[currPlayer]->chips;
						++pot;
						++currBet;
						currPlayer = (currPlayer + 1) % players.size();
						betPosition = 0;
					}
					else
					{
						players[currPlayer]->bet += twoChips;
						players[currPlayer]->chips -= twoChips;
						pot += twoChips;
						currBet += twoChips;
						currPlayer = (currPlayer + 1) % players.size();
						betPosition = 0;
					}
				}
			}
		}
	}
}

void Game::unfold()
{
	for (shared_ptr<Player> player : players)
	{
		player->fold = false; 
	}
}

//method to get the number of players in game
int Game::numPlayers()
{
	return players.size();
}
