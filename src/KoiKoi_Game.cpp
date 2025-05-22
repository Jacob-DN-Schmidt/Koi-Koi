#include "Hanafuda_Card.h"
#include "KoiKoi_Game.h"
#include "KoiKoi_Game_Handler.h"
#include "KoiKoi_Player.h"
#include <array>
#include <deque>
#include <string>
#include <vector>

#ifdef CONSOLE_DEBUG
#include <iostream>
#endif // CONSOLE_DEBUG


//------------------------------------------------------------------------------------------------------
// Game Flow Logic
//------------------------------------------------------------------------------------------------------
void KoiKoi_Game::startGame() {
	KoiKoi_Game_Handler::setGameRule(this->ruleset_);

#ifdef CONSOLE_DEBUG
	std::cout << "Player " << (this->turn_ + 1) << " will start\n\n";
#endif // CONSOLE_DEBUG

	for (int i = 0; i < this->rounds_; i++) {
		this->startRound();
		resetRound();

#ifdef CONSOLE_DEBUG
		std::cout << "Player 1 points: " << to_string(players_[0].getPoints()) << "\nPlayer 2 points: " << to_string(players_[1].getPoints()) << "\n";
		std::cout << "Opp points: " + to_string(players_[!turn_].getPoints()) + "\nPlayer points:" + to_string(players_[turn_].getPoints()) + "\n\n";
#endif // CONSOLE_DEBUG

	}

}

void KoiKoi_Game::startRound() {
	oya_ = turn_;

#ifdef CONSOLE_DEBUG
	std::cout << "Opp points: " + to_string(players_[!turn_].getPoints()) + "\nPlayer points:" + to_string(players_[turn_].getPoints()) + "\n\n";
#endif // CONSOLE_DEBUG

	while (true) {
		this->dealCards();

		if (!this->validateTable()) {

#ifdef CONSOLE_DEBUG
			std::cout << "invalid table, resetting round\n";
#endif // CONSOLE_DEBUG

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

#ifdef CONSOLE_DEBUG
			std::cout << "Player " << (i + 1) << " Teyaku!\n";
#endif CONSOLE_DEBUG

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
	display_.pause(this->gamestate());
	if (end) return;


	// Player1 round points, Player2 round points
	int roundPoints[] = { 0, 0 };
	// Player1 called, Player2 called
	//bool koiCalled[] = { false, false };
	while (true) {

		if (players_[oya_].getHandSize() == 0 && players_[!oya_].getHandSize() == 0) {

#ifdef CONSOLE_DEBUG
			std::cout << "No hands were made! Oya's authority\n\n";
#endif // CONSOLE_DEBUG

			players_[oya_].addPoints(6);
			this->turn_ = oya_;
			return;
		}

#ifdef CONSOLE_DEBUG
		std::cout << "Player " << (this->turn_ + 1) << "'s turn\n" << this->toFormattedString() << players_[turn_];
#endif

		Player& current = this->players_[this->turn_];
		playerTurn();
		drawFromDeck();
		display_.pause(this->gamestate());
		int currentHandEval = current.calcPlayedCardsVal(koiCalled[!turn_]);

		if (roundPoints[turn_] != currentHandEval) {
			roundPoints[turn_] = currentHandEval;

			if (current.getHandSize() != 0) {

#ifdef CONSOLE_DEBUG
				std::cout << current << "\nCurrent played cards value: " << to_string(currentHandEval) << "\n";
#endif // CONSOLE_DEBUG

				koiCalled[turn_] = current._selectCallKoi(display_, this->gamestate());

				if (!koiCalled[turn_]) {
					current.addPoints(currentHandEval);

#ifdef CONSOLE_DEBUG
					cout << "Player " << (this->turn_ + 1) << " scored " << to_string(currentHandEval) << " points\n\n";
#endif // CONSOLE_DEBUG

					return;
				}
			}
			else {
				current.addPoints(currentHandEval);

#ifdef CONSOLE_DEBUG
				cout << "Player " << (this->turn_ + 1) << " scored " << to_string(currentHandEval) << " points\n\n";
#endif // CONSOLE_DEBUG

				return;
			}
		}
		turn_ = !turn_;
	}
}

void KoiKoi_Game::resetRound() { 
	this->players_[Player1].clearCards(); 
	this->players_[Player2].clearCards(); 
	KoiKoi_Game_Handler::deleteDequeContent(this->table_); 
	this->deck_.reset();

	display_.clearAllTextures();

	koiCalled[0] = false;
	koiCalled[1] = false;
}

void KoiKoi_Game::playerTurn() {
	Player& player = players_[turn_];
	std::array<int, 2> choices = player._selectMatch(display_, this->gamestate());
	Hanafuda_Card* cardFromHand = player.playCard(choices[0]);
	std::vector<int> matches = this->checkMatch(cardFromHand);

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
		int tempChoice;

		if (cardFromHand->match(this->table_[choices[1]])) {
			tempChoice = choices[1];
		}
		else {
			tempChoice = player._selectPairMatch(display_, matches[0], matches[1], choices[0]);
		}

		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, tempChoice));
	}
	else {
		player.addToPlayed(cardFromHand);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, matches[0]));
	}
}


