#ifndef KOIKOI_PROBABILITY_EVAL_H
#define KOIKOI_PROBABILITY_EVAL_H

#include <string>
#include <deque>
#include <boost/math/special_functions/factorials.hpp>
#include <stdexcept>

class KoiKoi_Probablility_Eval {
public:
	std::string gamestate_;
	std::deque<std::string> play_;

	int cardsRemainingInDeck();

	static double nChoosek(double n, double k) {
		if (k > n) {
			throw std::invalid_argument("sample size k cannot be larger than pool size n");
		}
		double temp = boost::math::factorial<double>(k) * boost::math::factorial<double>(n - k);
		return boost::math::factorial<double>(n) / temp;
	}
//public:
	//------------------------------------------------------------------------------------------------------
	// Probability Functions
	//------------------------------------------------------------------------------------------------------
	float drawSpecificCard(std::string& cardName);
	float probOppHasHikari();
};

#endif // !KOIKOI_PROBABILITY_EVAL_H
