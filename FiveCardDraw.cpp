//FiveCardDraw.cpp: Source file containing definitions for the FiveCardDraw class
//Author: Mia Bendy (mia.bendy@wustl.edu), Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)
#include "stdafx.h"
#include "FiveCardDraw.h"
#include "PlayingCard.h"
#include "Lab4.h"
#include <iostream>
#include "Player.h"

using namespace std;

//Constructor for FiveCardDraw 
FiveCardDraw::FiveCardDraw()
	: dealer(0) //dealer is started at position 0
{
	pot = 0; 
	for (int r = Card::two; r <= Card::ace; ++r) //iterate through all 52 combinations of cards
	{
		for (int s = Card::clubs; s <= Card::spades; ++s)
		{
			//We need to cast r and s as Rank and Suit because they are declared as ints in the for loops
			//they must be declared as ints in the for loops since there is no ++ operator for our rank and suit enums
			Card::Rank rank = static_cast<Card::Rank>(r);
			Card::Suit suit = static_cast<Card::Suit>(s);
			d.add_card(Card(suit, rank)); //add the card to the deck
		}
	}
}

//Method for before the turn of currPlayer
int FiveCardDraw::before_turn(Player & currPlayer)
{
	if (!currPlayer.fold)
	{
		cout << "Name: " << currPlayer.name << " Cards: " << currPlayer.hand << endl;

		string response;
		while (response != "y" && response != "n") //continue asking if they want to discard until we see what is expected
		{
			cout << "Would you like to discard any cards? Type y or n" << endl;
			cin >> ws; //this line clears the whitespace so that getline doesn't read it
			getline(cin, response);
		}
		if (response == "y") //if they want to discard cards ask which indexes 
		{
			const int firstIndex = 0;
			const int lastIndex = 4;
			const int expSize = 5;
			bool validResponse = false;
			while (validResponse == false) //until the response looks as we want, keep prompting 
			{
				cout << "Please type the indexes of the cards you wish to discard separated by spaces" << endl;
				string indexes;
				while (indexes.empty())
				{
					getline(cin, indexes);
				}
				istringstream iss(indexes);
				string num;
				vector<int> discardValues;
				bool invalidCharacter = false;
				while (iss >> num) //pull off values supplied 
				{
					try
					{
						int i = stoi(num);
						if ((i < firstIndex) || (i > lastIndex)) //if index not in bounds, invalid answer
						{
							cout << "Invalid response" << endl;
							invalidCharacter = true;
						}
						else
						{
							discardValues.push_back(i); //if index in bounds push onto our discardValues vector
						}
					}
					catch (...) //if error caught, that means stoi did not work, thus invalid answer
					{
						cout << "Invalid response" << endl;
						invalidCharacter = true;
					}
				}
				if ((discardValues.size() <= expSize) && !invalidCharacter) //if the number discard values does not exceed what is expected 
				{
					set<int> discardSet(discardValues.begin(), discardValues.end()); //we put them into a set to remove duplicates
					vector<int> discardVector(discardSet.begin(), discardSet.end()); //put back into a vector to be sorted in descending order
					reverse(discardVector.begin(), discardVector.end()); //sort into descending order 
					validResponse = true; //a valid response was obtained 
					for (int i : discardVector) //iterate through discard vector (in descending order so as not to reindex) 
					{
						discardDeck.add_card(currPlayer.hand[i]); //add to discard deck
						currPlayer.hand.remove_card(i); //remove from hand 
					}
				}
			}
		}
	}
	return success;
}

//method for currPlayer's turn
int FiveCardDraw::turn(Player & currPlayer)
{
	const int empty = 0;
	const int expHandSize = 5;
	while (currPlayer.hand.size() < expHandSize) //while the player has less than 5 cards 
	{
		if (d.size() == empty) { //if the deck is empty 
			if (discardDeck.size() == empty) //if the discard deck is also empty
			{
				return empty_decks;  //return that the decks are empty 
			}
			discardDeck.shuffleCards(); //shuffle the discard deck
			currPlayer.hand << discardDeck;  //add cards to player hand from discard deck
		}
		else
		{
			currPlayer.hand << d; //add from deck 
		}
	}
	return success;
}

