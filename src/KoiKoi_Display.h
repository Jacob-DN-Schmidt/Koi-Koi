#ifndef KOIKOI_DISPLAY_H
#define KOIKOI_DISPLAY_H

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/serialization/strong_typedef.hpp"
#include "raylib.h"
#include <array>
#include <boost/algorithm/string/constants.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

using namespace::std;
using namespace::boost;

BOOST_STRONG_TYPEDEF(float, FloatEnum)
constexpr float cwidth = 146.0f;
constexpr float cheight = 240.0f;

constexpr float ywidth = 79.8f;

constexpr float nwidth = 54.6f;

constexpr float swidth = 42.0f;

constexpr float ckwidth = 189.0f;

constexpr float theight = 30.0f;

constexpr float appearenceoffsetX = 4.0f;
constexpr float appearenceoffsetY = 6.0f;

//float SomeFunction(FloatingPointEnum value) {
//	float new_value;
//	/* perform some operation using "value" to calculate "new_value" */
//	new_value = static_cast<float>(value); // <- a simplistic example
//	return new_value;
//}

//54.6
//79.8
//42
//189

//enum display_elements {
//	cwidth = 146, cheight = 240,
//	ywidth = 75, theight = 30,
//	nwidth = 50, theight = 30,
//	swidth = 46,
//	ckwidth = 183, theight = 30,
//	appearenceoffset = 4
//};

class KoiKoi_Display {
private:
	Texture2D highlight_;

	void loadHighlight() {
		highlight_ = LoadTexture("Hanafuda Cards/highlight.png");
	};

	void unloadHighlight() const {
		UnloadTexture(highlight_);
	};

	Texture2D back_;

	void loadBack() {
		back_ = LoadTexture("Hanafuda Cards/backCard.png");
	};

	void unloadBack() const {
		UnloadTexture(back_);
	};

	class Hanafuda_Card_Texture {
	public:
		string imgID_;
		float x_;
		float y_;
		Texture2D texture_;
		//bool select_;

		//Texture2D highlight_;

		Hanafuda_Card_Texture(string imgID, float x, float y) : imgID_("Hanafuda Cards/" + imgID + ".png"), x_(x), y_(y), texture_()/*, select_(false)*/ {};
		~Hanafuda_Card_Texture() {
			this->unload();
		};

		void load() {
			texture_ = LoadTexture(imgID_.c_str());
		};

		void unload() const {
			UnloadTexture(texture_);
		};

		virtual void draw() const {
			/*if (select_) {
				DrawTexture(highlight_, x_ - 5, y_ - 5, WHITE);
			}*/
			DrawTexture(texture_, (int) x_, (int) y_, WHITE);
		};

		/*void select() {
			select_ = !select_;
		};*/

		int getWidth() const {
			return texture_.width;
		};

		int getHeight() const {
			return texture_.height;
		};
	};

	class Hanafuda_Card_Selectable_Texture : public virtual Hanafuda_Card_Texture {
	public:
		bool selected_;
		float hitboxWidth_;
		float hitboxHeight_;
		Texture2D& highlight_;

		Hanafuda_Card_Selectable_Texture(string imgID, float x, float y, float hbWidth, float hbHeight, Texture2D& highlight) : Hanafuda_Card_Texture(imgID, x, y), selected_(false), hitboxWidth_(hbWidth), hitboxHeight_(hbHeight), highlight_(highlight) {};

		void draw() const override {
			if (selected_) {
				DrawTexture(highlight_, (int) (x_ - 5), (int) (y_ - 5), WHITE);
			}
			DrawTexture(texture_, (int) x_, (int) y_, WHITE);
		};

		void select() {
			selected_ = !selected_;
		};

		bool checkClick(int x, int y) {
			return (x >= x_ && x < x_ + hitboxWidth_ && y >= y_ && y < y_ + hitboxHeight_);
		};

		void deselect() {
			selected_ = false;
		};
	};

	bool canSelect_;
	bool mousePress_;

	bool callKoi_;
	bool callKoiChoice_;

	int handSelection_;
	int tableSelection_;

	string gamestate_;

