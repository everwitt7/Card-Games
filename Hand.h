#pragma once
//Hand.h: Header file containing declaration for the Hand class and relevant functions
//Author: Mia Bendy (mia.bendy@wustl.edu) with edits from Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "Deck.h"

class Hand {
	//friend functions so the methods have access to Hand member variables
	friend std::ostream &operator<<(std::ostream &out, const Hand &hand);
	friend int operator<<(Hand &hand, Deck &deck);
	friend bool poker_rank(const Hand &hand1, const Hand &hand2);
public:
	Hand();  //Default constructor for Hand
	Hand(const Hand &); //Copy constructor for Hand
	Hand(std::vector<Card>); //extra constructor for Hand
	Hand & operator= (const Hand &); //assignment operator
	int size() const; //method to return the number of cards in the hand 
	bool operator== (const Hand &) const; //equivalence operator
	bool operator< (const Hand &) const; //less than operator
	Card operator[] (size_t) const; //indexing operator s
	void remove_card(size_t); //method to remove a card
	std::string toString() const;  // "as string" method
	void printPokerHand();  //method to print the poker hand 
	poker_hand getPokerHand(); //method to get the code of the poker hand
	void clear(); //method to clear a hand

private:
	poker_hand pokerHand;  //member variable to hold the type of hand
	std::vector<Card> cardsInHand;
};

//definition of insertion operator of a hand onto an ostream
std::ostream &operator<<(std::ostream &out, const Hand &hand);

//declaration of insertion operator of deck into a hand, returns an int to handle error cases 
int operator<<(Hand &hand, Deck &deck);

//declaration of function to compare poker ranks of two hands
bool poker_rank(const Hand &hand1, const Hand &hand2);
