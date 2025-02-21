#include "Hanafuda_Card.h"
#include "Hanafuda_Deck.h"
#include <boost/format.hpp>
#include <boost/format/format_fwd.hpp>
#include <boost/format/free_funcs.hpp>
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <iosfwd>
#include <ostream>
#include <regex>
#include <string>

void Hanafuda_Deck::initializeFromFile() {
	fstream CardInfo;
	CardInfo.open("CardInfo.txt");
	regex parseMultKasu("^/(\\d+);(\\d+)$", regex::ECMAScript);
	string name;
	int month;
	int reps;
	int idSuffix = 1;
	string current;
	if (CardInfo.is_open()) {
		while (getline(CardInfo, current)) {
			if (current.at(0) == '/') {
				//cout << "mult\n";
				reps = stoi(regex_replace(current, parseMultKasu, "$1"));
				month = stoi(regex_replace(current, parseMultKasu, "$2"));
				for (int i = 0; i < reps; i++) {
					this->deck_.push_back(new Hanafuda_Card(month, 0, "", "img_" + boost::str(boost::format("%02d") % idSuffix)));
					idSuffix++;
				}
			}
			else {
				//cout << "special\n";
				this->deck_.push_back(new Hanafuda_Card(current, idSuffix));
				idSuffix++;
			}
		}
	}
	else {
		throw new exception("failed to open 'Special_Card.txt'");
	}
}

Hanafuda_Deck::Hanafuda_Deck() {
	this->initializeFromFile();
}

Hanafuda_Deck::~Hanafuda_Deck() {
	for (int i = 0; i < deck_.size(); i++) {
		delete deck_[i];
	}
	deck_.~deque();
}

void Hanafuda_Deck::shuffle() {
	for (int i = deck_.size() - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		this->swapPos(j, i);
	}
}

Hanafuda_Card* Hanafuda_Deck::dealCard() {
	Hanafuda_Card* temp = this->deck_.front();
	this->deck_.pop_front();
	return temp;
}

void Hanafuda_Deck::reset() {
	for (int i = 0; i < this->deck_.size(); i++) {
		delete this->deck_[i];
	}
	deck_.clear();
	this->initializeFromFile();
	this->shuffle();
}

ostream& operator<<(ostream& os, Hanafuda_Deck& deck) {
	if (deck.deck_.size() == 0) return os << "Deck is empty";
	deque<Hanafuda_Card*>::iterator itr = deck.deck_.begin();
	for (int i = 0; i < deck.deck_.size() - 1; i++, itr++) {
		os << *itr << " | ";
	}
	os << *itr;
	return os;
}
