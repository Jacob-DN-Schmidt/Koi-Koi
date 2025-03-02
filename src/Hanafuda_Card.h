#ifndef HANAFUDA_CARD_H
#define HANAFUDA_CARD_H

#include <array>
#include <deque>
#include <iosfwd>
#include <string>

using namespace std;

const enum suit { Kasu, Tanzaku, Poetry, Blue, Tane, Hikari };
//const enum month {
//	Pine, Plum_Blossom, Cherry_Blossom, Wisteria,
//	Iris, Peony, Bush_Clover, Susuki_Grass,
//	Chrysanthemum, Maple, Willow, Paulownia
//};

class Hanafuda_Card {
private:
	int month_;
	int suit_;
	string name_;
	string imgID_;

	void setImgID(int idSuffix);

public:
	static const array<string, 12> monthArr;
	static const array<string, 6> suitArr;

	template <class _Tp = Hanafuda_Card*>
	struct less {
		constexpr bool operator()(const _Tp& lhs, const _Tp& rhs) const {
			return (lhs->getSuit() < rhs->getSuit()) || (lhs->getSuit() == rhs->getSuit() && lhs->getMonth() < rhs->getMonth());
		}
	};

	Hanafuda_Card(int setMonth = 0, int setSuit = 0, string setName = "", int idSuffix = -1) :month_(setMonth), suit_(setSuit), name_(setName), imgID_() { this->setImgID(idSuffix); };
	Hanafuda_Card(string card, int idSuffix = -1);

	int getMonth() const { return this->month_; };
	int getSuit() const { return this->suit_; };
	string getName() const { return this->name_; };
	string getImgID() const { return this->imgID_; };

	// bool equal(int month, int suit) const { return this->month_ == month && this->suit_ == suit; };
	bool match(const Hanafuda_Card* rhs) const { return this->month_ == rhs->month_; };
	static void insert(Hanafuda_Card* card, deque<Hanafuda_Card*>& into);

	string toFormattedString() const {
		if (this->name_.empty()) {
			return Hanafuda_Card::monthArr[this->month_] + " " + Hanafuda_Card::suitArr[this->suit_];
		}
		return Hanafuda_Card::monthArr[this->month_] + " " + this->name_;
	};

	friend ostream& operator<<(ostream& os, const Hanafuda_Card& card) {
		return os << card.toFormattedString();
	};

	friend ostream& operator<<(ostream& os, const Hanafuda_Card* card) {
		return os << card->toFormattedString();
	};
};
#endif // !HANAFUDA_CARD_H