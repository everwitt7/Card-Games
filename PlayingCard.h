#pragma once
//PlayingCard.h: Header file containing declaration for the Card struct and relevant functions
//Author: Mia Bendy (mia.bendy@wustl.edu)

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

//Declaration of Card struct
struct Card {
	//Enum for the card's suit
	enum Suit {
		clubs, //0
		diamonds, //1
		hearts, //2
		spades //3
	};
	//Enum for the card's rank
	enum Rank {
		two, //0
		three, //1
		four, //2
		five, //3
		six, //4
		seven, //5
		eight, //6
		nine, //7
		ten, //8
		jack, //9
		queen, //10
		king, //11
		ace //12
	};
	//Instance variables of suit and rank for a Card
	Suit suit;
	Rank value;
	//Declaration for the constructor of Card 
	Card(Suit, Rank);
	bool operator< (const Card &) const; //A less than operator
	bool operator== (const Card &) const;  //Equivalence operator
	bool operator!= (const Card &) const; //non-equivalence operator
};

//operator to print out a card 
std::ostream &operator<<(std::ostream &out, const Card &card);

//enum for ordering of poker hands
enum poker_hand {
	no_rank,
	one_pair,
	two_pair,
	three_of_a_kind,
	straight,
	flush,
	full_house,
	four_of_a_kind,
	straight_flush
};

//Function declaration for finding poker hands
poker_hand pokerHandify(std::vector<Card>); //pass by value so not to modify the vector passed



