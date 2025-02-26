#include "Hanafuda_Card.h"
#include "KoiKoi_Player.h"
#include <deque>
#include <iostream>
#include <regex>
#include <string>

Player::~Player() {
	for (int i = 0; i < this->hand_.size(); i++) {
		delete this->hand_[i];
	}
	this->hand_.~deque();
	for (int i = 0; i < this->played_.size(); i++) {
		delete this->played_[i];
	}
	this->played_.~deque();
}

Hanafuda_Card* Player::playCard(int i) {
	deque<Hanafuda_Card*>::iterator itr = this->hand_.begin() + i;
	Hanafuda_Card* temp = *itr;
	this->hand_.erase(itr);
	return temp;
}

void Player::clearCards() {
	for (int i = 0; i < this->hand_.size(); i++) {
		delete this->hand_[i];
	}
	this->hand_.clear();
	for (int i = 0; i < this->played_.size(); i++) {
		delete this->played_[i];
	}
	this->played_.clear();
}

Hanafuda_Card* Player::promptToPlayCard() {
	cout << this->handToFormattedString() << "\nWhich card would you like to play?\n";
	string ans = "";
	regex match("([0-9]+)", regex::ECMAScript);
	while (true) {
		getline(cin, ans);
		if (regex_match(ans, match)) {
			int res = stoi(regex_replace(ans, match, "$1"));
			if (res <= this->hand_.size()) {
				return this->playCard(res - 1);
			}
		}
	}
}

string Player::handToFormattedString() const {
	if (this->hand_.size() == 0) return "Hand: empty\n";
	string res = "Hand:\n";
	int cardNum = 1;
	deque<Hanafuda_Card*>::const_iterator itr = this->hand_.begin();
	for (; cardNum < this->hand_.size(); cardNum++, itr++) {
		res += cardNum + ": " + (*itr)->toFormattedString() + " | ";
	}
	res += cardNum + ": " + (*itr)->toFormattedString() + "\n";
	return res;
}

string Player::playedToFormattedString() const {
	if (this->played_.size() < 1) return "Played cards: empty\n";
	string res = "Played cards:\n";
	int cardNum;
	deque<Hanafuda_Card*>::const_iterator itr = this->played_.begin();
	for (cardNum = 1; cardNum < this->played_.size(); cardNum++, itr++) {
		res += to_string(cardNum) + ": " + (*itr)->toFormattedString() + " | ";
	}
	res += to_string(cardNum) + ": " + (*itr)->toFormattedString() + "\n";
	return res;
}

string Player::handImage() const {
	string res = "";
	if (this->hand_.size() == 0) return res;
	for (int i = 0; i < this->hand_.size() - 1; i++) {
		res += this->hand_[i]->getImgID() + ",";
	}
	res += this->hand_.back()->getImgID();
	return res;
}

string Player::playedImage() const {
	string res = "";
	if (this->played_.size() == 0) return res;
	for (int i = 0; i < this->played_.size() - 1; i++) {
		if (this->played_[i]->getSuit() == 0) res += "J";
		res += this->played_[i]->getImgID() + ",";
	}
	res += this->played_.back()->getImgID();
	return res;
}
