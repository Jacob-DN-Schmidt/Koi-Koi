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
	int rounds_;
	std::string ruleset_;
	Player players_[2];
	bool oya_;
	Hanafuda_Deck deck_;
	std::deque<Hanafuda_Card*> table_;
	enum players { Player1 = false, Player2 = true };
	bool turn_;
	std::array<bool, 2> koiCalled = { false, false };

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
	void startGame();
	void startRound();
	void resetRound();
	void playerTurn();

	//------------------------------------------------------------------------------------------------------
	// Game Actions
	//------------------------------------------------------------------------------------------------------
	void drawFromDeck();
	void dealCards();
	bool validateTable() const;

	std::vector<int> checkMatch(const Hanafuda_Card* card);
	int nextMatch(const Hanafuda_Card* card);

	std::string gamestate();
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