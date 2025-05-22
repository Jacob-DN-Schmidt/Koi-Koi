#ifndef KOIKOI_GAME_H
#define KOIKOI_GAME_H

#include "Hanafuda_Card.h"
#include "Hanafuda_Deck.h"
#include "KoiKoi_Display.h"
#include "KoiKoi_Player.h"
#include <array>
#include <cstdlib>
#include <deque>
#include <string>
#include <vector>

#ifdef CONSOLE_PLAY
#include <cstdio>
#include <iosfwd>
#include <iostream>
#endif // CONSOLE_PLAY


class KoiKoi_Game {
private:
	enum players { Player1 = false, Player2 = true };

	int rounds_;
	std::string ruleset_;

	Player players_[2];
	bool oya_;
	bool turn_;
	std::array<bool, 2> koiCalled = { false, false };

	Hanafuda_Deck deck_;
	std::deque<Hanafuda_Card*> table_;
	
	KoiKoi_Display display_;

#ifdef CONSOLE_PLAY
	void pauseToSwap() {
		std::system("CLS");
		std::cout << "Swap player and press enter to continue";
		(void) getchar();
		std::system("CLS");
	};

	void pause() {
		std::cout << "Press enter to continue\n";
		(void) getchar();
	};

	void refresh() {
		std::system("CLS");
		std::cout << "Player " << (this->turn_ + 1) << "'s turn\n";
		std::cout << this->toFormattedString();
	};
#endif // CONSOLE_PLAY

public:
	//------------------------------------------------------------------------------------------------------
	// Game Flow Logic
	//------------------------------------------------------------------------------------------------------

	// KOIKOI_DOUBLE_SCORE, SEVEN_POINTS_DOUBLE_SCORE, COUNT_RIBBON_SET_AS_FIVE, SPOIL_VIEWING, CAN_COUNT_SAKE_AS_JUNK, TESHI_KUTTSUKI
	KoiKoi_Game(int rounds = 6, std::string ruleset = "110011") : rounds_(rounds), ruleset_(ruleset), players_(), deck_(Hanafuda_Deck()), table_(std::deque<Hanafuda_Card*>()), turn_(rand() % 2), oya_(false), display_() { this->deck_.shuffle(); display_.initiateWindow(); };
	// Starts a new game of Koi-Koi using a predefined ruleset and number of rounds
	void startGame();
	// Starts a new round of Koi-Koi by dealing out cards
	void startRound();
	// Clears each player's hands and the table, and resets the deck of cards
	void resetRound();
	// Starts the next player's turn based on the turn attribute (player at 0 for false, player at 1 for true)
	void playerTurn();


	//------------------------------------------------------------------------------------------------------
	// Game Actions
	//------------------------------------------------------------------------------------------------------

	// Draws a card from the deck for the player whose turn it is 
	void drawFromDeck();
	// Deals cards to the players and table in the order of non-oya -> table -> oya
	void dealCards();
	// Checks if the table after dealing contains 4 cards of a single month, return false if so, otherwise returns true
	bool validateTable() const;

	// Returns a list of the indexes of the cards on the table with a month matching the specified card
	std::vector<int> checkMatch(const Hanafuda_Card* card);
	// Returns the index of the first card on the table with a month matching the specified card
	int nextMatch(const Hanafuda_Card* card);

	// Returns a string describing the state of the game from the perspective of the player whose turn it is
	// Formatting is specified in "bin\KoiKoi_Game_Image_Formatting.txt"
	std::string gamestate();
	// Returns a string containing a comma delimited list of the image IDs of each card on the table
	std::string tableImage();

#ifdef CONSOLE_PLAY
	std::string tableToFormattedString() const;
	std::string toFormattedString() {
		return "Opponent " + this->players_[!(this->turn_)].playedToFormattedString() + "\n" +
			this->tableToFormattedString() + "\nYour " + this->players_[this->turn_].playedToFormattedString() + "\n";
	}


	friend std::ostream& operator<<(std::ostream& os, KoiKoi_Game& game) {
		return os << game.toFormattedString();
	};
#endif // CONSOLE_PLAY

};
#endif // !KOIKOI_GAME_H