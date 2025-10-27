#ifndef HANAFUDA_CARD_H
#define HANAFUDA_CARD_H

#include <array>
#include <deque>
#include <string>

#ifdef CONSOLE_DEBUG
#include <iosfwd>
#endif // CONSOLE_DEBUG

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
	std::string name_;
	std::string imgID_;

	void setImgID(int idSuffix);

public:
	static const std::array<std::string, 12> monthArr;
	static const std::array<std::string, 6> suitArr;

	template <class _Tp = Hanafuda_Card*>
	struct less {
		constexpr bool operator()(const _Tp& lhs, const _Tp& rhs) const {
			return (lhs->getSuit() < rhs->getSuit()) || (lhs->getSuit() == rhs->getSuit() && lhs->getMonth() < rhs->getMonth());
		}
	};

	Hanafuda_Card(int setMonth = 0, int setSuit = 0, std::string setName = "", int idSuffix = -1) :month_(setMonth), suit_(setSuit), name_(setName), imgID_() { this->setImgID(idSuffix); };
	Hanafuda_Card(std::string card, int idSuffix = -1);

	int getMonth() const { return this->month_; };
	int getSuit() const { return this->suit_; };
	std::string getName() const { return this->name_; };
	std::string getImgID() const { return this->imgID_; };
	std::string getCardID() const { return std::to_string(this->month_) + "_" + std::to_string(this->suit_); };

	// bool equal(int month, int suit) const { return this->month_ == month && this->suit_ == suit; };
	bool match(const Hanafuda_Card* rhs) const { return this->month_ == rhs->month_; };
	static void insert(Hanafuda_Card* card, std::deque<Hanafuda_Card*>& into);

#ifdef CONSOLE_PLAY
	std::string toFormattedString() const {
		if (this->name_.empty()) {
			return Hanafuda_Card::monthArr[this->month_] + " " + Hanafuda_Card::suitArr[this->suit_];
		}
		return Hanafuda_Card::monthArr[this->month_] + " " + this->name_;
	};

	//friend std::ostream& operator<<(std::ostream& os, const Hanafuda_Card& card) {
	//	return os << card.toFormattedString();
	//};

	//friend std::ostream& operator<<(std::ostream& os, const Hanafuda_Card* card) {
	//	return os << card->toFormattedString();
	//};
#endif // CONSOLE_PLAY

};
#endif // !HANAFUDA_CARD_H