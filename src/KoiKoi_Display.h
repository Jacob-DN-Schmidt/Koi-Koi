#ifndef KOIKOI_DISPLAY_H
#define KOIKOI_DISPLAY_H

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/constants.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/serialization/strong_typedef.hpp"
#include <raylib.h>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <iostream>


//------------------------------------------------------------------------------------------------------
// Float Enums
//------------------------------------------------------------------------------------------------------
namespace KoiKoi_Display_Enums {
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
}

class KoiKoi_Display {
private:
	//------------------------------------------------------------------------------------------------------
	// Card Texture Preloading
	//------------------------------------------------------------------------------------------------------
	struct IDTexture {
	public:
		std::string imageID_;
		Texture2D texture_;

		IDTexture(std::string imageID) : imageID_(imageID), texture_() { texture_ = LoadTexture(("Hanafuda_Card_Data/" + imageID_ + ".png").c_str()); };
	};

	std::vector<IDTexture> preloadedCardTextures;

	Texture2D& getPreloadedCardTexture(std::string& imageID);

	//------------------------------------------------------------------------------------------------------
	// Card Highlight/Back Textures
	//------------------------------------------------------------------------------------------------------

	static Texture2D highlightTexture_;
	static Texture2D back_;
	static Texture2D texture_error_;


	//------------------------------------------------------------------------------------------------------
	// Card Highlight/Back Operations
	//------------------------------------------------------------------------------------------------------

	// Load highlight texture
	static void loadHighlight();
	// Unload highlight texture
	static void unloadHighlight();

	// Load card back texture
	static void loadBack();
	// Unload card back texture
	static void unloadBack();


	//------------------------------------------------------------------------------------------------------
	// Nested Card Texture Containers
	//------------------------------------------------------------------------------------------------------

	// Nested base class
	class Hanafuda_Card_Texture {
	public:

		std::string imgID_;
		float x_;
		float y_;
		Texture2D& texture_;

		// Constructor: imgID(texture filename), x(left coord), y(top coord)
		Hanafuda_Card_Texture(std::string imgID, float x, float y, Texture2D& texture) : imgID_("Hanafuda Cards/" + imgID + ".png"), x_(x), y_(y), texture_(texture) {};
		// Draws card texture; should only be called while in drawing mode loop
		virtual void draw() const {
			DrawTexture(texture_, (int)x_, (int)y_, WHITE);
		};
	};

	// Nested child class
	class Hanafuda_Card_Selectable_Texture : public virtual Hanafuda_Card_Texture {
	public:

		bool selected_;
		float hitboxWidth_;
		float hitboxHeight_;

		// Constructor: imgID(texture filename), x(left coord), y(top coord), hbWidth(width of hitbox/texture shown), hbHeight(height of hitbox/texture shown)
		Hanafuda_Card_Selectable_Texture(std::string imgID, float x, float y, Texture2D& texture, float hbWidth, float hbHeight) : Hanafuda_Card_Texture(imgID, x, y, texture), selected_(false), hitboxWidth_(hbWidth), hitboxHeight_(hbHeight) {};

		void draw() const override {
			if (selected_) {
				DrawTexture(highlightTexture_, (int)(x_ - 5), (int)(y_ - 5), WHITE);
			}
			DrawTexture(texture_, (int)x_, (int)y_, WHITE);
		};

		bool select() {
			selected_ = !selected_;
			return selected_;
		};

		bool checkClick(int x, int y) {
			return (x >= x_ && x < x_ + hitboxWidth_ && y >= y_ && y < y_ + hitboxHeight_);
		};

		void deselect() {
			selected_ = false;
		};
	};


	//------------------------------------------------------------------------------------------------------
	// Interaction Control Flags
	//------------------------------------------------------------------------------------------------------

	bool canSelect_ = false;
	bool callKoi_ = false;
	bool tableSelectMatch_ = false;


	//------------------------------------------------------------------------------------------------------
	// Selections
	//------------------------------------------------------------------------------------------------------

	bool mousePress_ = false;
	bool callKoiChoice_ = false;
	int handSelection_ = -1;
	int tableSelection_ = -1;


	//------------------------------------------------------------------------------------------------------
	// Display Aspects
	//------------------------------------------------------------------------------------------------------
	Font courier_new = LoadFontEx("cour.ttf", 12, NULL, 0);

	std::string cardToMatch_ = "";
	std::string message_ = "";

	std::string gamestate_ = "";

	int blanksToDraw = 0;

	std::vector<Hanafuda_Card_Texture> opponentPlayed_;
	std::vector<Hanafuda_Card_Texture> playerPlayed_;

	std::vector<Hanafuda_Card_Selectable_Texture> playerHandSelectable_;
	std::vector<Hanafuda_Card_Selectable_Texture> tableSelectable_;

	int playerPts_ = 0;
	int oppPts_ = 0;

	// End private ========================================================================================== //

public:

	//------------------------------------------------------------------------------------------------------
	// Window/Aspect dimensions and drawing/bouding coordinates
	//------------------------------------------------------------------------------------------------------

	float screenWidth = 100;
	float screenHeight = 100;

	float paddingSide = 0;
	float paddingTop = 0;

