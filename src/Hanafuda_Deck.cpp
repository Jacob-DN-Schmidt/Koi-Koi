#include "Hanafuda_Card.h"
#include "Hanafuda_Deck.h"
#include <cstdlib>
#include <deque>
#include <exception>
#include <fstream>
#include <iosfwd>
#include <ostream>
#include <regex>
#include <string>

void Hanafuda_Deck::initializeFromFile() {
	std::fstream CardInfo;
	CardInfo.open("Hanafuda Cards/CardInfo.txt");
	std::regex parseMultKasu("^/(\\d+);(\\d+)$", std::regex::ECMAScript);
	std::string current;

	if (CardInfo.is_open()) {
		while (getline(CardInfo, current)) {
			if (current.at(0) == '/') {
				int reps = stoi(regex_replace(current, parseMultKasu, "$1"));
				int month = stoi(regex_replace(current, parseMultKasu, "$2"));
				int idSuffix = 0;

				for (int i = 0; i < reps; i++) {
					this->deck_.push_back(new Hanafuda_Card(month, 0, "", idSuffix));
					idSuffix++;
				}
			}
			else {
				this->deck_.push_back(new Hanafuda_Card(current));
			}
		}
	}
	else {
		throw new std::exception("failed to open 'CardInfo.txt'");
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

std::ostream& operator<<(std::ostream& os, Hanafuda_Deck& deck) {
	if (deck.deck_.size() == 0) return os << "Deck is empty";
	std::deque<Hanafuda_Card*>::iterator itr = deck.deck_.begin();
	for (int i = 0; i < deck.deck_.size() - 1; i++, itr++) {
		os << *itr << " | ";
	}
	os << *itr;
	return os;
}
