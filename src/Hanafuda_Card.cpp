#include "Hanafuda_Card.h"
#include <array>
#include <boost/format.hpp>
#include <boost/format/format_fwd.hpp>
#include <boost/format/free_funcs.hpp>
#include <regex>
#include <string>

const array<string, 6> Hanafuda_Card::suitArr = {
		"Kasu", "Tanzaku", "Poetry Tanzaku", "Blue Tanzaku", "Tane", "Hikari"
};

const array<string, 12> Hanafuda_Card::monthArr = {
		"Pine", "Plum Blossom", "Cherry Blossom", "Wisteria",
		"Iris", "Peony", "Bush Clover", "Susuki Grass",
		"Chrysanthemum", "Maple", "Willow", "Paulownia"
};

Hanafuda_Card::Hanafuda_Card(string card, int idSuffix) {
	regex parseSpecial("(^[^;]*);(\\d+);(\\d+)$", regex::ECMAScript);
	this->name_ = regex_replace(card, parseSpecial, "$1");
	this->month_ = stoi(regex_replace(card, parseSpecial, "$2"));
	this->suit_ = stoi(regex_replace(card, parseSpecial, "$3"));
	this->imgID_ = "img_" + boost::str(boost::format("%02d") % idSuffix);
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