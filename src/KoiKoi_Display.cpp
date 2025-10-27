#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/constants.hpp"
#include "boost/algorithm/string/split.hpp"
#include "KoiKoi_Display.h"
#include <raylib.h>
#include <array>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace::KoiKoi_Display_Enums;


//------------------------------------------------------------------------------------------------------
// Card Texture Preloading
//------------------------------------------------------------------------------------------------------
Texture2D& KoiKoi_Display::getPreloadedCardTexture(std::string& imageID) {
	for (std::size_t i = 0; i < preloadedCardTextures.size(); i++) {
		if (preloadedCardTextures[i].imageID_ == imageID) {
			return preloadedCardTextures[i].texture_;
		}
	}

}


//------------------------------------------------------------------------------------------------------
// Card Highlight/Back Textures
//------------------------------------------------------------------------------------------------------
Texture2D KoiKoi_Display::highlightTexture_;
Texture2D KoiKoi_Display::back_;
Texture2D KoiKoi_Display::texture_error_;

//------------------------------------------------------------------------------------------------------
// Card Highlight/Back Operations
//------------------------------------------------------------------------------------------------------
void KoiKoi_Display::loadHighlight() {
	highlightTexture_ = LoadTexture("Hanafuda_Card_Data/highlight.png");
};

void KoiKoi_Display::unloadHighlight() {
	UnloadTexture(highlightTexture_);
};

void KoiKoi_Display::loadBack() {
	back_ = LoadTexture("Hanafuda_Card_Data/backCard.png");
};

void KoiKoi_Display::unloadBack() {
	UnloadTexture(back_);
}


//------------------------------------------------------------------------------------------------------
// Window Operations
//------------------------------------------------------------------------------------------------------
void KoiKoi_Display::initiateWindow() {
	InitWindow((int) screenWidth, (int) screenHeight, "Koi-Koi Game");
	SetTargetFPS(30);

	int monitor = GetCurrentMonitor();

	// Setting dynamic screen varible values
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

	tableBoxX = cwidth + 2 * paddingSide;
	tableBoxY = (screenHeight / 2) - paddingTop - cheight;

	yesBoxX_ = (screenWidth / 2) - ((ywidth + swidth + nwidth) / 2) - appearenceoffsetX;
	yesBoxY_ = (screenHeight / 2) + paddingTop;

	noBoxX_ = yesBoxX_ + ywidth + swidth;
	noBoxY_ = yesBoxY_;

	// Preloading textures
	loadHighlight();
	loadBack();
	texture_error_ = LoadTexture("missing_texture.png");

	std::regex imagePattern(".*\\.png$", std::regex::ECMAScript);

	FilePathList list = LoadDirectoryFiles("Hanafuda_Card_Data");
	for (unsigned int i = 0; i < list.count; i++) {
		if (std::regex_match(GetFileName(list.paths[i]), imagePattern)) {
			std::cout << "\nTexture " << i << ": ";
			preloadedCardTextures.push_back(IDTexture(GetFileNameWithoutExt(list.paths[i])));
		}
	}

#ifdef CONSOLE_DEBUG
	std::cout << "width: " << screenWidth << " hight: " << screenHeight << " pbw: " << handBoxWidth << " pbh: " << playBoxHeight << " ps: " << paddingSide << " pt: " << paddingTop << "\n";
#endif // CONSOLE_DEBUG
}


//------------------------------------------------------------------------------------------------------
// Drawing Operations
//------------------------------------------------------------------------------------------------------
void KoiKoi_Display::refreshDisplay() {
	// Mouse input handling
	if (canSelect_ && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) mousePress_ = true;

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mousePress_) {
		onMouseClick(GetMouseX(), GetMouseY());
		mousePress_ = false;
	}

	// Drawing
	BeginDrawing();
	ClearBackground(Color{ 255 , 240 , 200, 255 });

	// Prompt to call Koi
	if (callKoi_) {
		DrawTextEx(courier_new, "Call Koi?", Vector2((screenWidth / 2) - (ckwidth / 2), (screenHeight / 2) - (theight + paddingTop)), 42, 4, BLACK);
		DrawTextEx(courier_new, "Yes | No", Vector2( yesBoxX_, yesBoxY_), 42, 4, BLACK);
	}

	// Draw message
	if (!message_.empty()) {
		DrawText(message_.c_str(), (int) (paddingSide), (int) (playBoxHeight + (theight / 2)), 42, BLACK);
	}

