#pragma once
//Deck.h: Header file containing declaration for the Deck class and relevant functions
//Author: Mia Bendy (mia.bendy@wustl.edu), with edits from Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include <random>
#include "PlayingCard.h"
#include "Lab4.h"

class Hand;
class Deck {
	//friend function so that the insertion operator has access to member variable
	friend std::ostream &operator<<(std::ostream &out, const Deck &deck);
	//friend function so that insertion from deck to hand has access to member variable
	friend int operator<<(Hand &hand, Deck &deck); 
public:
	Deck(); 
	Deck(std::string);  //constructor for Deck taking in a string
	void load(std::string file_name); //method for loading cards into deck 
	void shuffleCards(); //method for shuffling the deck
	int size() const; //method to return the number of cards in the deck 
	void add_card(Card); //method to add a card to a deck
	Card remove_card(); //method to pop a card off the back
	void clear();  //clears the deck 
	Card operator[] (size_t) const; //indexing operator

private:
	std::vector<Card> cards;
};

//declaration of insertion of deck object to ostream 
std::ostream &operator<<(std::ostream &out, const Deck &deck);