//------------------------------------------------------------------------------------------------------
// Game Actions
//------------------------------------------------------------------------------------------------------

void KoiKoi_Game::drawFromDeck() {

	Player& player = players_[turn_];
	Hanafuda_Card* temp = this->deck_.dealCard();

#ifdef CONSOLE_DEBUG
	cout << "Card drawn from deck: " << temp->toFormattedString() << "\n";
#endif // CONSOLE_DEBUG

	std::vector<int> matches = this->checkMatch(temp);
	display_.updateGamestate(this->gamestate());

	if (matches.size() == 0) {
		Hanafuda_Card::insert(temp, this->table_);

#ifdef CONSOLE_DEBUG
		cout << "No matches: added to table\n\n";
#endif // CONSOLE_DEBUG

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
		int ans = player._selectPairMatch(display_, matches[0], matches[1]);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, ans));
	}
	else {
		player.addToPlayed(temp);
		player.addToPlayed(KoiKoi_Game_Handler::popCardAt(this->table_, matches[0]));

#ifdef CONSOLE_DEBUG
		cout << "1 match: added to played cards\n\n";
#endif // CONSOLE_DEBUG

	}
}

void KoiKoi_Game::dealCards() {
	for (int i = 0; i < 8; i++) {
		this->players_[!oya_].drawCard(this->deck_);
		Hanafuda_Card::insert(this->deck_.dealCard(), this->table_);
		this->players_[oya_].drawCard(this->deck_);
	}
}

bool KoiKoi_Game::validateTable() const {
	std::array<int, 12> monthCounts = KoiKoi_Game_Handler::tabulateMonths(this->table_);
	for (int i = 0; i < monthCounts.size(); i++)
		if (monthCounts[i] == 4)
			return false;
	return true;
}

std::vector<int> KoiKoi_Game::checkMatch(const Hanafuda_Card* card) {
	std::vector<int> indexes;
	for (int i = 0; i < this->table_.size(); i++) {
		if (this->table_[i]->match(card)) indexes.push_back(i);
	}
	return indexes;
}

int KoiKoi_Game::nextMatch(const Hanafuda_Card* card) {
	for (int i = 0; i < this->table_.size(); i++) {
		if (this->table_[i]->match(card)) return i;
	}
	return -1;
}

std::string KoiKoi_Game::gamestate() {
	std::string res = "";
	res += std::to_string(players_[!turn_].getHandSize()) + "/";
	res += players_[!turn_].playedImage() + "/";
	res += players_[turn_].handImage() + "/";
	res += players_[turn_].playedImage() + "/";
	res += tableImage() + "/";
	res += std::to_string(players_[!turn_].getPoints()) + "/";
	res += std::to_string(players_[turn_].getPoints()) + "/";
	res += std::to_string(oya_ == turn_) + "/";
	res += std::to_string(koiCalled[0] || koiCalled[1]) + std::to_string(koiCalled[turn_]);
	return res;
}

std::string KoiKoi_Game::tableImage() {
	std::string res = "";
	for (int i = 0; i < table_.size() - 1; i++) {
		res += table_[i]->getImgID() + ",";
	}
	res += table_.back()->getImgID();
	return res;
}

#ifdef CONSOLE_PLAY
std::string KoiKoi_Game::tableToFormattedString() const {
	if (this->table_.size() < 1) return "Table is empty\n";
	std::string res = "Table:\n";
	int cardNum;
	std::deque<Hanafuda_Card*>::const_iterator itr = this->table_.begin();
	for (cardNum = 1; cardNum < this->table_.size(); cardNum++, itr++) {
		res += std::to_string(cardNum) + ": " + (*itr)->toFormattedString() + " | ";
	}
	res += std::to_string(cardNum) + ": " + (*itr)->toFormattedString() + "\n";
	return res;
}
#endif // CONSOLE_PLAY