#ifdef CONSOL_DEBUG
	std::cout << "Opp played draw\n";
#endif

	// Draw opponent played cards
	for (int i = 0; i < opponentPlayed_.size(); i++) {
		opponentPlayed_[i].draw();
	}

#ifdef CONSOL_DEBUG
	std::cout << "Player played draw\n";
#endif

	// Draw player played cards
	for (int i = 0; i < playerPlayed_.size(); i++) {
		playerPlayed_[i].draw();
	}

#ifdef CONSOL_DEBUG
	std::cout << "Player hand draw\n";
#endif

	// Draw player hand cards that are not selected
	for (int i = 0; i < playerHandSelectable_.size(); i++) {
		if (!playerHandSelectable_[i].selected_) {
			playerHandSelectable_[i].draw();
		}
	}

#ifdef CONSOL_DEBUG
	std::cout << "Player hand selected draw\n";
#endif

	// Draw player hand cards that are selected
	for (int i = 0; i < playerHandSelectable_.size(); i++) {
		if (playerHandSelectable_[i].selected_) {
			playerHandSelectable_[i].draw();
		}
	}

#ifdef CONSOL_DEBUG
	std::cout << "Table draw\n";
#endif

	// Draw table cards that are not selected
	for (int i = 0; i < tableSelectable_.size(); i++) {
		if (!tableSelectable_[i].selected_) {
			tableSelectable_[i].draw();
		}
	}

#ifdef CONSOL_DEBUG
	std::cout << "Table selected draw\n";
#endif

	// Draw table cards that are selected
	for (int i = 0; i < tableSelectable_.size(); i++) {
		if (tableSelectable_[i].selected_) {
			tableSelectable_[i].draw();
		}
	}

#ifdef CONSOL_DEBUG
	std::cout << "Blanks draw\n";
#endif

	// Drawing opponent hand cards backside
	if (blanksToDraw != 0) {
		float currentDisplacement = opponentHandBoxX;
		float dx = (handBoxWidth - cwidth) / (blanksToDraw - 1);

		if (dx > cwidth + (paddingSide * 2)) {
			dx = cwidth + (paddingSide * 2);
		}

		for (int i = 0; i < blanksToDraw; i++) {
			DrawTexture(back_, (int) currentDisplacement, (int) opponentHandBoxY, WHITE);
			currentDisplacement += dx;
		}
	}

#ifdef CONSOL_DEBUG
	std::cout << "Opp pts draw\n";
#endif

	// Drawing opponent points scored
	std::string temp = "Opponent Points: " + std::to_string(oppPts_);
	DrawTextEx(courier_new, temp.c_str(), Vector2(tableBoxWidth + paddingSide, tableBoxY + cheight - paddingTop), 24, 4, BLACK);

#ifdef CONSOLE_DEBUG
	std::cout << "Player pts draw\n";
#endif

	// Drawing player points scored
	temp = "Player Points: " + std::to_string(playerPts_);
	DrawTextEx(courier_new, temp.c_str(), Vector2(tableBoxWidth + paddingSide, tableBoxY + cheight + (paddingTop * 3)), 24, 4, BLACK);

	EndDrawing();

	if (WindowShouldClose()) {
		for (std::size_t i = 0; i < preloadedCardTextures.size(); i++) {
			// Clear card textures from VRAM
			UnloadTexture(preloadedCardTextures[i].texture_);
		}
		preloadedCardTextures.clear();
		this->closeWindow();
		exit(EXIT_SUCCESS);
	}
}