	float handBoxWidth = 0;
	float playBoxWidth = 0;
	float playBoxHeight = 0;

	float tableBoxWidth = 0;
	float tableBoxHeight = 0;

	float opponentHandBoxX = 0;
	float opponentHandBoxY = 0;

	float opponentPlayBoxX = 0;
	float opponentPlayBoxY = 0;

	float playerHandBoxX = 0;
	float playerHandBoxY = 0;

	float playerPlayBoxX = 0;
	float playerPlayBoxY = 0;

	float tableBoxX = 0;
	float tableBoxY = 0;

	float yesBoxX_ = 0;
	float yesBoxY_ = 0;

	float noBoxX_ = 0;
	float noBoxY_ = 0;


	//------------------------------------------------------------------------------------------------------
	// Constuctors/Destructors
	//------------------------------------------------------------------------------------------------------
	KoiKoi_Display() {};
	~KoiKoi_Display() {
		this->closeWindow();
	};


	//------------------------------------------------------------------------------------------------------
	// Window Operations
	//------------------------------------------------------------------------------------------------------

	// Sets dimensions and coordinates to fit in fullscreen mode
	void initiateWindow();
	// Call when Display is meant to close; clears all textures out of vram
	void closeWindow() {
		clearAllTextures();
		unloadHighlight();
		unloadBack();
		CloseWindow();
	}


	//------------------------------------------------------------------------------------------------------
	// Drawing Operations
	//------------------------------------------------------------------------------------------------------

	// Draws game aspects that have been parsed and loaded
	// Mouse interaction flag - canSelect_
	void refreshDisplay();


	//------------------------------------------------------------------------------------------------------
	// Interaction Operations
	//------------------------------------------------------------------------------------------------------

	// Handles mouse inputs
	// Operation flags - callKoi_, tableSelect_
	void onMouseClick(int x, int y);
	// Waits until one card in both the player's hand and on the table is selected and returns an array with the idex of the hand card followed by the index of the table card
	std::array<int, 2> waitForSelection(const std::string& gamestate);
	// Waits until index of selected card on table matches one of match indexes and returns index 
	int waitForTableSelection(int tableMatch1, int tableMatch2, int handSelection = -1, std::string message = "");
	// Waits until left mouse click or spacebar press
	void pause(const std::string& gamestate);
	// Waits until player clicks yes or no
	bool promptCallKoi(const std::string& gamestate);
	// Waits until selected card on table matches the specified card
	int promptMatch(const std::string& card, int firstMatch, int secondMatch);


	//------------------------------------------------------------------------------------------------------
	// Texture Operations
	//------------------------------------------------------------------------------------------------------
	void updateGamestate(const std::string& gamestate);

	void parseOpponentHand(std::string numCards) {
		if (!numCards.empty()) {
			blanksToDraw = stoi(numCards);
		}
	}
	void parsePlayerHand(std::string cards);
	void parseTable(std::string cards);
	void parsePlayed(std::string& cards, float startX, float startY, std::vector<Hanafuda_Card_Texture>& into);
	void parseAllGamestateAspects() {

		std::vector<std::string> collections;
		boost::split(collections, gamestate_, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);

		//------------------------------------------------------------------------------------------------------
		// opp hand load textures
		//------------------------------------------------------------------------------------------------------
		parseOpponentHand(collections[0]);

		//------------------------------------------------------------------------------------------------------
		// opp played load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayed(collections[1], opponentPlayBoxX, opponentPlayBoxY, opponentPlayed_);

		//------------------------------------------------------------------------------------------------------
		// player hand load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayerHand(collections[2]);

		//------------------------------------------------------------------------------------------------------
		// player played load textures
		//------------------------------------------------------------------------------------------------------
		parsePlayed(collections[3], playerPlayBoxX, playerPlayBoxY, playerPlayed_);

		//------------------------------------------------------------------------------------------------------
		// table load textures
		//------------------------------------------------------------------------------------------------------
		parseTable(collections[4]);

		//------------------------------------------------------------------------------------------------------
		// Opponent load points
		//------------------------------------------------------------------------------------------------------
		//cout << "Opp pts: " << temp << "\n";
		oppPts_ = stoi(collections[5]);

		//------------------------------------------------------------------------------------------------------
		// Player load points
		//------------------------------------------------------------------------------------------------------
		playerPts_ = stoi(collections[6]);
	};

	// Sets card at index in table selected attribute to true
	void selectTableAt(int index) {
		tableSelectable_[index].selected_ = true;
	};
	// Deselects all cards in hand and sets hand selection index to -1
	void deselectHand() {

		for (int i = 0; i < playerHandSelectable_.size(); i++) {
			playerHandSelectable_[i].deselect();
		}
		handSelection_ = -1;

	};
	// Deselects all cards in table and sets table selection to -1
	void deselectTable() {

		for (int i = 0; i < tableSelectable_.size(); i++) {
			tableSelectable_[i].deselect();
		}
		tableSelection_ = -1;

	};

	// Clears all card textures in aspect
	void clearAllTextures() {
		opponentPlayed_.clear();
		playerPlayed_.clear();
		playerHandSelectable_.clear();
		tableSelectable_.clear();
	};

};
#endif // KOIKOI_DISPLAY_H