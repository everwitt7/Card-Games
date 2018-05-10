//Player.cpp: This is the source file where the Player struct and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "stdafx.h"
#include "Player.h"
#include "Lab4.h"

using namespace std;

//constructor for a player 
Player::Player(string name_) 
	: name(name_), hand(Hand()), wins(0), losses(0), chips(20), fold(false), bet(0)
{
	//try to open a file with the name of the player
	string file_name = name + ".txt"; 
	ifstream ifs;

	const int nochips = 0;
	const int defaultchips = 20;

	ifs.open(file_name);
	if (ifs.is_open()) //if there is a file parse to get the player name, wins, and losses
	{
		string line1, playerName, playerWins, playerLosses, playerChips;
		while (getline(ifs, line1))
		{
			istringstream iss(line1);
			iss >> playerName; 
			iss >> playerWins;
			iss >> playerLosses;
			iss >> playerChips;
		}
		name = playerName; //update name
		wins = stoi(playerWins); //update wins
		losses = stoi(playerLosses);  //update losses
		chips = stoi(playerChips);
		if (chips == nochips)
		{
			string response;

			while (response != "y" && response != "n") //continue asking if they want to discard until we see what is expected
			{
				cout << "Would You Like To Buy Back In (20 Chips)? Type y or n" << endl;
				cin >> ws; //this line clears the whitespace so that getline doesn't read it
				getline(cin, response);
			}
			if (response == "y") //if they want to discard cards ask which indexes 
			{
				chips = defaultchips;
			}
			else 
			{
				throw no_chips;
			}
		}
	}
}

//function to compare players hands using their poker rank 
bool pokerRank(const std::shared_ptr<Player> &p1, const std::shared_ptr<Player> &p2)
{
	if (p1 == nullptr) //if player one is null return false
	{
		return false; 
	}
	else 
	{
		if (p2 == nullptr) //if player two is null return true
		{
			return true; 
		}
		return poker_rank(p1->hand, p2->hand);  //otherwise call poker_rank on their hands
	}
}

//definition of insertion operator of a hand onto an ostream
std::ostream &operator<<(std::ostream &out, const Player &player) {
	out << "Name: " << player.name << "Wins: " << player.wins << "Losses: " << player.losses; 
	return out; 
}