void KoiKoi_Display::onMouseClick(int x, int y) {

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
	else if (tableSelectMatch_) {
		for (int i = 0; i < tableSelectable_.size(); i++) {
#ifdef CONSOLE_DEBUG
			std::cout << std::to_string(tableSelectable_.size()) << " " << std::to_string(i) << "\n";
#endif // CONSOLE_DEBUG
			if (tableSelectable_[i].selected_ && tableSelectable_[i].checkClick(x, y)) {
				tableSelection_ = i;
#ifdef CONSOLE_DEBUG
				std::cout << "Select Success " << tableSelection_ << "\n";
#endif // CONSOLE_DEBUG
				tableSelectMatch_ = false;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < playerHandSelectable_.size(); i++) {
			if (playerHandSelectable_[i].checkClick(x, y)) {

				if (playerHandSelectable_[i].select()) {
					handSelection_ = i;
				}
				else {
					handSelection_ = -1;
				}
			}
			else {
				playerHandSelectable_[i].deselect();
			}
		}

		for (int i = 0; i < tableSelectable_.size(); i++) {
			if (tableSelectable_[i].checkClick(x, y)) {
				if (tableSelectable_[i].select()) {
					tableSelection_ = i;
				}
				else {
					tableSelection_ = -1;
				}
			}
			else {
				tableSelectable_[i].deselect();
			}
		}
	}

}

std::array<int, 2> KoiKoi_Display::waitForSelection(const std::string& gamestate) {
	canSelect_ = true;
	updateGamestate(gamestate);

	while (handSelection_ == -1 || tableSelection_ == -1) {
		refreshDisplay();
	}

	std::array<int, 2> res = { handSelection_, tableSelection_ };
	deselectHand();
	deselectTable();
	canSelect_ = false;

	return res;
}

int KoiKoi_Display::waitForTableSelection(int tableMatch1, int tableMatch2, int handSelection, std::string message) {
	canSelect_ = true;
	tableSelectMatch_ = true;
	message_ = message;
	selectTableAt(tableMatch1);
	selectTableAt(tableMatch2);

	if (handSelection != -1) {
		playerHandSelectable_[handSelection].selected_ = true;
	}

	while (tableSelectMatch_) {
		refreshDisplay();
	}

	int res = tableSelection_;
	deselectHand();
	deselectTable();
	message_ = "";

	return res;
}

void KoiKoi_Display::pause(const std::string& gamestate) {
	updateGamestate(gamestate);
	while (!IsKeyPressed(KEY_SPACE) && !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) refreshDisplay();
	
}

bool KoiKoi_Display::promptCallKoi(const std::string& gamestate) {
	callKoi_ = true;
	canSelect_ = true;
	updateGamestate(gamestate);

	while (callKoi_) {
		refreshDisplay();
	}
	canSelect_ = false;
	return callKoiChoice_;
}

int KoiKoi_Display::promptMatch(const std::string& card, int firstMatch, int secondMatch) {
	canSelect_ = true;
	tableSelectMatch_ = true;

	if (card.front() == 'J') {
		cardToMatch_ = card.substr(1, card.size());
	}
	else {
		cardToMatch_ = card;
	}

	// TODO
	return -1;
}

void KoiKoi_Display::updateGamestate(const std::string& gamestate) {

	if (gamestate_.empty()) {
		gamestate_ = gamestate;
		clearAllTextures();
		parseAllGamestateAspects();
		return;
	}

	std::vector<std::string> currentPartitions;
	boost::split(currentPartitions, gamestate_, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);

	std::vector<std::string> updatedPartitions;
	boost::split(updatedPartitions, gamestate, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);


	//------------------------------------------------------------------------------------------------------
	// opp hand load textures
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Opp hand: " << updatedPartitions[0] << "\n";
#endif // CONSOLE_DEBUG
	if (currentPartitions[0] != updatedPartitions[0]) {
		parseOpponentHand(updatedPartitions[0]);
	}


	//------------------------------------------------------------------------------------------------------
	// opp played load textures
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Opp played: " << updatedPartitions[1] << "\n";
#endif // CONSOLE_DEBUG
	if (currentPartitions[1] != updatedPartitions[1]) {
		opponentPlayed_.clear();
		parsePlayed(updatedPartitions[1], opponentPlayBoxX, opponentPlayBoxY, opponentPlayed_);
	}


	//------------------------------------------------------------------------------------------------------
	// player hand load textures
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Player hand: " << updatedPartitions[2] << "\n";
#endif // CONSOLE_DEBUG
	if (currentPartitions[2] != updatedPartitions[2]) {
		playerHandSelectable_.clear();
		parsePlayerHand(updatedPartitions[2]);
	}


	//------------------------------------------------------------------------------------------------------
	// player played load textures
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Player played: " << updatedPartitions[3] << "\n";
#endif // CONSOLE_DEBUG
	if (currentPartitions[3] != updatedPartitions[3]) {
		playerPlayed_.clear();
		parsePlayed(updatedPartitions[3], playerPlayBoxX, playerPlayBoxY, playerPlayed_);
	}


	//------------------------------------------------------------------------------------------------------
	// table load textures
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Table: " << updatedPartitions[4] << "\n";
#endif // CONSOLE_DEBUG
	if (currentPartitions[4] != updatedPartitions[4]) {
		tableSelectable_.clear();
		parseTable(updatedPartitions[4]);
	}


	//------------------------------------------------------------------------------------------------------
	// Opponent load points
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Opp pts: " << updatedPartitions[5] << "\n";
#endif // CONSOLE_DEBUG
	oppPts_ = stoi(updatedPartitions[5]);


	//------------------------------------------------------------------------------------------------------
	// Player load points
	//------------------------------------------------------------------------------------------------------
#ifdef CONSOLE_DEBUG
	std::cout << "Player pts: " << updatedPartitions[6] << "\n";
#endif // CONSOLE_DEBUG
	playerPts_ = stoi(updatedPartitions[6]);


	//------------------------------------------------------------------------------------------------------
	// Update gamestate
	//------------------------------------------------------------------------------------------------------
	gamestate_ = gamestate;
}

void KoiKoi_Display::parsePlayerHand(std::string cards) {
	std::vector<std::string> imgNames;

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
				playerHandSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames[i], currentDisplacement, playerHandBoxY, getPreloadedCardTexture(imgNames[i]), hbWidth, cheight));
				currentDisplacement += dx;
			}
		}
		playerHandSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames.back(), currentDisplacement, playerHandBoxY, getPreloadedCardTexture(imgNames.back()), cwidth, cheight));
		imgNames.clear();
	}

}

