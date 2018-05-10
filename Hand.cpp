//Hand.cpp: This is the source file where the Hand class and relevant functions are defined
//Author: Mia Bendy (mia.bendy@wustl.edu) with edits from Everett Witt (everett.witt@wustl.edu) and Reilly Freret (freret@wustl.edu)

#include "stdafx.h"
#include "Hand.h"

using namespace std;

//Default Hand constructor, poker hand is of no rank originally 
Hand::Hand()
	: pokerHand(poker_hand::no_rank) {
	vector<Card> v;
	cardsInHand = v;
}

//Copy constructor 
Hand::Hand(const Hand &otherHand)
	: pokerHand(otherHand.pokerHand), cardsInHand(otherHand.cardsInHand) {}

Hand::Hand(vector<Card> c)
	:pokerHand(pokerHandify(c)), cardsInHand(c)
{
	sort(cardsInHand.begin(), cardsInHand.end()); 
}



//Assignment operator
Hand & Hand::operator= (const Hand &otherHand) {
	//check for self assignment 
	if (this != &otherHand) {
		cardsInHand.clear();
		cardsInHand = otherHand.cardsInHand;
		pokerHand = otherHand.pokerHand;
	}
	return *this;
}

//method to return number of cards in hand
int Hand::size() const {
	return cardsInHand.size();
}

//equivalence operator for two hands
bool Hand::operator== (const Hand &otherHand) const {
	return (cardsInHand == otherHand.cardsInHand);
}

//Less than operator
bool Hand::operator< (const Hand &otherHand) const {
	size_t shorterHand;
	if (cardsInHand == otherHand.cardsInHand)
	{
		return false;
	}
	if (size() <= otherHand.size())
	{
		shorterHand = size();
	}
	else
	{
		shorterHand = otherHand.size();
	}
	//iterate through both hands, for the amount of the shorter hand
	for (size_t i = 0; i < shorterHand; ++i)
	{
		//if the card is less than the card in the other hand return true
		if (cardsInHand[i].value < otherHand.cardsInHand[i].value)
		{
			return true;
		}
		//if the card is greater than the card in the other hand return false
		if (otherHand.cardsInHand[i].value < cardsInHand[i].value)
		{
			return false;
		}
	}
	//If made it here, that means all cards in hands are equivalent except one is longer
	if (size() < otherHand.size())
	{
		//if the hand is shorter than the other hand return true
		return true;
	}
	else
	{
		//if the hand is longer than the other hand return false; 
		return false;
	}
}

//indexing operator for a hand
Card Hand::operator[] (size_t i) const{
	if ((0 <= i) && (i < cardsInHand.size())) //if i is in bounds, return it
	{
		return cardsInHand[i];
	}
	else
	{
		throw index_out_of_bounds;
	}
}

//method to remove a card
void Hand::remove_card(size_t i) {
	if ((0 <= i) && (i < cardsInHand.size())) //if i is in bounds, remove that card
	{
		cardsInHand.erase(cardsInHand.begin() + i); 
	}
	else
	{
		throw index_out_of_bounds;
	}
}

//a method to create a string representing the cards in the hand
string Hand::toString() const {
	string handString;
	for (size_t i = 0; i < cardsInHand.size(); ++i) 
	{
		Card::Suit csuit = cardsInHand[i].suit;
		Card::Rank crank = cardsInHand[i].value;
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
		//combine rank and suit for the card string and add it to the total hand string
		handString += rankString + suitString + " ";
	}
	return handString;
}

//method to print the poker hand code
void Hand::printPokerHand() {
	cout << "Poker Rank: " << pokerHand;
}

//method to get the poker hand
poker_hand Hand::getPokerHand()
{
	return pokerHand;
}

//method to clear a hand
void Hand::clear()
{
	cardsInHand.clear(); 
}

//definition of operator for a hand onto an ostream
ostream &operator<<(std::ostream &out, const Hand &hand) {
	out << hand.toString();
	return out;
}

