#ifndef HANAFUDA_DECK_H
#define HANAFUDA_DECK_H

#include "Hanafuda_Card.h"
#include <deque>
#include <iosfwd>
#include <string>

class Hanafuda_Deck {
private:
	deque<Hanafuda_Card*> deck_;

	void initializeFromFile();

public:
	Hanafuda_Deck();
	/*Hanafuda_Deck(const Hanafuda_Deck& deck) :deck_() {
		for (auto itr = deck.deck_.cbegin(); itr != deck.deck_.cend(); itr++) {
			this->deck_.push_back(new Hanafuda_Card(*itr));
		};
	};*/
	~Hanafuda_Deck();
	void shuffle();
	Hanafuda_Card* dealCard();
	void reset();

	void swapPos(int first, int second) {
		Hanafuda_Card* temp = this->deck_[first]; this->deck_[first] = this->deck_[second]; this->deck_[second] = temp; temp = nullptr;
	};

	string deckImage() {
		string res = "";
		for (int i = 0; i < this->deck_.size() - 1; i++) {
			res += this->deck_[i]->getImgID() + ",";
		}
		res += this->deck_.back()->getImgID();
		return res;
	};

	friend ostream& operator<<(ostream& os, Hanafuda_Deck& deck);
};
#endif // !HANAFUDA_DECK_H