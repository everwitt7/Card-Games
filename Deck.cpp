//Deck.cpp: This is the source file where the Deck class and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu), with edits from Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "stdafx.h"
#include "Deck.h"

using namespace std;

//Default constructor, constructs an empty deck 
Deck::Deck() {
	vector<Card> v; 
	cards = v; 
}


//Constructor for Deck taking in the name of a file with card definitions
Deck::Deck(string file_name) {
	vector<Card> v;
	cards = v;
	try
	{
		load(file_name);
	}
	catch (errorCodes &i)
	{
		throw i;
	}
}

//load function to load cards from a file to the deck
void Deck::load(string file_name) {
	//Map which matches the valid suit char to its suit
	map <char, Card::Suit> suits;
	suits['C'] = Card::clubs;
	suits['c'] = Card::clubs;
	suits['D'] = Card::diamonds;
	suits['d'] = Card::diamonds;
	suits['H'] = Card::hearts;
	suits['h'] = Card::hearts;
	suits['S'] = Card::spades;
	suits['s'] = Card::spades;

	//Map which matches the valid value chars to their values
	map <char, Card::Rank> values;
	values['2'] = Card::two;
	values['3'] = Card::three;
	values['4'] = Card::four;
	values['5'] = Card::five;
	values['6'] = Card::six;
	values['7'] = Card::seven;
	values['8'] = Card::eight;
	values['9'] = Card::nine;
	values['1'] = Card::ten; //Because I mapped chars to values, I have to use '1' and check for the '0' later
	values['J'] = Card::jack;
	values['j'] = Card::jack;
	values['Q'] = Card::queen;
	values['q'] = Card::queen;
	values['K'] = Card::king;
	values['k'] = Card::king;
	values['A'] = Card::ace;
	values['a'] = Card::ace;

	const int valid_str_length = 2;
	const int valid_rank_10_length = 3;
	const int map_count = 0;
	const int suit = 1;
	const int rank = 0;
	const int tenSuit = 2;
	const int tenValue = 1;
	const int expLineCount = 5;
	const int nullLine = 0;
	const int slashIndexOne = 0;
	const int slashIndexTwo = 1;
	const int firstSlashIndex = 2;
	const int secondSlashIndex = 3;
	const int rank10SlashIndex = 3;
	const int rank10SlashIndexTwo = 4;

	ifstream ifs;
	ifs.open(file_name);
	if (ifs.is_open())
	{
		string line1, word1;
		while (getline(ifs, line1))
		{
			istringstream iss(line1);
			while (iss >> word1)
			{
				//break out of loop if next word is '//'
				if (word1[slashIndexOne] == '/' && word1[slashIndexTwo] == '/') {
					break;
				}
				//Valid card strings are two chars, or three chars (only for value 10)
				else if (word1.length() == valid_str_length)
				{
					//Check if the chars are valid for a suit and a value
					if (suits.count(word1[suit]) > map_count && values.count(word1[rank]) > map_count)
					{
						Card::Suit cSuit = suits[word1[suit]];
						Card::Rank cValue = values[word1[rank]];
						//if a valid card string, push it onto the vector
						cards.push_back(Card(cSuit, cValue));
					}
					//if invalid card string print out a warning message
					else
					{
						cout << "Warning: not a valid card string" << endl;
					}
				}
				//If its length is not two, but three check if it is of value 10.
				else if (word1.length() == valid_rank_10_length)
				{
					//check if the chars are valid for a suit and a value, make sure the second char is '0' 
					if (suits.count(word1[tenSuit]) > 0 && values.count(word1[rank]) > 0 && word1[tenValue] == '0')
					{
						Card::Suit cSuit = suits[word1[tenSuit]];
						Card::Rank cValue = values[word1[rank]];
						//if a valid card string, push it onto the vector
						cards.push_back(Card(cSuit, cValue));
					}
					//if invalid card string print a warning message
					else
					{
						cout << "Warning: not a valid card string" << endl;
					}
				}
				//check for card string with '/' '/' right after it
				else if ((word1.length() != valid_str_length) && (word1[firstSlashIndex] == '/') && (word1[secondSlashIndex] == '/'))
				{
					//Check if the chars are valid for a suit and a value
					if (suits.count(word1[suit]) > map_count && values.count(word1[rank]) > map_count)
					{
						Card::Suit cSuit = suits[word1[suit]];
						Card::Rank cValue = values[word1[rank]];
						//if a valid card string, push it onto the vector
						cards.push_back(Card(cSuit, cValue));
					}
					//if invalid card string print out a warning message
					else
					{
						cout << "Warning: not a valid card string" << endl;
					}
					break;
				}
				//check for card string with '/' '/' right after it, this is for a rank 10 
				else if ((word1.length() != valid_rank_10_length) && (word1[rank10SlashIndex] == '/') && (word1[rank10SlashIndexTwo] == '/'))
				{
					//check if the chars are valid for a suit and a value, make sure the second char is '0' 
					if (suits.count(word1[tenSuit]) > 0 && values.count(word1[rank]) > 0 && word1[tenValue] == '0')
					{
						Card::Suit cSuit = suits[word1[tenSuit]];
						Card::Rank cValue = values[word1[rank]];
						//if a valid card string, push it onto the vector
						cards.push_back(Card(cSuit, cValue));
					}
					//if invalid card string print a warning message
					else
					{
						cout << "Warning: not a valid card string" << endl;
					}
					break;
				}
				//Word length is not 2 or 3 (so string is invalid) print out a warning message
				else
				{
					cout << "Warning: not a valid card string" << endl;
				}
			}
		}
	}
	else
	{
		cout << "File not opening, could be invalid file name" << endl;
		throw errorCodes::file_not_open;
	}
}

//method to shuffle the cards in the deck
void Deck::shuffleCards() {
	random_device rd;
	mt19937 g(rd());
	shuffle(cards.begin(), cards.end(), g);
}

//method returning how many cards are in the deck
int Deck::size() const {
	return cards.size();
}

//method to add a singular card to the deck
void Deck::add_card(Card c) {
	cards.push_back(c); 
}

Card Deck::remove_card()
{
	const int noCards = 0; 
	if (cards.size() == noCards)
	{
		throw deck_empty; 
	}
	Card c = cards[cards.size() - 1]; 
	cards.pop_back(); 
	return c;
}

//method to remove all cards from the deck
void Deck::clear()
{
	cards.clear(); 
}

//random access operator to access card in the deck at index i
Card Deck::operator[](size_t i) const
{
	if ((0 <= i) && (i < cards.size())) //if i is in bounds, return it
	{
		return cards[i];
	}
	else
	{
		throw index_out_of_bounds;
	}
}

//definition of insertion operator for a deck onto an ostream
ostream &operator<<(ostream &out, const Deck &deck) {
	//iterate through vector and insert each card onto the stream
	for (size_t i = 0; i < deck.cards.size(); ++i)
	{
		out << deck.cards[i] << endl;
	}
	return out;
}