//definition of operator for a deck into a hand
int operator<<(Hand &hand, Deck &deck) {
	const int empty = 0;
	const int pokerHandSize = 5;
	//if the deck is empty, return an empty deck error code
	if (deck.size() == empty)
	{
		return errorCodes::deck_empty;
	}
	else
	{
		hand.cardsInHand.push_back(deck.cards[deck.size() - 1]); //push the last card in the deck into the hand
		deck.cards.pop_back(); //pop the last card off the deck
		sort(hand.cardsInHand.begin(), hand.cardsInHand.end()); //sort the hand now
																//if the hand is of size 5, set its pokerHand 
		if (hand.size() == pokerHandSize)
		{
			poker_hand h = poker_hand::no_rank;
			try
			{
				h = pokerHandify(hand.cardsInHand);
				hand.pokerHand = h;
			}
			catch (int)
			{
				cout << "Error: can't get a poker hand if hand is larger than 5" << endl;
			}
		}
		//if the hand is not of size 5, set it to no rank
		else
		{
			hand.pokerHand = poker_hand::no_rank;
		}
		return errorCodes::success;
	}
}

//method to compare hands based on poker rank
bool poker_rank(const Hand &hand1, const Hand &hand2) {
	const int expHandSize = 5;
	const int firstCard = 0;
	const int secondCard = 1;
	const int thirdCard = 2;
	const int fourthCard = 3;
	const int fifthCard = 4;
	if ((hand1.cardsInHand.size() == expHandSize) && (hand2.cardsInHand.size() == expHandSize))
	{
		//if the hands are the same, some extra work will be necessary
		if (hand1.pokerHand == hand2.pokerHand) {
			poker_hand pHand = hand1.pokerHand;
			//if the hand is a straight flush, then the hand with the higher highest card ranks higher
			if (pHand == poker_hand::straight_flush)
			{
				//return true if hand1 has a higher highest card
				return hand2.cardsInHand[fifthCard].value < hand1.cardsInHand[fifthCard].value;
			}
			//two four of a kind hands are sorted by the rank of the 4 cards that have the same rank
			if (pHand == poker_hand::four_of_a_kind)
			{
				//if we have four of a kind then either indexes 0-3 are the same, or 1-4 are the same
				//thus the indexes 1-3 capture the rank of the 4 cards that have the same rank
				return hand2.cardsInHand[thirdCard].value < hand1.cardsInHand[thirdCard].value;
			}
			//two full house hands are sorted by the rank of the 3 cards that have the same rank 
			//and then by the rank of the 2 cards that have the same rank
			if (pHand == poker_hand::full_house)
			{
				Card::Rank hand1ThreeRank;
				Card::Rank hand1TwoRank;
				Card::Rank hand2ThreeRank;
				Card::Rank hand2TwoRank;
				//set appropriate values for variables defined above
				if ((hand1.cardsInHand[firstCard].value == hand1.cardsInHand[secondCard].value) && (hand1.cardsInHand[secondCard].value == hand1.cardsInHand[thirdCard].value))
				{
					hand1ThreeRank = hand1.cardsInHand[firstCard].value;
					hand1TwoRank = hand1.cardsInHand[fifthCard].value;
				}
				else
				{
					hand1ThreeRank = hand1.cardsInHand[fifthCard].value;
					hand1TwoRank = hand1.cardsInHand[firstCard].value;
				}
				if ((hand2.cardsInHand[firstCard].value == hand2.cardsInHand[secondCard].value) && (hand2.cardsInHand[secondCard].value == hand2.cardsInHand[thirdCard].value))
				{
					hand2ThreeRank = hand2.cardsInHand[firstCard].value;
					hand2TwoRank = hand2.cardsInHand[fifthCard].value;
				}
				else
				{
					hand2ThreeRank = hand2.cardsInHand[fifthCard].value;
					hand2TwoRank = hand2.cardsInHand[firstCard].value;
				}
				//if three ranks are the same compare two ranks
				if (hand1ThreeRank == hand2ThreeRank)
				{
					return hand2TwoRank < hand1TwoRank;
				}
				//otherwise just compare three ranks
				else
				{
					return hand2ThreeRank < hand1ThreeRank;
				}
			}
			//two flush hands are sorted by highest ranking card, then by the next highest ranking card, etc.
			if (pHand == poker_hand::flush)
			{
				//iterate through the hands until you find when cards differ and return if hand1 has a greater ranking card
				for (int i = fifthCard; i >= firstCard; --i)
				{
					if (hand1.cardsInHand[i].value != hand2.cardsInHand[i].value)
					{
						return hand2.cardsInHand[i].value < hand1.cardsInHand[i].value;
					}
				}
				return false;
			}
			//two straight hands are sorted by the highest card in each hand
			if (pHand == poker_hand::straight)
			{
				return hand2.cardsInHand[fifthCard].value < hand1.cardsInHand[fifthCard].value;
			}
			//two three of a kind hands are sorted by the rank of the 3 cards that have the same rank
			if (pHand == poker_hand::three_of_a_kind)
			{
				//since the hands are sorted, the third card always holds the value of the three that have the same rank
				return hand2.cardsInHand[thirdCard].value < hand1.cardsInHand[thirdCard].value;

			}
			//two two pair hands are sorted by the rank of the higher pair, 
			//then by the rank of the lower pair, and then by the 5th card
			if (pHand == poker_hand::two_pair)
			{
				//since the hands are sorted, the rank of the lower pair is always housed in index 1, 
				//and rank of higher pair always hosued in index 3
				Card::Rank hand1LowPair = hand1.cardsInHand[secondCard].value;
				Card::Rank hand1HighPair = hand1.cardsInHand[fourthCard].value;
				Card::Rank hand2LowPair = hand2.cardsInHand[secondCard].value;
				Card::Rank hand2HighPair = hand2.cardsInHand[fourthCard].value;
				//if the high pairs are equal
				if (hand1HighPair == hand2HighPair)
				{
					//check the low pairs
					if (hand1LowPair == hand2LowPair)
					{
						Card::Rank hand1Fifth;
						Card::Rank hand2Fifth;
						//if in here, we need to compare the fifth card, so we need to find the fifth card
						if (hand1.cardsInHand[firstCard].value == hand1.cardsInHand[secondCard].value)
						{
							if (hand1.cardsInHand[thirdCard].value == hand1.cardsInHand[fourthCard].value)
							{
								hand1Fifth = hand1.cardsInHand[fifthCard].value;
							}
							else
							{
								hand1Fifth = hand1.cardsInHand[thirdCard].value;
							}
						}
						else {
							hand1Fifth = hand1.cardsInHand[firstCard].value;
						}
						if (hand2.cardsInHand[firstCard].value == hand2.cardsInHand[secondCard].value)
						{
							if (hand2.cardsInHand[thirdCard].value == hand2.cardsInHand[fourthCard].value)
							{
								hand2Fifth = hand2.cardsInHand[fifthCard].value;
							}
							else
							{
								hand2Fifth = hand2.cardsInHand[thirdCard].value;
							}
						}
						else {
							hand2Fifth = hand2.cardsInHand[firstCard].value;
						}
						return hand2Fifth < hand1Fifth;
					}
					else
					{
						return hand2LowPair < hand1LowPair;
					}
				}
				//the high pairs do not equal each other, thus we compare them
				else
				{
					return hand2HighPair < hand1HighPair;
				}
			}
			//two one pair hands are sorted by the rank of the pair, then by the highest unpaired card, 
			//then the next highest unpaired card, and then by the next highest unpaired card
			if (pHand == poker_hand::one_pair)
			{
				Card::Rank hand1Pair;
				Card::Rank hand1High;
				Card::Rank hand1Med;
				Card::Rank hand1Low;
				Card::Rank hand2Pair;
				Card::Rank hand2High;
				Card::Rank hand2Med;
				Card::Rank hand2Low;
				//find the rank of the pair, the high unpaired, the middle unpaired, and the low unpaired for hand1
				if (hand1.cardsInHand[firstCard].value == hand1.cardsInHand[secondCard].value)
				{
					hand1Pair = hand1.cardsInHand[firstCard].value;
					hand1High = hand1.cardsInHand[fifthCard].value;
					hand1Med = hand1.cardsInHand[fourthCard].value;
					hand1Low = hand1.cardsInHand[thirdCard].value;
				}
				else if (hand1.cardsInHand[secondCard].value == hand1.cardsInHand[thirdCard].value)
				{
					hand1Pair = hand1.cardsInHand[secondCard].value;
					hand1High = hand1.cardsInHand[fifthCard].value;
					hand1Med = hand1.cardsInHand[fourthCard].value;
					hand1Low = hand1.cardsInHand[firstCard].value;
				}
				else if (hand1.cardsInHand[thirdCard].value == hand1.cardsInHand[fourthCard].value)
				{
					hand1Pair = hand1.cardsInHand[thirdCard].value;
					hand1High = hand1.cardsInHand[fifthCard].value;
					hand1Med = hand1.cardsInHand[secondCard].value;
					hand1Low = hand1.cardsInHand[firstCard].value;
				}
				else
				{
					hand1Pair = hand1.cardsInHand[fourthCard].value;
					hand1High = hand1.cardsInHand[thirdCard].value;
					hand1Med = hand1.cardsInHand[secondCard].value;
					hand1Low = hand1.cardsInHand[firstCard].value;
				}
				//find the rank of the pair, the high unpaired, the middle unpaired, and the low unpaired for hand2
				if (hand2.cardsInHand[firstCard].value == hand2.cardsInHand[secondCard].value)
				{
					hand2Pair = hand2.cardsInHand[firstCard].value;
					hand2High = hand2.cardsInHand[fifthCard].value;
					hand2Med = hand2.cardsInHand[fourthCard].value;
					hand2Low = hand2.cardsInHand[thirdCard].value;
				}
				else if (hand2.cardsInHand[secondCard].value == hand2.cardsInHand[thirdCard].value)
				{
					hand2Pair = hand2.cardsInHand[secondCard].value;
					hand2High = hand2.cardsInHand[fifthCard].value;
					hand2Med = hand2.cardsInHand[fourthCard].value;
					hand2Low = hand2.cardsInHand[firstCard].value;
				}
				else if (hand2.cardsInHand[thirdCard].value == hand2.cardsInHand[fourthCard].value)
				{
					hand2Pair = hand2.cardsInHand[thirdCard].value;
					hand2High = hand2.cardsInHand[fifthCard].value;
					hand2Med = hand2.cardsInHand[secondCard].value;
					hand2Low = hand2.cardsInHand[firstCard].value;
				}
				else
				{
					hand2Pair = hand2.cardsInHand[fourthCard].value;
					hand2High = hand2.cardsInHand[thirdCard].value;
					hand2Med = hand2.cardsInHand[secondCard].value;
					hand2Low = hand2.cardsInHand[firstCard].value;
				}
				//now compare the pair, if equal the next high card, if equal the medium card, if equal the low card
				if (hand1Pair == hand2Pair)
				{
					if (hand1High == hand2High)
					{
						if (hand1Med == hand2Med)
						{
							if (hand1Low == hand2Low)
							{
								//if here, all cards are the same
								return false;
							}
							return hand2Low < hand1Low;
						}
						return hand2Med < hand1Med;
					}
					return hand2High < hand1High;
				}
				else
				{
					return hand2Pair < hand1Pair;
				}

			}
			//two unranked hands are sorted by highest ranking card, then by the next highest ranking card, etc
			else
			{
				for (int i = fifthCard; i >= firstCard; --i)
				{
					//compare the highest cards until you find different ones
					if (hand1.cardsInHand[i] != hand2.cardsInHand[i])
					{
						return hand2.cardsInHand[i].value < hand1.cardsInHand[i].value;
					}
				}
				//if reach this code, then the hands must be the same, and we can return false
				return false;
			}
		}
		//if the hands are not the same, return true if hand1 ranks higher
		else
		{
			return hand2.pokerHand < hand1.pokerHand;
		}
	}
	else
	{
		throw errorCodes::incorrect_vector_size;
	}
}