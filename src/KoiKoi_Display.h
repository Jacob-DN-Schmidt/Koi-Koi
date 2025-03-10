#ifndef KOIKOI_DISPLAY_H
#define KOIKOI_DISPLAY_H

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/constants.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/serialization/strong_typedef.hpp"
#include "raylib.h"
#include <array>
#include <deque>
#include <string>
#include <vector>


using namespace::boost;

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
	// Card Highlight/Back Textures
	//------------------------------------------------------------------------------------------------------

	static Texture2D highlightTexture;
	static Texture2D back_;


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
		Texture2D texture_;

		// Constructor: imgID(texture filename), x(left coord), y(top coord)
		Hanafuda_Card_Texture(std::string imgID, float x, float y) : imgID_("Hanafuda Cards/" + imgID + ".png"), x_(x), y_(y), texture_() {};
		~Hanafuda_Card_Texture() {
			this->unload();
		};
		// Loads card texture into vram; should only be called after display initiation
		void load() {
			texture_ = LoadTexture(imgID_.c_str());
		};
		// Unloads card texture 
		void unload() const {
			UnloadTexture(texture_);
		};
		// Draws card texture; should only be called while in drawing mode loop
		virtual void draw() const {
			DrawTexture(texture_, (int) x_, (int) y_, WHITE);
		};

		// Unused Methods
		//int getWidth() const {
		//	return texture_.width;
		//};

		//int getHeight() const {
		//	return texture_.height;
		//};
	};

	// Nested child class
	class Hanafuda_Card_Selectable_Texture : public virtual Hanafuda_Card_Texture {
	public:

		bool selected_;
		float hitboxWidth_;
		float hitboxHeight_;

		// Constructor: imgID(texture filename), x(left coord), y(top coord), hbWidth(width of hitbox/texture shown), hbHeight(height of hitbox/texture shown)
		Hanafuda_Card_Selectable_Texture(std::string imgID, float x, float y, float hbWidth, float hbHeight) : Hanafuda_Card_Texture(imgID, x, y), selected_(false), hitboxWidth_(hbWidth), hitboxHeight_(hbHeight) {};

		void draw() const override {
			if (selected_) {
				DrawTexture(highlightTexture, (int) (x_ - 5), (int) (y_ - 5), WHITE);
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


	//------------------------------------------------------------------------------------------------------
	// Interaction Control Flags
	//------------------------------------------------------------------------------------------------------

	bool canSelect_ = false;
	bool callKoi_ = false;
	bool tableSelect_ = false;


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
		clearTextures();
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
	void parsePlayed(std::string& cards, float startX, float startY, std::vector<Hanafuda_Card_Texture>& into) const;
	void parseAllGamestateAspects() {

		std::vector<std::string> collections;
		boost::split(collections, gamestate_, boost::is_any_of("/"), boost::algorithm::token_compress_mode_type::token_compress_off);

		std::deque<std::string> imgNames;

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

	void loadGamestateAspect(std::vector<Hanafuda_Card_Texture>& aspect) {
		for (int i = 0; i < aspect.size(); i++) {
			aspect[i].load();
		}
	}
	void loadGamestateAspect(std::vector<Hanafuda_Card_Selectable_Texture>& aspect) {
		for (int i = 0; i < aspect.size(); i++) {
			aspect[i].load();
		}
	}
	void loadGamestateAspect() {
		loadGamestateAspect(opponentPlayed_);
		loadGamestateAspect(playerPlayed_);
		loadGamestateAspect(playerHandSelectable_);
		loadGamestateAspect(tableSelectable_);
	}

	void selectTableAt(int index) {
		tableSelectable_[index].selected_ = true;
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

	void clearTextures(std::vector<Hanafuda_Card_Texture>& target) {
		for (int i = 0; i < target.size(); i++) target[i].unload();
		target.clear();
	}
	void clearTextures(std::vector<Hanafuda_Card_Selectable_Texture>& target) {
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