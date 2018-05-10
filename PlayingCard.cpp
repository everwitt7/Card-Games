//PlayingCard.cpp: This is the source file where the Card struct and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu)

#include "stdafx.h"
#include "PlayingCard.h"
#include "Lab4.h"
using namespace std;


//Defintion of Card Struct constructor
Card::Card(Suit suitB, Rank valueB)
	: suit(suitB), value(valueB) {}

//Definition of Card Struct < operator 
bool Card::operator< (const Card & c) const {
	Card::Suit cSuit = c.suit;
	Card::Rank cValue = c.value;
	//Since the enum values are in order, just use those to compare
	//if the values are the same, use the suit 
	if (value == cValue)
	{
		return suit < cSuit;
	}
	//if the values are different then rank by the values
	else
	{
		return value < cValue;
	}
}

//equivalence operator 
bool Card::operator== (const Card & c) const {
	//return true if values are the same
	return (value == c.value);
}

//non equivalence operator 
bool Card::operator!= (const Card & c) const {
	//return true if values are the same
	return !(value == c.value);
}

//insertion operator onto an ostream
ostream &operator<<(ostream &out, const Card &card) {
	Card::Suit csuit = card.suit;
	Card::Rank crank = card.value;
	string suitString;
	string rankString;
	//Switch statment to make suitString the appropriate suit name string
	switch (csuit)
	{
	case Card::clubs:
		suitString = "C";
		break;
	case Card::diamonds:
		suitString = "D";
		break;
	case Card::hearts:
		suitString = "H";
		break;
	case Card::spades:
		suitString = "S";
		break;
	}
	//Switch statment to make rankString the appropriate rank name string
	switch (crank)
	{
	case Card::two:
		rankString = "2";
		break;
	case Card::three:
		rankString = "3";
		break;
	case Card::four:
		rankString = "4";
		break;
	case Card::five:
		rankString = "5";
		break;
	case Card::six:
		rankString = "6";
		break;
	case Card::seven:
		rankString = "7";
		break;
	case Card::eight:
		rankString = "8";
		break;
	case Card::nine:
		rankString = "9";
		break;
	case Card::ten:
		rankString = "10";
		break;
	case Card::jack:
		rankString = "J";
		break;
	case Card::queen:
		rankString = "Q";
		break;
	case Card::king:
		rankString = "K";
		break;
	case Card::ace:
		rankString = "A";
		break;
	}
	//combine rank and suit to create a string to print out
	string cardString = rankString + suitString;
	out << cardString;
	return out;
}


poker_hand pokerHandify(std::vector<Card> temp) {

	const int expVectorSize = 5;
	const int expOneSuit = 1;
	const int expTwoRanks = 2;
	const int expThreeRanks = 3;
	const int expFourRanks = 4;
	const int firstCard = 0;
	const int secondCard = 1;
	const int thirdCard = 2;
	const int fourthCard = 3;
	const int fifthCard = 4;

	//if the vector is of size 5, can give it a poker rank
	if (temp.size() == expVectorSize)
	{
		set<Card::Suit> suitSet; //a set of the suits in the hand
		set<Card::Rank> rankSet; //a set of the values in the hand
		poker_hand handCode = poker_hand::no_rank; //Default hand is no rank
		for (size_t i = 0; i < temp.size(); ++i) {
			suitSet.insert(temp[i].suit);
			rankSet.insert(temp[i].value);
		}
		//sort the cards
		sort(temp.begin(), temp.end());

		//check for straight and straight flush
		if ((temp[fifthCard].value == temp[fourthCard].value + 1) && (temp[fourthCard].value == temp[thirdCard].value + 1) && (temp[thirdCard].value == temp[secondCard].value + 1) && (temp[secondCard].value == temp[firstCard].value + 1))
		{
			handCode = poker_hand::straight;
			if (suitSet.size() == expOneSuit)
			{
				handCode = poker_hand::straight_flush;
			}
		}
		//check for a flush
		else if (suitSet.size() == expOneSuit)
		{
			handCode = poker_hand::flush;
		}
		//if the set of Ranks is only of size 2, then the only two possible hands are four of a kind or a full house
		else if (rankSet.size() == expTwoRanks)
		{
			//Since the vector is sorted in ascending order, if the 1st rank is the same as the 4th rank it is four of a kind
			//Or if the higher rank has four of the same, then the 2nd rank will be the same as the 5th rank
			if ((temp[firstCard].value == temp[fourthCard].value) || (temp[secondCard].value == temp[fifthCard].value))
			{
				handCode = poker_hand::four_of_a_kind;
			}
			//Otherwise we know that it is a full house
			else
			{
				handCode = poker_hand::full_house;
			}
		}
		//If the rank set is only of size 3, then the only possible hands are three of a kind, or two pair
		else if (rankSet.size() == expThreeRanks)
		{
			//Check for three of a kind, since it is sorted we know that if 1st = 3rd, or 2nd = 4th, or 3rd = 5th that it is three of a kind
			if ((temp[firstCard].value == temp[thirdCard].value) || (temp[secondCard].value == temp[fourthCard].value) || (temp[thirdCard].value == temp[fifthCard].value))
			{
				handCode = poker_hand::three_of_a_kind;
			}
			//Otherwise we know it is a two pair
			else
			{
				handCode = poker_hand::two_pair;
			}
		}
		//If the rank set is of size 4, the only possible hand is a single pair
		else if (rankSet.size() == expFourRanks)
		{
			handCode = poker_hand::one_pair;
		}
		return handCode;
	}
	//if the vector is not of size five, throw an error
	else
	{
		throw errorCodes::incorrect_vector_size;
	}

}

