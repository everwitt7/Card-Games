// Lab3.cpp : Main method contained, along with definition of usageMsg function 
// Author: Mia Bendy (mia.bendy@wustl.edu)

#include "stdafx.h"
#include "Lab4.h"
#include "PlayingCard.h"
#include "Deck.h"
#include "Hand.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "Player.h"


using namespace std;

//Function to print out a message with how to correctly run the program
void usageMsg(string program_name, string msg) {
	cout << "Program Name: " << program_name << endl;
	cout << msg << endl;
}



//function to print an error 
void printError(errorCodes i)
{
	switch (i)
	{
	case success:
		cout << "Success" << endl;
		break;
	case file_not_open:
		cout << "Error: File not opening" << endl;
		break;
	case index_out_of_bounds:
		cout << "Error: Index out of bounds error" << endl;
		break;
	case instance_not_available:
		cout << "Error: Game instance not available" << endl;
		break;
	case game_already_started:
		cout << "Error: Game already exists" << endl;
		break;
	case unknown_game:
		cout << "Error: Unknown Game" << endl;
		break;
	case no_game_in_progress:
		cout << "Error: No game in progress" << endl;
		break;
	case already_playing:
		cout << "Error: Player already playing" << endl;
		break;
	case empty_decks:
		cout << "Error: Decks are empty" << endl;
		break;
	case no_cmd_line:
		cout << "Error: No Command line Argument" << endl;
		break;
	case one_incorrect_cmd_line:
		cout << "Error: One incorrect command line" << endl;
		break;
	case two_incorrect_cmd_line:
		cout << "Error: Two incorrect command lines" << endl;
		break;
	case too_many_cmd_line:
		cout << "Error: Too many command line arguments" << endl;
		break;
	case too_few_cmd_line:
		cout << "Error: Too few command line arguments" << endl;
		break;
	case deck_empty:
		cout << "Error: Deck Empty" << endl;
		break;
	case no_chips:
		cout << "Error: No Chips" << endl;
		break;
	case incorrect_vector_size:
		cout << "Error: Incorrect Vector Size" << endl;
		break;
	case no_players:
		cout << "Error: No Players" << endl;
		break;
	case not_enough_players:
		cout << "Error: Not Enough Players" << endl;
		break;
	case too_many_players:
		cout << "Error: Too Many Players" << endl;
		break;
	case unknown:
		cout << "Error: Unknown" << endl;
		break;
	}

}

int main(int argc, char * argv[])
{
	const int exp_arguments = 4;
	const string program_name = argv[0];
	const int notEnoughPlayers = 1;
	const int playerBegin = 2;
	if (argc >= exp_arguments) //if there are more than 4 command line arguments it is valid
	{
		const string game_name = argv[1];
		try {
			Game::start_game(game_name); //try to start a game with the supplied name 
			shared_ptr<Game> g = Game::instance(); //get a pointer to the game
			for (int i = playerBegin; i < argc; ++i) //iterate through the rest of the command line argument 
			{
				string player = argv[i];
				try
				{
					g->add_player(player); //try to add each player 
				}
				catch (errorCodes)
				{
					cout << player << " is already playing" << endl;
				}
			}
			if (g->numPlayers() == notEnoughPlayers)
			{
				cout << "Only one valid player supplied" << endl;
				return not_enough_players;
			}
			while (g->numPlayers() > notEnoughPlayers) //while the number of players greater than 1 keep playing
			{
				int z = g->before_round(); //call before round 
				if (z != success) //if before round is not successful stop the game and return 
				{
					Game::stop_game();
					printError(static_cast<errorCodes>(z)); //z had to be cast here because it is an int but need it to be an errorCode
					return z;
				}
				int a = g->round(); //call round
				if (a != success)  //if round is not successful stop the game and return 
				{
					Game::stop_game();
					printError(static_cast<errorCodes>(a));//a had to be cast here because it is an int but need it to be an errorCode
					return a;
				}
				int b = g->after_round();//call after round
				while (b != success) //if everyone leaves end the game
				{
					string response;
					while (response != "y" && response != "n")
					{
						cout << "Would you like to play a new game? Type y for yes, n for no" << endl;
						cin >> ws;
						cin >> response;
					}
					if (response == "y")
					{
						string game;
						while ((game != "FiveCardDraw") && (game != "SevenCardStud") && (game != "TexasHoldEm") && (game != "q"))
						{
							cout << "Please type the name of the game, or q for quit" << endl;
							cin >> ws;
							cin >> game;
						}
						if (game == "q")
						{
							g->stop_game(); 
							return success; 
						}
						else
						{
							g->stop_game(); 
							try
							{
								cout << game << endl;
								Game::start_game(game); 
								g = Game::instance(); 
								string addPlayer = "";
								while (addPlayer != "done") 
								{
									cout << "Type the name of the player you'd like to add. When you're done, type \"done\"" << endl;
									cin >> ws;
									cin >> addPlayer;
									if (addPlayer != "done")
									{
										try
										{
											g->add_player(addPlayer); //try to add each player 
										}
										catch (errorCodes)
										{
											cout << addPlayer << " is already playing" << endl;
										}
									}
								}
								if (g->numPlayers() == notEnoughPlayers)
								{
									cout << "Only one valid player supplied" << endl;
									return not_enough_players;
								}
								while (g->numPlayers() > notEnoughPlayers) //while the number of players greater than 1 keep playing
								{
									int z = g->before_round(); //call before round 
									if (z != success) //if before round is not successful stop the game and return 
									{
										Game::stop_game();
										printError(static_cast<errorCodes>(z)); //z had to be cast here because it is an int but need it to be an errorCode
										return z;
									}
									int a = g->round(); //call round
									if (a != success)  //if round is not successful stop the game and return 
									{
										Game::stop_game();
										printError(static_cast<errorCodes>(a));//a had to be cast here because it is an int but need it to be an errorCode
										return a;
									}
									int b = g->after_round();//call after round
								}
							}
							catch (errorCodes i)
							{
								printError(i);
								cout << "game failed" << endl;
								return i;
							}
						}
					}
					else if (response == "n")
					{
						g->stop_game();
						return success;
					}
				}
			}
			g->stop_game(); //if out here, that means there are fewer than 2 players so end the game and return success
			return success;
		}
		catch (errorCodes i) //if an error is caught here, print it and return 
		{
			printError(i);
			return i;
		}
		catch (...) //if something is caught it is unknown, but end the program 
		{
			cout << "unknown error caught" << endl;
			return unknown;
		}
	}
	else //if there are less than 4 arguments, print the usage message
	{
		string errMsg = "Too few arguments, please provide a game name followed by two or more players";
		usageMsg(program_name, errMsg);
		return errorCodes::too_few_cmd_line;
	}

}

