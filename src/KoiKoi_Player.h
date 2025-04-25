#ifndef PLAYER_H
#define PLAYER_H

#include "Hanafuda_Card.h"
#include "Hanafuda_Deck.h"
#include "KoiKoi_Game_Handler.h"
#include "KoiKoi_Display.h"
#include <deque>
#include <iosfwd>
#include <string>

class Player {
private:
	std::deque<Hanafuda_Card*> hand_;
	std::deque<Hanafuda_Card*> played_;
	int points_;
public:
	Player() :hand_(std::deque<Hanafuda_Card*>()), played_(std::deque<Hanafuda_Card*>()), points_(0) {};
	~Player();
	Hanafuda_Card* playCard(int i);
	void drawCard(Hanafuda_Deck& deck) { Hanafuda_Card::insert(deck.dealCard(), this->hand_); };

	int calcPlayedCardsVal(bool opponentKoiKoi) const { return KoiKoi_Game_Handler::eval(this->played_, opponentKoiKoi); };
	void addPoints(int pnts) { this->points_ += pnts; };
	void addToPlayed(Hanafuda_Card* card) { Hanafuda_Card::insert(card, this->played_); };
	std::deque<Hanafuda_Card*> getPlayed() const { return this->played_; };
	std::deque<Hanafuda_Card*> getHand() const { return this->hand_; };
	Hanafuda_Card* getCardAt(int i) { return this->hand_[i]; };
	int getPoints() const { return this->points_; };
	size_t getHandSize() const { return this->hand_.size(); };
	void clearCards();
	void reset() { this->clearCards(); this->points_ = 0; };

	std::string handImage() const;
	std::string playedImage() const;

	bool _selectCallKoi(KoiKoi_Display& display, std::string gamestate) {
		return display.promptCallKoi(gamestate);
	}

	int _selectPairMatch(KoiKoi_Display& display, int firstPair, int secondPair, int handSelection = -1) {
		return display.waitForTableSelection(firstPair, secondPair, handSelection);
	}

	std::array<int, 2> _selectMatch(KoiKoi_Display& display, std::string gamestate) {
		return display.waitForSelection(gamestate);
	}

#ifdef CONSOLE_DEBUG
	Hanafuda_Card* promptToPlayCard();
	std::string handToFormattedString() const;
	std::string playedToFormattedString() const;
#endif

	/*string toFormattedString() {
		string res = "";
		deque<Hanafuda_Card*>::iterator itr = this->hand_.begin();
		for (int i = 0; i < this->hand_.size() - 1; i++, itr++) {
			res += (*itr)->toFormattedString() + " | ";
		}
		res += (*itr)->toFormattedString() + "\n";
		return res;
	};*/

	//friend ostream& operator<<(ostream& os, Player& player) {
	//	return os << player.handToFormattedString();
	//};

	//friend bool operator==(const Player& lhs, const Player& rhs) {
	//	return lhs.hand_ == rhs.hand_ && lhs.played_ == rhs.played_ && lhs.points_ == rhs.points_;
	//};
	//friend bool operator!=(const Player& lhs, const Player& rhs) {
	//	return !(lhs == rhs);
	//}
};
#endif // !PLAYER_H