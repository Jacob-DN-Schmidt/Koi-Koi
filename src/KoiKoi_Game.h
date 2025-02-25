#ifndef KOIKOI_GAME_H
#define KOIKOI_GAME_H

#include "Hanafuda_Card.h"
#include "Hanafuda_Deck.h"
#include "KoiKoi_Display.h"
#include "KoiKoi_Game_Handler.h"
#include "KoiKoi_Player.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iosfwd>
#include <iostream>
#include <string>
#include <vector>

class KoiKoi_Game {
private:
	int rounds_;
	string ruleset_;
	Player players_[2];
	bool oya_;
	Hanafuda_Deck deck_;
	deque<Hanafuda_Card*> table_;
	enum players { Player1 = false, Player2 = true };
	bool turn_;

	KoiKoi_Display display_;

	void pauseToSwap() {
		system("CLS");
		cout << "Swap player and press enter to continue";
		(void) getchar();
		system("CLS");
	};
	void pause() {
		cout << "Press enter to continue\n";
		(void) getchar();
	}
	void refresh() {
		system("CLS");
		cout << "Player " << (this->turn_ + 1) << "'s turn\n";
		cout << this->toFormattedString();
	};

public:
	// KOIKOI_DOUBLE_SCORE, SEVEN_POINTS_DOUBLE_SCORE, COUNT_RIBBON_SET_AS_FIVE, SPOIL_VIEWING, CAN_COUNT_SAKE_AS_JUNK, TESHI_KUTTSUKI
	KoiKoi_Game(int rounds = 6, string ruleset = "110011") : rounds_(rounds), ruleset_(ruleset), players_(), deck_(Hanafuda_Deck()), table_(deque<Hanafuda_Card*>()), turn_(rand() % 2), oya_(false), display_() { this->deck_.shuffle(); display_.initiateWindow(); };
	void startGame();
	void startRound();
	void resetRound() { this->players_[Player1].clearCards(); this->players_[Player2].clearCards(); KoiKoi_Game_Handler::deleteDequeContent(this->table_); this->deck_.reset(); display_.clearTextures(); }
	//static Hanafuda_Card* promptToPlayCard(Player& player);
	void playerTurn();
	void drawFromDeck();
	bool validateTable() const {
		array<int, 12> monthCounts = KoiKoi_Game_Handler::tabulateMonths(this->table_);
		for (int i = 0; i < monthCounts.size(); i++)
			if (monthCounts[i] == 4)
				return false;
		return true;
	};

	void dealCards() {
		for (int i = 0; i < 8; i++) {
			this->players_[!oya_].drawCard(this->deck_);
			Hanafuda_Card::insert(this->deck_.dealCard(), this->table_);
			this->players_[oya_].drawCard(this->deck_);
		}
	};

	int nextMatch(const Hanafuda_Card* card) {
		for (int i = 0; i < this->table_.size(); i++) {
			if (this->table_[i]->match(card)) return i;
		}
		return -1;
	}

	vector<int> checkMatch(const Hanafuda_Card* card) {
		vector<int> indexes;
		for (int i = 0; i < this->table_.size(); i++) {
			if (this->table_[i]->match(card)) indexes.push_back(i);
		}
		return indexes;
	};

	string gamestate();
	string tableImage();
	string tableToFormattedString() const;
	string toFormattedString() {
		/*string res = "Opponent " + this->players_[!(this->turn_)].playedToFormattedString() + "\n";
		res += this->tableToFormattedString() + "\n";
		res += "Your " + this->players_[this->turn_].playedToFormattedString() + "\n";*/
		return "Opponent " + this->players_[!(this->turn_)].playedToFormattedString() + "\n" +
			this->tableToFormattedString() + "\nYour " + this->players_[this->turn_].playedToFormattedString() + "\n";
	}

	friend ostream& operator<<(ostream& os, KoiKoi_Game& game) {
		return os << game.toFormattedString();
	};
};
#endif // !KOIKOI_GAME_H