	//vector<Hanafuda_Card_Texture> textures_;
	vector<Hanafuda_Card_Texture> opponentPlayed_;
	vector<Hanafuda_Card_Texture> playerPlayed_;


	vector<Hanafuda_Card_Selectable_Texture> playerHandSelectable_;
	vector<Hanafuda_Card_Selectable_Texture> tableSelectable_;

	int blanksToDraw;

public:
	float screenWidth;
	float screenHeight;

	float paddingSide;
	float paddingTop;

	float handBoxWidth;
	float playBoxWidth;
	float playBoxHeight;

	float opponentHandBoxX;
	float opponentHandBoxY;

	float opponentPlayBoxX;
	float opponentPlayBoxY;

	float playerHandBoxX;
	float playerHandBoxY;

	float playerPlayBoxX;
	float playerPlayBoxY;

	float tableBoxWidth;
	float tableBoxHeight;

	float tableBoxX;
	float tableBoxY;

	int playerPts_;
	int oppPts_;

	float yesBoxX_;
	float yesBoxY_;

	float noBoxX_;
	float noBoxY_;

	bool tableSelect_;
	string cardToMatch_;
	string message_;

	KoiKoi_Display() : gamestate_(""),
		canSelect_(false),
		mousePress_(false),
		callKoi_(false),
		callKoiChoice_(false),
		handSelection_(-1),
		tableSelection_(-1),
		blanksToDraw(0),
		screenWidth(100),
		screenHeight(100),
		paddingSide(0),
		paddingTop(0),
		handBoxWidth(0),
		playBoxWidth(0),
		playBoxHeight(0),
		opponentHandBoxX(0),
		opponentHandBoxY(0),
		opponentPlayBoxX(0),
		opponentPlayBoxY(0),
		playerHandBoxX(0),
		playerHandBoxY(0),
		playerPlayBoxX(0),
		playerPlayBoxY(0),
		tableBoxWidth(0),
		tableBoxHeight(0),
		tableBoxX(0),
		tableBoxY(0),
		playerPts_(0),
		oppPts_(0),
		highlight_(),
		back_(),
		yesBoxX_(0),
		yesBoxY_(0),
		noBoxX_(0),
		noBoxY_(0),
		tableSelect_(false),
		cardToMatch_(""),
		message_("") {
	};

	~KoiKoi_Display() {
		this->closeWindow();
	}

	void initiateWindow() {
		InitWindow((int) screenWidth, (int) screenHeight, "Koi-Koi Game");
		SetTargetFPS(30);

		int monitor = GetCurrentMonitor();
		screenWidth = (float) GetMonitorWidth(monitor);
		screenHeight = (float) GetMonitorHeight(monitor);

		SetWindowSize((int) screenWidth, (int) screenHeight);

		ToggleFullscreen();

		paddingSide = screenWidth / 150;
		paddingTop = screenHeight / 100;

		handBoxWidth = ((screenWidth * 2) / 5) - (paddingSide * 4);
		playBoxWidth = ((screenWidth * 3) / 5) - (paddingSide * 2);
		playBoxHeight = cheight + (paddingTop * 2);

		opponentHandBoxX = paddingSide;
		opponentHandBoxY = paddingTop;

		opponentPlayBoxX = handBoxWidth + (paddingSide * 4);
		opponentPlayBoxY = paddingTop;

		playerHandBoxX = paddingSide;
		playerHandBoxY = screenHeight - paddingTop - cheight;

		playerPlayBoxX = opponentPlayBoxX;
		playerPlayBoxY = playerHandBoxY;

		tableBoxWidth = (screenWidth * 4) / 5;
		tableBoxHeight = (screenHeight - (playBoxHeight * 2) - (paddingTop * 6));

		tableBoxX = paddingSide;
		tableBoxY = (screenHeight / 2) - paddingTop - cheight;

		yesBoxX_ = (screenWidth / 2) - ((ywidth + swidth + nwidth) / 2) - appearenceoffsetX;
		yesBoxY_ = (screenHeight / 2) + paddingTop;

		noBoxX_ = yesBoxX_ + ywidth + swidth;
		noBoxY_ = yesBoxY_;

		loadHighlight();
		loadBack();

		cout << "width: " << screenWidth << " hight: " << screenHeight << " pbw: " << handBoxWidth << " pbh: " << playBoxHeight << " ps: " << paddingSide << " pt: " << paddingTop << "\n";
	};