//method for the after turn of currPlayer
int FiveCardDraw::after_turn(Player & currPlayer)
{
	//print their name and their hand s
	//cout << "Name: " << currPlayer.name << " Cards: " << currPlayer.hand << endl;
	return success;
}

//method for before a round in the game
int FiveCardDraw::before_round()
{
	unfold(); //make everyone not folded
	ante(); //ante 
	const int handSize = 5;
	const int noCards = 0;
	d.shuffleCards();
	int player = (dealer + 1) % players.size(); //starts at the position one past the dealer
	while (players[dealer]->hand.size() != handSize) //while the dealers hand does not have 5 cards (since they get dealt last)  
	{
		if (d.size() == noCards) {
			return deck_empty;
		}
		players[player]->hand << d; // add a card to the players hand
		player = (player + 1) % players.size();  //update the player index 

	}
	bet_phase(dealer);
	int start = (dealer + 1) % players.size();
	while (start != dealer) //while the player index is not the dealer index
	{
		before_turn(*players[start]); //call before turn on the player
		start = (start + 1) % players.size(); //update index of player
	}
	before_turn(*players[dealer]); //since the while loop would break at the dealer, call before turn for the dealer
	return success;
}

//method for a round of the game 
int FiveCardDraw::round()
{
	int start = (dealer + 1) % players.size(); //start index one past dealer
	while (start != dealer) //while we aren't at the dealer
	{
		int a = turn(*players[start]); //call turn on the player 
		if (a != success) //if not successful, return 
		{
			return a;
		}
		after_turn(*players[start]); //call after turn on the player
		start = (start + 1) % players.size(); //update player index
	}
	int b = turn(*players[dealer]); //call turn on dealer
	if (b != success)
	{
		return b;
	}
	after_turn(*players[dealer]); //call after turn on dealer
	bet_phase(dealer);
	return success;
}

//method to reset the decks
void FiveCardDraw::reset()
{
	for (size_t i = 0, k = discardDeck.size(); i < k; ++i)
	{
		d.add_card(discardDeck[i]); //add all cards from discard deck to the main deck
	}
	discardDeck.clear(); //clear the discard deck
	for (unsigned int i = 0; i < players.size(); ++i)
	{
		for (int j = 0; j < players[i]->hand.size(); ++j)
		{
			d.add_card(players[i]->hand[j]); //add all the cards from the players hands into the deck
		}
		players[i]->hand.clear(); //clear the players hands
	}
	d.shuffleCards(); //shuffle the main deck
}

//method to prompt and ask if anyone wants to leave, and leave 
void FiveCardDraw::leaveGame()
{
	const string no = "no";
	bool keepPrompting = true;
	while (keepPrompting) //keep prompting until no one wants to leave
	{
		cout << "Type the name of the player who would like to leave, if none type no" << endl;
		string response;
		cin >> response;
		if (response == no) //if no one wants to leave break out of method
		{
			keepPrompting = false;
			break;
		}
		shared_ptr<Player> p = find_player(response); //try to find the player using their response
		if (p) //if player exists, and is not automated
		{
			ofstream ofs(response + ".txt"); //open a file and update their information 
			if (ofs.is_open())
			{
				ofs << p->name << " " << p->wins << " " << p->losses << " " << p->chips;
			}
			ofs.close();
			auto pi = find(players.begin(), players.end(), p);
			if (pi < players.end()) //if the player iterator is not past the end
			{
				players.erase(find(players.begin(), players.end(), p)); //erase the player from the player vector
				cout << p->name << " left game" << endl; //let users know they left
			}
		}
	}
}

