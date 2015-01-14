#include <algorithm>

#include "controller.h"
#include "camera.h"
#include "map.h"

#include "player.h"

namespace
{
	const int kPlayerWidth = 10;
	const int kPlayerHeight = 10;

	const float kMoveMaxSpeedX = 4.f;
	const float kMoveMaxSpeedY = 4.f;

	//const float kGravityAcc = 1.f;
}

Player::Player():
	posX_(0), posY_(0),
	velX_(0), velY_(0),
	accX_(0), accY_(0)
{
}

Player::~Player()
{
}

void
Player::update(const Controller& controller, const MapTileLayer& tileLayer)
{
	/* Handle controller */
	if (controller.getButtonState(BUTTON_UP) == BUTTON_STATE_PRESSED)
		accY_ = -0.6f;
	else if (controller.getButtonState(BUTTON_DOWN) ==BUTTON_STATE_PRESSED)
		accY_ = 0.6f;
	else
		accY_ = velY_ * -0.3;

	if (controller.getButtonState(BUTTON_LEFT) == BUTTON_STATE_PRESSED)
		accX_ = -0.6f;
	else if (controller.getButtonState(BUTTON_RIGHT) == BUTTON_STATE_PRESSED)
		accX_ = 0.6f;
	else
		accX_ = velX_ * -0.3;;

	/* Handle position */
	posX_ += velX_;
	posY_ += velY_;

	velX_ += accX_;
	velX_ = std::min(velX_, kMoveMaxSpeedX);
	velX_ = std::max(velX_, -1 * kMoveMaxSpeedX);

	velY_ += accY_;
	velY_ = std::min(velY_, kMoveMaxSpeedY);
	velY_ = std::max(velY_, -1 * kMoveMaxSpeedY);

	testCollideWithTiles_(tileLayer);
}

void 
Player::render(SDL_Renderer* renderer, const Camera& camera)
{
	SDL_Rect cameraRect = camera.getViewRect();
	SDL_Rect posToDraw;

	posToDraw.w = kPlayerWidth;
	posToDraw.h = kPlayerHeight;
	posToDraw.x = round(posX_) - cameraRect.x;
	posToDraw.y = round(posY_) - cameraRect.y;

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer, &posToDraw);
}

SDL_Rect
Player::posRectOnMap() const
{
	SDL_Rect posRect;

	posRect.w = kPlayerWidth;
	posRect.h = kPlayerHeight;
	posRect.x = round(posX_);
	posRect.y = round(posY_);

	return posRect;
}

void
Player::testCollideWithTiles_(const MapTileLayer& tileLayer)
{
	std::vector<SDL_Rect> tiles;

	tiles = tileLayer.getCollidedTiles(posRectOnMap());
}
