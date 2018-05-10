//SevenCardStud.cpp: This is the source file where the SevenCardStud class and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu), with edits from Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "stdafx.h"
#include "SevenCardStud.h"

//Indexes of face down cards 0, 1, 6
//Indexes of face up cards 2, 3, 4, 5

using namespace std; 

//Constructor for FiveCardDraw 
SevenCardStud::SevenCardStud()
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
	vector<bool> v = { 0, 0, 1, 1, 1, 1, 0 }; 
	face = v;  
}

int SevenCardStud::before_turn(Player & currPlayer)
{
	string name = currPlayer.name; 
	try
	{
		playerToCards[name].push_back(d.remove_card());
		playerToCards[name].push_back(d.remove_card());
		playerToCards[name].push_back(d.remove_card());
		return success; 
	}
	catch(errorCodes i)
	{
		return i; 
	}

}

int SevenCardStud::turn(Player & currPlayer)
{
	try
	{
		string name = currPlayer.name;
		playerToCards[name].push_back(d.remove_card());
		return success;
	}
	catch (errorCodes i)
	{
		return i; 
	}
}

int SevenCardStud::after_turn(Player &)
{
	return success;
}

int SevenCardStud::before_round()
{
	const int maxPlayers = 7; 
	if (players.size() > maxPlayers)
	{
		return too_many_players; 
	}
	unfold(); 
	ante(); 
	d.shuffleCards(); 
	return success;
}

int SevenCardStud::round()
{
	int start = (dealer + 1) % players.size();
	while (start != dealer) //while the player index is not the dealer index
	{
		int a = before_turn(*players[start]); //call before turn on the player
		if (a != success)
		{
			return a; 
		}
		start = (start + 1) % players.size(); //update index of player
	}
	int b = before_turn(*players[dealer]);
	if (b != success)
	{
		return b; 

	}
	bet_phase(dealer);

	const int fourTimes = 4; 
	for (int i = 0; i < fourTimes; ++i)
	{
		int curr = (dealer + 1) % players.size();
		while (curr != dealer) //while the player index is not the dealer index
		{
			int c = turn(*players[curr]); //call before turn on the player
			if (c != success)
			{
				return c; 
			}
			curr = (curr + 1) % players.size(); //update index of player
		}
		int d = turn(*players[dealer]);
		if (d != success)
		{
			return d; 
		}
		bet_phase(dealer);
	}
	return success;
}

void SevenCardStud::setHand(Player & currPlayer)
{
 	set<vector<Card>> b;
	vector<Card> a = playerToCards[currPlayer.name]; 
	while (next_permutation(a.begin(), a.end())) {
		vector<Card> c(a.begin(), a.begin() + 5);
		sort(c.begin(), c.end());
		b.insert(c);
	}
	vector<Hand> h; 

	for (vector<Card> d : b)
	{
		h.push_back(Hand(d)); 
	}

	sort(h.begin(), h.end(), poker_rank); 
	currPlayer.hand = h[0]; 
}

void SevenCardStud::reset()
{
	d.clear();
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
	d.shuffleCards(); 

	for (shared_ptr<Player> player : players) {
		player->hand.clear();
		playerToCards[player->name].clear();
	}

}

void SevenCardStud::leaveGame()
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

void SevenCardStud::joinGame()
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

void SevenCardStud::autoLeaveGame(Player &currPlayer)
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

int SevenCardStud::after_round()
{

	for (unsigned int i = 0; i < players.size(); ++i)
	{
		setHand(*players[i]); 
	}
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

void SevenCardStud::print_hand(Player p)
{
	string Pname = p.name; 
	cout << Pname << " Hand: "; 
	for (unsigned int i = 0; i < playerToCards[Pname].size(); ++i)
	{
		cout << playerToCards[Pname][i] << " "; 
	}
	cout << endl; 
	for (unsigned int i = 0; i < players.size(); ++i)
	{
		if (players[i]->name != Pname)
		{
			cout << players[i]->name << " Hand: ";
			for (unsigned int j = 0; j < playerToCards[players[i]->name].size(); ++j)
			{
				if (face[j])
				{
					cout << playerToCards[players[i]->name][j] << " ";
				}
				else
				{
					cout << "* ";
				}
				
			}
			cout << endl; 
		}
	}
}
