#ifndef KOIKOI_GAME_HANDLER_H
#define KOIKOI_GAME_HANDLER_H

#include "Hanafuda_Card.h"
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

class KoiKoi_Game_Handler {
private:
	struct KeyHasher {
		std::size_t operator()(const string& flag) const {
			return hash<string>()(flag);
		}
	};

	static unordered_map<string, int, KeyHasher> flagMap;

	enum flags {
		hasOnoNoMichikaze = 6, hasFullMoon = 7, hasCurtain = 8,
		hasSakeCup = 9, hasLighting = 10, hasBoar = 11,
		hasDeer = 12, hasButterfly = 13, countSakeAsJunk = 14
	};

	typedef array<int, 15> TabulationArray;
	static TabulationArray tabulation;

	static int junk() {
		return ((tabulation[Kasu] + tabulation[countSakeAsJunk]) > 9) *
			((tabulation[Kasu] + tabulation[countSakeAsJunk]) - 9);
	};

	static int ribbons() {
		int val = tabulation[Tanzaku] + tabulation[Poetry] + tabulation[Blue] - 4 + 
			(COUNT_RIBBON_SET_AS_FIVE && (tabulation[Poetry] == 3 || tabulation[Blue] == 3));
		val *= (val > 0);
		return val;
	};

	static int ribbonSet() {
		return 5 * ((tabulation[Poetry] == 3) + (tabulation[Blue] == 3));
	};

	static int tane() {
		return (tabulation[Tane] > 4) * (tabulation[Tane] - 4);
	};

	static int boarDeerButterfly() {
		return tabulation[hasBoar] * tabulation[hasDeer] * tabulation[hasButterfly] * 5;
	};
	static int viewing() {
		int val = tabulation[hasSakeCup] && !tabulation[countSakeAsJunk] && (!SPOIL_VIEWING || (tabulation[hasOnoNoMichikaze] && tabulation[hasLighting]));
		val *= 5 * (tabulation[hasFullMoon] + tabulation[hasCurtain]);
		return val;
	};

	static int allLights() {
		int val = ((tabulation[Hikari] == 3 || tabulation[Hikari] == 4) && !tabulation[hasOnoNoMichikaze]) * (4 * tabulation[Hikari] - 8) + 
			((tabulation[Hikari] == 4 || tabulation[Hikari] == 5) * tabulation[hasOnoNoMichikaze]) * (3 * tabulation[Hikari] - 5);
		return val;
	};

	static int allHands() {
		return junk() + ribbons() + ribbonSet() +
			tane() + boarDeerButterfly() +
			viewing() + allLights();
	};

	// Kasu, tanzaku, poetry, blue, tane, hikari, hasOnoNoMichikaze, hasFullMoon, hasCurtain, hasSakeCup, hasLighting, hasBoar, hasDeer, hasButterfly, countSakeAsJunk
	static void tabulate(const deque<Hanafuda_Card*> hand) {
		tabulation = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int curFlag = -1;
		Hanafuda_Card* temp;
		for (int i = 0; i < hand.size(); i++) {
			temp = hand[i];
			tabulation[temp->getSuit()]++;
			curFlag = flagMap[temp->getName()];
			if (curFlag != -1) tabulation[curFlag] = true;
		}
		if (tabulation[hasSakeCup] && tabulation[Kasu] >= 9 && CAN_COUNT_SAKE_AS_JUNK) {
			tabulation[countSakeAsJunk] = promptYN("Count Sake Cup as Junk"); //todo
		}
	}

public:
	static bool KOIKOI_DOUBLE_SCORE;
	static bool SEVEN_POINTS_DOUBLE_SCORE;
	static bool COUNT_RIBBON_SET_AS_FIVE;
	static bool SPOIL_VIEWING;
	static bool CAN_COUNT_SAKE_AS_JUNK;
	static bool TESHI_KUTTSUKI;

	static int eval(const deque<Hanafuda_Card*> hand, bool opponentKoiKoi = false);
	static bool promptYN(string question);
	static int promptToDecideMatch(const int& one, const int& two);
	static Hanafuda_Card* popCardAt(deque<Hanafuda_Card*>& from, int at);
	static array<int, 12> tabulateMonths(const deque<Hanafuda_Card*>& hand);
	static int teshiKuttsuki(const deque<Hanafuda_Card*>& hand);

	static void deleteDequeContent(deque<Hanafuda_Card*>& target) {
		for (int i = 0; i < target.size(); i++) {
			delete target[i];
		}
		target.clear();
	};

	static void setGameRuleDefault() {
		KOIKOI_DOUBLE_SCORE = true;
		SEVEN_POINTS_DOUBLE_SCORE = true;
		COUNT_RIBBON_SET_AS_FIVE = false;
		SPOIL_VIEWING = false;
		CAN_COUNT_SAKE_AS_JUNK = true;
		TESHI_KUTTSUKI = true;
	};

	// KOIKOI_DOUBLE_SCORE, SEVEN_POINTS_DOUBLE_SCORE, COUNT_RIBBON_SET_AS_FIVE, SPOIL_VIEWING, CAN_COUNT_SAKE_AS_JUNK
	static void setGameRule(string ruleState) {
		KOIKOI_DOUBLE_SCORE = ruleState.at(0) - 48;
		SEVEN_POINTS_DOUBLE_SCORE = ruleState.at(1) - 48;
		COUNT_RIBBON_SET_AS_FIVE = ruleState.at(2) - 48;
		SPOIL_VIEWING = ruleState.at(3) - 48;
		CAN_COUNT_SAKE_AS_JUNK = ruleState.at(4) - 48;
		TESHI_KUTTSUKI = ruleState.at(5) - 48;
	};

	static void printGameRule() {
		cout << KOIKOI_DOUBLE_SCORE << SEVEN_POINTS_DOUBLE_SCORE << COUNT_RIBBON_SET_AS_FIVE << SPOIL_VIEWING << CAN_COUNT_SAKE_AS_JUNK << "\n";
	}
};
#endif // !KOIKOI_GAME_HANDLER_H