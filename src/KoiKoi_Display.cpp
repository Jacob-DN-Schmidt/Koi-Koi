#include "KoiKoi_Display.h"
#include <raylib.h>

Texture2D highlight_;
Texture2D back_;

void loadHighlight() {
	highlight_ = LoadTexture("Hanafuda Cards/highlight.png");
};

void unloadHighlight() {
	UnloadTexture(highlight_);
};

void loadBack() {
	back_ = LoadTexture("Hanafuda Cards/backCard.png");
};

void unloadBack() {
	UnloadTexture(back_);
};