void KoiKoi_Display::parseTable(std::string cards) {

	if (!cards.empty()) {
		std::vector<std::string> imgNames;
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
			tableSelectable_.push_back(Hanafuda_Card_Selectable_Texture(imgNames[i], currentDisplacement, (tableBoxY + (row * dy)), getPreloadedCardTexture(imgNames[i]), hbWidth, cheight));
			if (row) currentDisplacement += dx;
			row = !row;
		}
		imgNames.clear();
	}

}

void KoiKoi_Display::parsePlayed(std::string& cards, float startX, float startY, std::vector<Hanafuda_Card_Texture>& into) {

	if (!cards.empty()) {
		std::deque<std::string> imgNames;
		boost::split(imgNames, cards, boost::is_any_of(","), boost::algorithm::token_compress_mode_type::token_compress_on);

		float currentDisplacement = startX;
		bool hadJunk = false;

		while (imgNames.size() != 0 && imgNames.front().front() == 'J') {
			std::string temp = imgNames.front().substr(1, imgNames.front().size());
			into.push_back(Hanafuda_Card_Texture(temp, currentDisplacement, startY, getPreloadedCardTexture(temp)));
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
					into.push_back(Hanafuda_Card_Texture(imgNames[i], currentDisplacement, startY, getPreloadedCardTexture(imgNames[i])));
					currentDisplacement += dx;
				}
			}
			into.push_back(Hanafuda_Card_Texture(imgNames.back(), currentDisplacement, startY, getPreloadedCardTexture(imgNames.back())));
			imgNames.clear();
		}
	}

}
