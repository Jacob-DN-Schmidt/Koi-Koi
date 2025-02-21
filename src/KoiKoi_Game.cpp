#include "Hanafuda_Card.h"
#include "KoiKoi_Game.h"
#include "KoiKoi_Game_Handler.h"
#include "Player.h"
#include <array>
#include <deque>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

//Hanafuda_Card* KoiKoi_Game::promptToPlayCard(Player& player) {
//	cout << player << "\nWhich card would you like to play?\n";
//	string ans = "";
//	regex match("([0-9]+)", regex::ECMAScript);
//	while (true) {
//		getline(cin, ans);
//		if (regex_match(ans, match)) {
//			int res = stoi(regex_replace(ans, match, "$1"));
//			if (res <= player.getHandSize()) {
//				return player.playCard(res - 1);
//			}
//		}
//	}
//}

void KoiKoi_Game::startGame() {
	KoiKoi_Game_Handler::setGameRule(this->ruleset_);
	cout << "Player " << (this->turn_ + 1) << " will start\n\n";


	for (int i = 0; i < this->rounds_; i++) {
		this->startRound();
		resetRound();
		cout << "Player 1 points: " << to_string(players_[0].getPoints()) << "\nPlayer 2 points: " << to_string(players_[1].getPoints()) << "\n";
		cout << "Opp points: " + to_string(players_[!turn_].getPoints()) + "\nPlayer points:" + to_string(players_[turn_].getPoints()) + "\n\n";

	}

}

void KoiKoi_Game::startRound() {
	oya_ = turn_;
	cout << "Opp points: " + to_string(players_[!turn_].getPoints()) + "\nPlayer points:" + to_string(players_[turn_].getPoints()) + "\n\n";

	while (true) {
		this->dealCards();
		if (!this->validateTable()) {
			cout << "invalid table, resetting round\n";
			this->resetRound();
		}
		else {
			break;
		}
	}

	bool end = false;
	for (int i = 0; i < 2; i++) {
		int teshiKuttsukiPoints = KoiKoi_Game_Handler::teshiKuttsuki(players_[i].getHand());
		if (teshiKuttsukiPoints != 0) {
			cout << "Player " << (i + 1) << " Teyaku!\n";
			players_[i].addPoints(teshiKuttsukiPoints);
			if (!end) {
				end = true;
				this->turn_ = i;
			}
			else {
				this->turn_ = (oya_ != false);
			}
		}
	}
	if (end) return;


	// Player1 round points, Player2 round points
	int roundPoints[] = { 0, 0 };
	// Player1 called, Player2 called
	bool koikoiCalled[] = { false, false };
	while (true) {
		if (players_[oya_].getHandSize() == 0 && players_[!oya_].getHandSize() == 0) {

			cout << "No hands were made! Oya's authority\n\n";
			players_[oya_].addPoints(6);
			this->turn_ = oya_;

			return;
		}
		cout << "Player " << (this->turn_ + 1) << "'s turn\n" << this->toFormattedString() << players_[turn_];
		Player& current = this->players_[this->turn_];
		playerTurn();
		drawFromDeck();
		display_.pause(this->gamestate());
		int currentHandEval = current.calcPlayedCardsVal(koikoiCalled[!turn_]);
		if (roundPoints[turn_] != currentHandEval) {
			roundPoints[turn_] = currentHandEval;
			if (current.getHandSize() != 0) {
				cout << current << "\nCurrent played cards value: " << to_string(currentHandEval) << "\n";
				koikoiCalled[turn_] = display_.promptCallKoi(this->gamestate());
				if (!koikoiCalled[turn_]) {
					current.addPoints(currentHandEval);

					cout << "Player " << (this->turn_ + 1) << " scored " << to_string(currentHandEval) << " points\n\n";

					return;
				}
			}
			else {
				current.addPoints(currentHandEval);

				cout << "Player " << (this->turn_ + 1) << " scored " << to_string(currentHandEval) << " points\n\n";

				return;
			}
		}
		turn_ = !turn_;
	}

}

void KoiKoi_Game::playerTurn() {
	Player& player = players_[turn_];
	array<int, 2> choices = display_.waitForSelection(this->gamestate());
	Hanafuda_Card* cardFromHand = player.playCard(choices[0]);
	vector<int> matches = this->checkMatch(cardFromHand);
	if (matches.size() == 0) {
		Hanafuda_Card::insert(cardFromHand, this->table_);
	}
	else if (matches.size() == 3) {
		player.addToPlayed(cardFromHand);
		int nextIndex = nextMatch(cardFromHand);
		while (nextIndex != -1) {
			player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, nextIndex));
			nextIndex = nextMatch(cardFromHand);
		}
	}
	else if (matches.size() == 2) {
		player.addToPlayed(cardFromHand);
		int tempChoice = choices[1];
		display_.waitForTableSelection(matches[0], matches[1], choices[0]);

		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, choices[1]));
	}
	else {
		player.addToPlayed(cardFromHand);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, matches[0]));
	}
}

void KoiKoi_Game::drawFromDeck() {

	Player& player = players_[turn_];
	Hanafuda_Card* temp = this->deck_.dealCard();
	cout << "Card drawn from deck: " << temp->toFormattedString() << "\n";
	vector<int> matches = this->checkMatch(temp);
	display_.updateGamestate(this->gamestate());
	if (matches.size() == 0) {
		Hanafuda_Card::insert(temp, this->table_);
		cout << "No matches: added to table\n\n";
	}
	else if (matches.size() == 3) {
		player.addToPlayed(temp);
		int nextIndex = nextMatch(temp);
		while (nextIndex != -1) {
			player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, nextIndex));
			nextIndex = nextMatch(temp);
		}
	}
	else if (matches.size() == 2) {
		player.addToPlayed(temp);
		int ans = display_.waitForTableSelection(matches[0], matches[1]);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, ans));
	}
	else {
		player.addToPlayed(temp);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, matches[0]));
		cout << "1 match: added to played cards\n\n";
	}


}

string KoiKoi_Game::gamestate() {
	string res = "";
	res += to_string(players_[!turn_].getHandSize()) + "/";
	res += players_[!turn_].playedImage() + "/";
	res += players_[turn_].handImage() + "/";
	res += players_[turn_].playedImage() + "/";
	res += tableImage() + "/";
	res += to_string(players_[!turn_].getPoints()) + "/";
	res += to_string(players_[turn_].getPoints()) + "/";
	return res;
}

string KoiKoi_Game::tableImage() {
	string res = "";
	for (int i = 0; i < table_.size() - 1; i++) {
		res += table_[i]->getImgID() + ",";
	}
	res += table_.back()->getImgID();
	return res;
}

string KoiKoi_Game::tableToFormattedString() const {
	if (this->table_.size() < 1) return "Table is empty\n";
	string res = "Table:\n";
	int cardNum;
	deque<Hanafuda_Card*>::const_iterator itr = this->table_.begin();
	for (cardNum = 1; cardNum < this->table_.size(); cardNum++, itr++) {
		res += to_string(cardNum) + ": " + (*itr)->toFormattedString() + " | ";
	}
	res += to_string(cardNum) + ": " + (*itr)->toFormattedString() + "\n";
	return res;
}