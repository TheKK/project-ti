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

	const float kGravityAcc = 1.f;

	const int16_t kJoyAxisMax = 32767;
}

Player::Player():
	posX_(10), posY_(10),
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
	updateInput_(controller);

	updateX_(tileLayer);
	updateY_(tileLayer);
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
	posRect.x = std::round(posX_);
	posRect.y = std::round(posY_);

	return posRect;
}

void
Player::testCollideWithTiles_(const MapTileLayer& tileLayer)
{

}

void
Player::updateInput_(const Controller& controller)
{
	if (controller.getButtonState(BUTTON_LEFT) == BUTTON_STATE_PRESSED)
		accX_ = -0.6f;
	else if (controller.getButtonState(BUTTON_RIGHT) == BUTTON_STATE_PRESSED)
		accX_ = 0.6f;
	else
		accX_ = velX_ * -0.3;;

	if (controller.getButtonState(BUTTON_UP) == BUTTON_STATE_PRESSED)
		accY_ = -0.6f;
	else if (controller.getButtonState(BUTTON_DOWN) == BUTTON_STATE_PRESSED)
		accY_ = 0.6f;
	else
		accY_ = velY_ * -0.3;;

	if (std::abs(controller.getAxisValue(0)) > 4000)
		accX_ = 0.6f * ((float) controller.getAxisValue(0) / (float) kJoyAxisMax);

	if (std::abs(controller.getAxisValue(1)) > 4000)
		accY_ = 0.6f * ((float) controller.getAxisValue(1) / (float) kJoyAxisMax);
}

void
Player::updateX_(const MapTileLayer& tileLayer)
{
	std::vector<SDL_Rect> tiles;
	SDL_Rect colliedRect;

	velX_ += accX_;
	velX_ = std::min(velX_, kMoveMaxSpeedX);
	velX_ = std::max(velX_, -1 * kMoveMaxSpeedX);

	if (velX_ > 0.f) {
		/* Test right part first */
		colliedRect.x = std::round(posX_ + kPlayerWidth / 2 + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velX_ = 0.f;
				accX_ = 0.f;
				posX_ = tile.x - kPlayerWidth;
			}
		}

		/* Test left then */
		colliedRect.x = std::round(posX_ + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velX_ = 0.0f;
				accX_ = 0.0f;
				posX_ = tile.x + tile.w;
			}
		}
	} else if (velX_ < 0) {
		/* Test left part first */
		colliedRect.x = std::round(posX_ + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velX_ = 0.0f;
				accX_ = 0.0f;
				posX_ = tile.x + tile.w;
			}
		}

		/* Test right part then */
		colliedRect.x = std::round(posX_ + kPlayerWidth / 2 + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velX_ = 0.f;
				accX_ = 0.f;
				posX_ = tile.x - kPlayerWidth;
			}
		}
	}

	posX_ += velX_;
}

void
Player::updateY_(const MapTileLayer& tileLayer)
{
	std::vector<SDL_Rect> tiles;
	SDL_Rect colliedRect;

	velY_ += accY_;
	velY_ = std::min(velY_, kMoveMaxSpeedY);
	velY_ = std::max(velY_, -1 * kMoveMaxSpeedY);

	if (velY_ > 0.f) {
		/* Test botton part first */
		colliedRect.x = std::round(posX_);
		colliedRect.y = std::round(posY_ + kPlayerHeight / 2 + velY_);
		colliedRect.w = std::round(kPlayerWidth);
		colliedRect.h = std::round(kPlayerHeight / 2);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velY_ = 0.f;
				accY_ = 0.f;
				posY_ = tile.y - kPlayerHeight;
			}
		}

		/* Test top part then */
		colliedRect.x = std::round(posX_);
		colliedRect.y = std::round(posY_ + velY_);
		colliedRect.w = std::round(kPlayerWidth);
		colliedRect.h = std::round(kPlayerHeight / 2);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velY_ = 0.f;
				accY_ = 0.f;
				posY_ = tile.y + tile.h;
			}
		}
	} else if (velY_ < 0.f) {
		/* Test top part first */
		colliedRect.x = std::round(posX_);
		colliedRect.y = std::round(posY_ + velY_);
		colliedRect.w = std::round(kPlayerWidth);
		colliedRect.h = std::round(kPlayerHeight / 2);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velY_ = 0.f;
				accY_ = 0.f;
				posY_ = tile.y + tile.h;
			}
		}

		/* Test botton part then */
		colliedRect.x = std::round(posX_);
		colliedRect.y = std::round(posY_ + kPlayerHeight / 2 + velY_);
		colliedRect.w = std::round(kPlayerWidth);
		colliedRect.h = std::round(kPlayerHeight / 2);

		tiles = tileLayer.getCollidedTiles(colliedRect);

		for (const SDL_Rect& tile : tiles) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velY_ = 0.f;
				accY_ = 0.f;
				posY_ = tile.y - kPlayerHeight;
			}
		}
	}

	posY_ += velY_;
}
