#include "boost/algorithm/string/case_conv.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "Hanafuda_Card.h"
#include <array>
#include <deque>
#include <regex>
#include <string>
#include <iostream>

const array<string, 6> Hanafuda_Card::suitArr = {
		"Kasu", "Tanzaku", "Poetry Tanzaku", "Blue Tanzaku", "Tane", "Hikari"
};

const array<string, 12> Hanafuda_Card::monthArr = {
		"Pine", "Plum Blossom", "Cherry Blossom", "Wisteria",
		"Iris", "Peony", "Bush Clover", "Susuki Grass",
		"Chrysanthemum", "Maple", "Willow", "Paulownia"
};

void Hanafuda_Card::setImgID(int idSuffix) {
	if (!this->name_.empty()) {
		this->imgID_ = boost::algorithm::replace_all_copy(boost::algorithm::to_lower_copy(this->name_), " ", "_");
	}
	else {
		this->imgID_ = "M" + to_string(this->month_) + "_S" + to_string(this->suit_);

		if (idSuffix > -1) {
			this->imgID_ += "_";
			this->imgID_ += (char) ('A' + idSuffix);
		}
	}
}

Hanafuda_Card::Hanafuda_Card(string card, int idSuffix) {
	regex parseSpecial("(^[^;]*);(\\d+);(\\d+)$", regex::ECMAScript);
	this->name_ = regex_replace(card, parseSpecial, "$1");
	this->month_ = stoi(regex_replace(card, parseSpecial, "$2"));
	this->suit_ = stoi(regex_replace(card, parseSpecial, "$3"));
	this->setImgID(idSuffix);
}

void Hanafuda_Card::insert(Hanafuda_Card* card, deque<Hanafuda_Card*>& into) {
	if (into.size() == 0 || !Hanafuda_Card::less<Hanafuda_Card*>()(card, into.back())) into.push_back(card);
	else {
		for (auto itr = into.begin(); itr != into.end(); itr++) {
			if (Hanafuda_Card::less<Hanafuda_Card*>()(card, *itr)) {
				into.insert(itr, card);
				break;
			}
		}
	}
}