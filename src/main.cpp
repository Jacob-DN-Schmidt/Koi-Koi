//#define CONSOLE_DEBUG
//#define CONSOLE_PLAY

#include "KoiKoi_Game.h"
#include "KoiKoi_Probability_Eval.h"
#include <climits>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
	std::cout << boost::math::factorial<double>(4) << "\n";
	std::cout << KoiKoi_Probablility_Eval::nChoosek(10,4);
	return 0;

	unsigned int seed = UINT_MAX & time(nullptr);
	std::ofstream seedFile;
	seedFile.open("bin/seed_log.txt", std::ios::app);
	if (seedFile.is_open()) {
		seedFile << seed << "\n";
		seedFile.close();
	}
	srand(seed);
	KoiKoi_Game game;
	game.startGame();

	return 0;
}

//
//#include "KoiKoi_Game.h"
//#include <cstdlib>
//#include <ctime>
//#include <exception>
//#include <iostream>
//
//int main() {
//	try {
//		srand((unsigned int) time(nullptr));
//		KoiKoi_Game game;
//		game.startGame();
//		//Player player;
//		//Hanafuda_Deck deck;
//		//cout << deck.deckImage();
//		/*for (int i = 0; i < 8; i++)
//			player.drawCard(deck);
//		cout << "Player before: " << player << "\n";
//		delete player.playCard(4);
//		cout << "Player after: " << player;*/
//		//deque<Hanafuda_Card*> test;
//		//KoiKoi_Game_Handler::setGameRule("000011");
//
//		//for (int i = 0; i < 4; i++) {
//		//	for (int j = 0; j < 2; j++){
//		//		test.push_back(new Hanafuda_Card(i, j));
//		//		cout << test.back() << " ";
//		//	}
//		//}
//		//test.push_back(new Hanafuda_Card(10, 0));
//		//cout << test.back() << "\n";
//		//cout << KoiKoi_Game_Handler::teshiKuttsuki(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 4; i++) {
//		//	test.push_back(new Hanafuda_Card(0, i));
//		//	cout << test.back() << " ";
//		//}
//		//cout << "\n" << KoiKoi_Game_Handler::teshiKuttsuki(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::setGameRule("000010");
//
//		//for (int i = 0; i < 4; i++) {
//		//	for (int j = 0; j < 2; j++) {
//		//		test.push_back(new Hanafuda_Card(i, j));
//		//		cout << test.back() << " ";
//		//	}
//		//}
//		//cout << "\n" << KoiKoi_Game_Handler::teshiKuttsuki(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 4; i++) {
//		//	test.push_back(new Hanafuda_Card(0, i));
//		//	cout << test.back() << " ";
//		//}
//		//cout << "\n" << KoiKoi_Game_Handler::teshiKuttsuki(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::printGameRule();
//		//cout << "\n";
//
//		//for (int i = 0; i < 10; i++)
//		//	test.push_back(new Hanafuda_Card(0, 0));
//		//cout << "Junk test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 9; i++)
//		//	test.push_back(new Hanafuda_Card(0, 0));
//		//test.push_back(new Hanafuda_Card(8, 4, "Sake Cup"));
//		//cout << "Junk w Sake test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 9; i++)
//		//	test.push_back(new Hanafuda_Card(0, 0));
//		//test.push_back(new Hanafuda_Card(8, 4, "Sake Cup"));
//		//cout << "Junk w Sake test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::CAN_COUNT_SAKE_AS_JUNK = false;
//
//		//for (int i = 0; i < 9; i++)
//		//	test.push_back(new Hanafuda_Card(0, 0));
//		//test.push_back(new Hanafuda_Card(8, 4, "Sake Cup"));
//		//cout << "Junk w Sake and cannot as junk test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//cout << "\n";
//
//		//for (int i = 0; i < 5; i++)
//		//	test.push_back(new Hanafuda_Card(0, 1));
//		//cout << "Ribbon test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 2));
//		//cout << "Poetry test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 3));
//		//cout << "Blue test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 2));
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 3));
//		//cout << "Poetry/Blue test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::setGameRule("001011");
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 2));
//		//test.push_back(new Hanafuda_Card(0, 1));
//		//cout << "Poetry as 5 test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 3));
//		//test.push_back(new Hanafuda_Card(0, 1));
//		//cout << "Blue as 5 test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 2));
//		//cout << "Poetry as 5 test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, 3));
//		//cout << "Blue as 5 test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//cout << "\n";
//
//		//for (int i = 0; i < 5; i++)
//		//	test.push_back(new Hanafuda_Card(0, 4));
//		//cout << "Tane test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Boar;6;4"));
//		//test.push_back(new Hanafuda_Card("Deer;9;4"));
//		//test.push_back(new Hanafuda_Card("Butterfly;5;4"));
//		//cout << "Boar Deer Butterfly test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//cout << "\n";
//		//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Full Moon;7;5"));
//		//cout << "Moon Viewing test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Curtain;2;5"));
//		//cout << "Curtain Viewing test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::CAN_COUNT_SAKE_AS_JUNK = true;
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Full Moon;7;5"));
//		//cout << "Moon Viewing w Sake as Junk test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Curtain;2;5"));
//		//cout << "Curtain Viewing w Sake as Junk test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//KoiKoi_Game_Handler::SPOIL_VIEWING = true;
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Full Moon;7;5"));
//		//test.push_back(new Hanafuda_Card("Ono no Michikaze;10;5"));
//		//cout << "Moon Viewing w Spoil and Rain man: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Curtain;2;5"));
//		//test.push_back(new Hanafuda_Card("Ono no Michikaze;10;5"));
//		//cout << "Curtain Viewing w Spoil and Rain man: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Full Moon;7;5"));
//		//test.push_back(new Hanafuda_Card("Lighting;10;0"));
//		//cout << "Moon Viewing w Spoil and Lighting: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//
//		//test.push_back(new Hanafuda_Card("Sake Cup;8;4"));
//		//test.push_back(new Hanafuda_Card("Curtain;2;5"));
//		//test.push_back(new Hanafuda_Card("Lighting;10;0"));
//		//cout << "Curtain Viewing w Spoil and Lighting: " << KoiKoi_Game_Handler::eval(test) << "\n";
//		//KoiKoi_Game_Handler::deleteDequeContent(test);
//		//
//		//for (int i = 0; i < 3; i++)
//		//	test.push_back(new Hanafuda_Card(0, Hikari));
//		//cout << "Three Lights test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//
//		//test.push_back(new Hanafuda_Card("Ono no Michikaze;10;5"));
//		//cout << "Rainy Four Lights test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//
//		//delete test.back();
//		//test.pop_back();
//
//		//test.push_back(new Hanafuda_Card(0, Hikari));
//		//cout << "Four Lights test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//
//		//test.push_back(new Hanafuda_Card("Ono no Michikaze;10;5"));
//		//cout << "Rainy Five Lights test: " << KoiKoi_Game_Handler::eval(test) << "\n";
//
//		//int val = tabulation[hasSakeCup] * !COUNT_SAKE_AS_JUNK * ~(SPOIL_VIEWING * ~(~tabulation[hasOnoNoMichikaze] * ~tabulation[hasLighting]));
//
//
//		return 0;
//	}
//	catch (const exception& e) {
//		cout << e.what();
//		return 1;
//	}
//}