	void closeWindow() {
		clearTextures();
		unloadHighlight();
		unloadBack();
		CloseWindow();
	}

	void refreshDisplay() {

		if ((canSelect_ || tableSelect_) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) mousePress_ = true;

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePress_) {
			onMouseClick(GetMouseX(), GetMouseY());
			mousePress_ = false;
		}

		BeginDrawing();
		ClearBackground(Color{ 255 , 240 , 200, 255 });

		/*DrawLine(handBoxWidth + paddingSide, 0, handBoxWidth + paddingSide, screenHeight, BLACK);
		DrawLine(handBoxWidth + (paddingSide * 4), 0, handBoxWidth + (paddingSide * 4), screenHeight, RED);*/

		if (callKoi_) {
			DrawText("Call Koi?", (int) ((screenWidth / 2) - (ckwidth / 2)), (int) ((screenHeight / 2) - (theight + paddingTop)), 42, BLACK);
			DrawText("Yes | No", (int) yesBoxX_, (int) yesBoxY_, 42, BLACK);
		}

		if (!message_.empty()) {
			DrawText(message_.c_str(), (int) (paddingSide), (int) (playBoxHeight + (theight / 2)), 42, BLACK);
		}
		// Yes w = 42
		// Yes h = 17

		// No w = 28
		// No h = 17

		//cout << "Opp played draw\n";
		for (int i = 0; i < opponentPlayed_.size(); i++) {
			opponentPlayed_[i].draw();
		}

		//cout << "Player played draw\n";
		for (int i = 0; i < playerPlayed_.size(); i++) {
			playerPlayed_[i].draw();
		}

		//cout << "Player hand draw\n";
		for (int i = 0; i < playerHandSelectable_.size(); i++) {
			if (!playerHandSelectable_[i].selected_) {
				playerHandSelectable_[i].draw();
			}
		}

		//cout << "Player hand selected draw\n";
		for (int i = 0; i < playerHandSelectable_.size(); i++) {
			if (playerHandSelectable_[i].selected_) {
				playerHandSelectable_[i].draw();
			}
		}

		//cout << "Table draw\n";
		for (int i = 0; i < tableSelectable_.size(); i++) {
			if (!tableSelectable_[i].selected_) {
				tableSelectable_[i].draw();
			}
		}

		//cout << "Table selected draw\n";
		for (int i = 0; i < tableSelectable_.size(); i++) {
			if (tableSelectable_[i].selected_) {
				tableSelectable_[i].draw();
			}
		}

		//cout << "Blanks draw\n";
		if (blanksToDraw != 0) {
			float currentDisplacement = opponentHandBoxX;
			float dx = 0;

			if (blanksToDraw - 1 != 0) {
				dx = (handBoxWidth - cwidth) / (blanksToDraw - 1);
			}

			if (dx > cwidth + (paddingSide * 2)) {
				dx = cwidth + (paddingSide * 2);
			}

			for (int i = 0; i < blanksToDraw; i++) {
				DrawTexture(back_, (int) currentDisplacement, (int) opponentHandBoxY, WHITE);
				currentDisplacement += dx;
			}
		}

		//cout << "Opp pts draw\n";
		string temp = "Opponent Points: " + to_string(oppPts_);
		DrawText(temp.c_str(), (int) (tableBoxWidth + paddingSide), (int) (tableBoxY + cheight - paddingTop), 24, BLACK);

		//cout << "Player pts draw\n";
		temp = "Player Points: " + to_string(playerPts_);
		DrawText(temp.c_str(), (int) (tableBoxWidth + paddingSide), (int) (tableBoxY + cheight + (paddingTop * 3)), 24, BLACK);

		EndDrawing();

