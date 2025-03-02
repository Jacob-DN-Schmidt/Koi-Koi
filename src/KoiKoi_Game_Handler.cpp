#include "Hanafuda_Card.h"
#include "KoiKoi_Game_Handler.h"
#include <array>
#include <boost/assign/list_of.hpp>
#include <deque>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

unordered_map<string, int, KoiKoi_Game_Handler::KeyHasher> KoiKoi_Game_Handler::flagMap =
boost::assign::map_list_of("", -1)("Ono no Michikaze", hasOnoNoMichikaze)("Full Moon", hasFullMoon)
("Curtain", hasCurtain)("Sake Cup", hasSakeCup)("Lighting", hasLighting)("Boar", hasBoar)
("Deer", hasDeer)("Butterfly", hasButterfly)("Chinese Phoenix", -1)("Crane and Sun", -1)
("Warbling White-eye", -1)("Cuckoo", -1)("Eight-plank Bridge", -1)("Geese", -1)("Swallow", -1);

bool KoiKoi_Game_Handler::KOIKOI_DOUBLE_SCORE;
bool KoiKoi_Game_Handler::SEVEN_POINTS_DOUBLE_SCORE;
bool KoiKoi_Game_Handler::COUNT_RIBBON_SET_AS_FIVE;
bool KoiKoi_Game_Handler::SPOIL_VIEWING;
bool KoiKoi_Game_Handler::CAN_COUNT_SAKE_AS_JUNK;
bool KoiKoi_Game_Handler::TESHI_KUTTSUKI;

KoiKoi_Game_Handler::TabulationArray KoiKoi_Game_Handler::tabulation;

int KoiKoi_Game_Handler::eval(const deque<Hanafuda_Card*> hand, bool opponentKoiKoi) {
	tabulate(hand);
	int score = allHands();
	if (KOIKOI_DOUBLE_SCORE && opponentKoiKoi) score *= 2;
	if (SEVEN_POINTS_DOUBLE_SCORE && score >= 7) score *= 2;
	tabulation.fill(0);
	return score;
}

bool KoiKoi_Game_Handler::promptYN(string question) {
	cout << question << ": y|n?\n";
	string ans = "";
	regex match("(^\\s|^)(y|n)([\\w\\s$]+|$)", regex::icase);
	while (!regex_match(ans, match)) {
		getline(cin, ans);
	}
	return ans.front() == 'y' || ans.front() == 'Y';
}

int KoiKoi_Game_Handler::promptToDecideMatch(const int& one, const int& two) {
	cout << "Enter card to match: " << (one + 1) << " or " << (two + 1) << "\n";
	string ans = "";
	int res = 0;
	string temp = "^(" + to_string(one + 1) + ")$|^(" + to_string(two + 1) + ")$";
	regex match(temp, regex::ECMAScript);
	while (true) {
		getline(cin, ans);
		if (regex_match(ans, match)) {
			return stoi(regex_replace(ans, match, "$1$2")) - 1;
		}
	}
}

Hanafuda_Card* KoiKoi_Game_Handler::popCardAt(deque<Hanafuda_Card*>& from, int at) {
	deque<Hanafuda_Card*>::iterator itr = from.begin() + at;
	Hanafuda_Card* temp = *itr;
	from.erase(itr);
	return temp;
}

array<int, 12> KoiKoi_Game_Handler::tabulateMonths(const deque<Hanafuda_Card*>& hand) {
	array<int, 12> res = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < hand.size(); i++) {
		res[hand[i]->getMonth()] += 1;
	}
	return res;
}

int KoiKoi_Game_Handler::teshiKuttsuki(const deque<Hanafuda_Card*>& hand) {
	if (!TESHI_KUTTSUKI) return 0;
	bool kuttsuki = true;
	array<int, 12> monthCounts = tabulateMonths(hand);
	array<int, 12>::iterator itr = monthCounts.begin();
	for (int i = 0; i < monthCounts.size(); i++, itr++) {
		if (*itr == 4) return 6;
		if (*itr % 2 == 1) kuttsuki = false;
	}
	if (kuttsuki) return 6;
	return 0;
}
