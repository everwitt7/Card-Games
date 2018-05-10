#pragma once
//Lab2.h: Header file containing declaration errorCodes enum and usageMsg function
//Author: Mia Bendy (mia.bendy@wustl.edu)

#include <random>

//usage message function declaration 
void usageMsg(std::string program_name, std::string msg);

//Enum for error codes 
enum errorCodes {
	success = 0,
	file_not_open,
	index_out_of_bounds,
	instance_not_available,
	game_already_started,
	unknown_game,
	no_game_in_progress,
	already_playing,
	empty_decks,
	no_cmd_line,
	one_incorrect_cmd_line,
	two_incorrect_cmd_line,
	too_many_cmd_line,
	too_few_cmd_line,
	deck_empty,
	no_chips,
	incorrect_vector_size,
	no_players,
	not_enough_players,
	too_many_players,
	unknown
};

//declaration of a function to print an error 
void printError(errorCodes);