		if (WindowShouldClose()) {
			this->closeWindow();
			exit(EXIT_SUCCESS);
		}

	};

	void onMouseClick(int x, int y) {

		if (callKoi_) {
			if (x >= yesBoxX_ && x < yesBoxX_ + ywidth && y >= yesBoxY_ + appearenceoffsetY && y < yesBoxY_ + theight + appearenceoffsetY) {
				callKoi_ = false;
				callKoiChoice_ = true;
			}
			else if (x >= noBoxX_ && x < noBoxX_ + nwidth && y >= noBoxY_ + appearenceoffsetY && y < noBoxY_ + theight + appearenceoffsetY) {
				callKoi_ = false;
				callKoiChoice_ = false;
			}
		}
		else if (tableSelect_) {
			cout << "click ";
			for (int i = 0; i < tableSelectable_.size(); i++) {
				cout << to_string(tableSelectable_.size()) << " " << to_string(i) << "\n";
				if (tableSelectable_[i].selected_ && tableSelectable_[i].checkClick(x, y)) {
					cout << "Select Success ";
					tableSelection_ = i;
					cout << tableSelection_ << "\n";
					tableSelect_ = false;
					break;
				}
			}
		}
		else {
			for (int i = 0; i < playerHandSelectable_.size(); i++) {
				if (playerHandSelectable_[i].checkClick(x, y)) {
					if (playerHandSelectable_[i].selected_) {
						deselectHand();
					}
					else {
						deselectHand();
						playerHandSelectable_[i].select();
						handSelection_ = i;
					}
				}
			}

			for (int i = 0; i < tableSelectable_.size(); i++) {
				if (tableSelectable_[i].checkClick(x, y)) {
					if (tableSelectable_[i].selected_) {
						deselectTable();
					}
					else {
						deselectTable();
						tableSelectable_[i].select();
						tableSelection_ = i;
					}
				}
			}
		}

	};

	array<int, 2> waitForSelection(const string& gamestate) {

		canSelect_ = true;
		updateGamestate(gamestate);
		while (handSelection_ == -1 || tableSelection_ == -1) {
			refreshDisplay();
		}
		array<int, 2> res = { handSelection_, tableSelection_ };
		deselectHand();
		deselectTable();
		canSelect_ = false;
		return res;


	};

	int waitForTableSelection(int tableMatch1, int tableMatch2, int handSelection = -1, string message = "") {

		tableSelect_ = true;
		message_ = message;
		selectTableAt(tableMatch1);
		selectTableAt(tableMatch2);
		if (handSelection != -1) {
			playerHandSelectable_[handSelection].selected_ = true;
		}
		while (tableSelect_) {
			refreshDisplay();
		}
		int res = tableSelection_;
		deselectHand();
		deselectTable();
		message_ = "";
		return res;

	}

	void pause(const string& gamestate) {

		updateGamestate(gamestate);
		while (!IsKeyPressed(KEY_SPACE) && !IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			refreshDisplay();
		}

	}

	bool promptCallKoi(const string& gamestate) {

		callKoi_ = true;
		canSelect_ = true;
		updateGamestate(gamestate);
		while (callKoi_) {
			refreshDisplay();
		}
		canSelect_ = false;
		return callKoiChoice_;

	};

	int promptMatch(const string& card, int firstMatch, int secondMatch) {

		tableSelect_ = true;
		if (card.front() == 'J') {
			cardToMatch_ = card.substr(1, card.size());
		}
		else {
			cardToMatch_ = card;
		}

	};

	void deselectHand() {

		for (int i = 0; i < playerHandSelectable_.size(); i++) {
			playerHandSelectable_[i].deselect();
		}
		handSelection_ = -1;

	};

	void deselectTable() {

		for (int i = 0; i < tableSelectable_.size(); i++) {
			tableSelectable_[i].deselect();
		}
		tableSelection_ = -1;

	};

	// sets card.selected_ at index in table to true
	void selectTableAt(int index) {
		tableSelectable_[index].selected_ = true;
	};

	void updateGamestate(const string& gamestate) {

		if (gamestate_.empty()) {
			gamestate_ = gamestate;
			clearTextures();
			parseAllGamestateAspects();
			loadAllGamestateAspects();
			return;
		}
		cout << "Prev: " << gamestate_ << "\n";
		cout << "Curr: " << gamestate << "\n";
		vector<string> currentPartitions;
		boost::split(currentPartitions, gamestate_, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);

		vector<string> updatedPartitions;
		boost::split(updatedPartitions, gamestate, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);

		cout << "Opp hand: " << updatedPartitions[0] << "\n";
		if (currentPartitions[0] != updatedPartitions[0]) {
			parseOpponentHand(updatedPartitions[0]);
		}

		cout << "Opp played: " << updatedPartitions[1] << "\n";
		if (currentPartitions[1] != updatedPartitions[1]) {
			clearTextures(opponentPlayed_);
			parsePlayed(updatedPartitions[1], opponentPlayBoxX, opponentPlayBoxY, opponentPlayed_);
			loadGamestateAspect(opponentPlayed_);
		}

		cout << "Player hand: " << updatedPartitions[2] << "\n";
		if (currentPartitions[2] != updatedPartitions[2]) {
			clearTextures(playerHandSelectable_);
			parsePlayerHand(updatedPartitions[2]);
			loadGamestateAspect(playerHandSelectable_);
		}

		cout << "Player played: " << updatedPartitions[3] << "\n";
		if (currentPartitions[3] != updatedPartitions[3]) {
			clearTextures(playerPlayed_);
			parsePlayed(updatedPartitions[3], playerPlayBoxX, playerPlayBoxY, playerPlayed_);
			loadGamestateAspect(playerPlayed_);
		}

		cout << "Table: " << updatedPartitions[4] << "\n";
		if (currentPartitions[4] != updatedPartitions[4]) {
			clearTextures(tableSelectable_);
			parseTable(updatedPartitions[4]);
			loadGamestateAspect(tableSelectable_);
		}
		cout << "Opp pts: " << updatedPartitions[5] << "\n";
		oppPts_ = stoi(updatedPartitions[5]);
		cout << "Player pts: " << updatedPartitions[6] << "\n";
		playerPts_ = stoi(updatedPartitions[6]);
		gamestate_ = gamestate;

	};

	void parseOpponentHand(string numCards) {
		if (!numCards.empty()) {
			blanksToDraw = stoi(numCards);
		}
	}

	void parsePlayerHand(string cards) {

		deque<string> imgNames;

		if (!cards.empty()) {
			boost::split(imgNames, cards, boost::is_any_of(","), boost::algorithm::token_compress_mode_type::token_compress_on);
			float currentDisplacement = playerHandBoxX;
			float dx = 0;

			if (imgNames.size() - 1 != 0) {
				dx = (handBoxWidth - cwidth) / (imgNames.size() - 1);
				float hbWidth = dx;

				if (dx > cwidth + (paddingSide * 2)) {
					dx = cwidth + (paddingSide * 2), hbWidth = cwidth;
				}

				for (int i = 0; i < imgNames.size() - 1; i++) {
					playerHandSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames[i], currentDisplacement, playerHandBoxY, hbWidth, cheight, highlight_));
					currentDisplacement += dx;
				}
			}
			playerHandSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames.back(), currentDisplacement, playerHandBoxY, cwidth, cheight, highlight_));
			imgNames.clear();
		}

	}

	void parseTable(string cards) {

		if (!cards.empty()) {
			deque<string> imgNames;
			boost::split(imgNames, cards, boost::is_any_of(","), boost::algorithm::token_compress_mode_type::token_compress_on);
			bool row = 0;
			float currentDisplacement = tableBoxX;
			float dx = (tableBoxWidth / imgNames.size());
			float hbWidth = dx;

			if (dx > (cwidth + (paddingSide * 2))) {
				dx = (cwidth + (paddingSide * 2)), hbWidth = cwidth;
			}
			float dy = ((paddingTop * 2) + cheight);

			for (int i = 0; i < imgNames.size(); i++) {
				tableSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames[i], currentDisplacement, (tableBoxY + (row * dy)), hbWidth, cheight, highlight_));
				if (row) currentDisplacement += dx;
				row = !row;
			}
			imgNames.clear();
		}

	}

	void parsePlayed(string& cards, float startX, float startY, vector<Hanafuda_Card_Texture>& into) {

		if (!cards.empty()) {
			deque<string> imgNames;
			boost::split(imgNames, cards, boost::is_any_of(","), boost::algorithm::token_compress_mode_type::token_compress_on);

			float currentDisplacement = startX;
			bool hadJunk = false;

			while (imgNames.size() != 0 && imgNames.front().front() == 'J') {
				into.push_back(Hanafuda_Card_Texture(imgNames.front().substr(1, imgNames.front().size()), currentDisplacement, startY));
				currentDisplacement += 15;
				imgNames.pop_front();
				hadJunk = true;
			}

			if (imgNames.size() != 0) {
				if (hadJunk) {
					currentDisplacement -= 15;
					currentDisplacement += cwidth + (paddingSide * 2);
				}

				if (imgNames.size() - 1 != 0) {
					float dx = (playBoxWidth - currentDisplacement + startX - cwidth) / (imgNames.size() - 1);

					if (dx > (cwidth + (paddingSide * 2))) {
						dx = cwidth + (paddingSide * 2);
					}

					for (int i = 0; i < imgNames.size() - 1; i++) {
						into.push_back(Hanafuda_Card_Texture(imgNames[i], currentDisplacement, startY));
						currentDisplacement += dx;
					}
				}
				into.push_back(Hanafuda_Card_Texture(imgNames.back(), currentDisplacement, startY));
				imgNames.clear();
			}
		}

	};

	void parseAllGamestateAspects() {
		/*regex match("^/ohs(\\d+)//op(.*)//ph(.*)//pp(.*)//tbl(.*)//opts(\\d+)//ppts(\\d+)/$", regex::icase);*/

		vector<string> collections;
		boost::split(collections, gamestate_, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);
		deque<string> imgNames;

		//------------------------------------------------------------------------------------------------------
		// opp hand load textures
		//------------------------------------------------------------------------------------------------------
		parseOpponentHand(collections[0]);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// opp played load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayed(collections[1], opponentPlayBoxX, opponentPlayBoxY, opponentPlayed_);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// player hand load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayerHand(collections[2]);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// player played load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayed(collections[3], playerPlayBoxX, playerPlayBoxY, playerPlayed_);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// table load textures
		//------------------------------------------------------------------------------------------------------
		parseTable(collections[4]);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// Opponent load points
		//------------------------------------------------------------------------------------------------------
		//cout << "Opp pts: " << temp << "\n";
		oppPts_ = stoi(collections[5]);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==


		//------------------------------------------------------------------------------------------------------
		// Player load points
		//------------------------------------------------------------------------------------------------------
		playerPts_ = stoi(collections[6]);
		//==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==  ==

	};

	void loadGamestateAspect(vector<Hanafuda_Card_Texture>& aspect) {

		for (int i = 0; i < aspect.size(); i++) {
			aspect[i].load();
		}

	}

	void loadGamestateAspect(vector<Hanafuda_Card_Selectable_Texture>& aspect) {

		for (int i = 0; i < aspect.size(); i++) {
			aspect[i].load();
		}

	}

	void loadAllGamestateAspects() {

		loadGamestateAspect(opponentPlayed_);
		loadGamestateAspect(playerPlayed_);
		loadGamestateAspect(playerHandSelectable_);
		loadGamestateAspect(tableSelectable_);

	}

	void clearTextures(vector<Hanafuda_Card_Texture>& target) {

		for (int i = 0; i < target.size(); i++) target[i].unload();
		target.clear();

	}

	void clearTextures(vector<Hanafuda_Card_Selectable_Texture>& target) {

		for (int i = 0; i < target.size(); i++) target[i].unload();
		target.clear();

	}

	void clearTextures() {
		for (int i = 0; i < opponentPlayed_.size(); i++) opponentPlayed_[i].unload();
		for (int i = 0; i < playerPlayed_.size(); i++) playerPlayed_[i].unload();
		for (int i = 0; i < playerHandSelectable_.size(); i++) playerHandSelectable_[i].unload();
		for (int i = 0; i < tableSelectable_.size(); i++) tableSelectable_[i].unload();
		opponentPlayed_.clear();
		playerPlayed_.clear();
		playerHandSelectable_.clear();
		tableSelectable_.clear();
	};

};
#endif // KOIKOI_DISPLAY_H