//method to prompt and join the game
void FiveCardDraw::joinGame()
{
	const string no = "no";
	bool keepPrompting = true;
	while (keepPrompting) //until no one wants to join, keep prompting
	{
		cout << "Type the name of the player who would like to join, if none type no" << endl;
		string response;
		cin >> response;
		if (response == no) //if no one wants to join, break out of the method 
		{
			keepPrompting = false;
			break;
		}
		try //try to add player
		{
			add_player(response);
		}
		catch (errorCodes) //if add player doesn't work, means player name was already taken 
		{
			cout << "Player name already taken" << endl;
		}
	}

}

//method to leave game without prompting
void FiveCardDraw::autoLeaveGame(Player & currPlayer)
{
	shared_ptr<Player> p = find_player(currPlayer.name); //find the player
	if (p) //if the player exists, open a file and update info 
	{
		ofstream ofs(currPlayer.name + ".txt");
		if (ofs.is_open())
		{
			ofs << currPlayer.name << " " << p->wins << " " << p->losses << " " << p->chips;
		}
		ofs.close();
	}
	auto pi = find(players.begin(), players.end(), p);
	if (pi < players.end())
	{
		players.erase(find(players.begin(), players.end(), p)); //erase the player 
		cout << p->name << " left game" << endl; //let players know who left 
	}
}

//mehtod for after a round of the game
int FiveCardDraw::after_round()
{
	vector<int> winnerIndexes;
	const int empty = 0;
	const int winner = 0;  //the winner will be housed in the zero index 
	const int onePlayer = 1;
	vector<shared_ptr<Player>> temporary; //make a vector to hold the players temporarily 
	for (size_t i = 0; i < players.size(); ++i)
	{
		if (!players[i]->fold)
		{
			temporary.push_back(players[i]); 
		}
	}
	sort(temporary.begin(), temporary.end(), pokerRank); //sort players based on their poker hands
	const int loser = temporary.size() - 1; //loser is housed in the last index 
	for (unsigned int i = 0; i < temporary.size(); ++i) //iterate through players to find if there was a tie
	{
		if (temporary[i]->hand == temporary[winner]->hand) //if anyone has same hand as winner (including the winner)
		{
			++temporary[i]->wins; //update their wins 
			winnerIndexes.push_back(i);
		}
		else
		{
			++temporary[i]->losses; //update everyone elses losses
		}
	}

	const int totalWinners = winnerIndexes.size();
	const int winnerPot = pot / totalWinners; //for uneven pot, remove extra chips

	for (int i = 0; i < totalWinners; ++i) {
		cout << "Winner pot:  " << winnerPot << endl; 
		temporary[winnerIndexes[i]]->chips += winnerPot;
		cout << "total chips after win:  " << temporary[winnerIndexes[i]]->chips << endl;
	}

	pot = empty;

	//iterate through the temporary vector of players
	for (unsigned int i = 0; i < temporary.size(); ++i)
	{ 
		cout << "Player: " << temporary[i]->name << " Wins: " << temporary[i]->wins <<
			" Losses: " << temporary[i]->losses << " Current Hand: " << temporary[i]->hand << " Chips: " << temporary[i]->chips << endl;
	}
	for (unsigned int i = 0; i < players.size(); ++i)
	{
		if (players[i]->fold)
		{
			//print out the player name, their wins, their losses, and their current hand 
			cout << "Player: " << players[i]->name << " Wins: " << players[i]->wins <<
				" Losses: " << players[i]->losses << " Folded this round " << " Chips: " << players[i]->chips << endl;
		}
	}
	const int firstPlayer = 0;
	reset();  //reset the deck
	leaveGame();  //ask if anyone wants to leave
	joinGame(); //ask if anyone wants to join 
	if (players.size() == onePlayer) //if there is only one player left after this
	{
		autoLeaveGame(*players[firstPlayer]); //call autoLeaveGame on that player so that their info can be saved

	}
	const int noPlayers = 0;
	if (players.size() == noPlayers)
	{
		return no_players;
	}
	dealer = (dealer + 1) % players.size(); //update the dealer position 
	return success;
}

void FiveCardDraw::print_hand(Player p)
{
	cout << "Name: " << p.name << " Cards: " << p.hand << " Chips: " << p.chips << endl;
}



