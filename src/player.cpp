#include <iostream>
#include <algorithm>

#include "controller.h"
#include "camera.h"
#include "map.h"
#include "graphics.h"

#include "player.h"

namespace
{
	const int kPlayerWidth = 10;
	const int kPlayerHeight = 10;

	const std::string kSpriteName = "./assets/images/standSprite.png";

	const float kMoveMaxSpeedX = 2.5f;

	const float kGravityAcc = 0.11f;
}

Player::Player(Graphics& graphics):
	standSprite_(graphics, kSpriteName, {0, 0, 10, 10}),
	posX_(10), posY_(10),
	velX_(0), velY_(0),
	accX_(0), accY_(0)
{
	currentState_ = &standingState_;
	nextState_ = nullptr;
}

void
Player::update(const Controller& controller, const MapTileLayer& tileLayer)
{
	accX_ = accY_ = 0.f;

	currentState_->update(*this, controller);
	if (nextState_) {
		currentState_->onExit(*this);
		nextState_->onEnter(*this);
		currentState_ = nextState_;
		nextState_ = nullptr;
	}

	standSprite_.update();

	updateX_(tileLayer);
	updateY_(tileLayer);
	checkWallAround_(tileLayer);
}

void
Player::render(const Graphics& graphics, const Camera& camera)
{
	SDL_Rect posToDraw;

	posToDraw.w = kPlayerWidth;
	posToDraw.h = kPlayerHeight;
	posToDraw.x = round(posX_) - camera.getLeft();
	posToDraw.y = round(posY_) - camera.getTop();

	standSprite_.render(graphics, &posToDraw);
}

void
Player::setX(int x)
{
	posX_ = x;
}

void
Player::setY(int y)
{
	posY_ = y;
}

void
Player::setAccX(float accX)
{
	accX_ = accX;
}

void
Player::setAccY(float accY)
{
	accY_ = accY;
}

void
Player::setVelX(float velX)
{
	velX_ = velX;
}

void
Player::setVelY(float velY)
{
	velY_ = velY;
}

void
Player::setCheckpoint(int x, int y)
{
	checkPointX_ = x;
	checkPointY_ = y;
}

int
Player::getVelX() const
{
	return velX_;
}

int
Player::getVelY() const
{
	return velY_;
}

void
Player::testCollide(const std::vector<SDL_Rect>& rects)
{
	SDL_Rect colliedRect;

	if (velX_ > 0.f) {
		/* Test right part first */
		colliedRect.x = std::round(posX_ + kPlayerWidth / 2 + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		for (const SDL_Rect& rect : rects) {
			if (SDL_HasIntersection(&rect, &colliedRect)) {
				velX_ = 0.f;
				accX_ = 0.f;
				posX_ = rect.x - kPlayerWidth;
			}
		}

		/* Test left then */
		colliedRect.x = std::round(posX_ + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		for (const SDL_Rect& rect : rects) {
			if (SDL_HasIntersection(&rect, &colliedRect)) {
				velX_ = 0.0f;
				accX_ = 0.0f;
				posX_ = rect.x + rect.w;
			}
		}
	} else if (velX_ < 0) {
		/* Test left part first */
		colliedRect.x = std::round(posX_ + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		for (const SDL_Rect& rect : rects) {
			if (SDL_HasIntersection(&rect, &colliedRect)) {
				velX_ = 0.0f;
				accX_ = 0.0f;
				posX_ = rect.x + rect.w;
			}
		}

		/* Test right part then */
		colliedRect.x = std::round(posX_ + kPlayerWidth / 2 + velX_);
		colliedRect.y = std::round(posY_);
		colliedRect.w = std::round(kPlayerWidth / 2);
		colliedRect.h = std::round(kPlayerHeight);

		for (const SDL_Rect& rect : rects) {
			if (SDL_HasIntersection(&rect, &colliedRect)) {
				velX_ = 0.f;
				accX_ = 0.f;
				posX_ = rect.x - kPlayerWidth;
			}
		}
	}

	if (velY_ > 0.f) {
		/* Test botton part first */
		colliedRect.x = std::round(posX_);
		colliedRect.y = std::round(posY_ + kPlayerHeight / 2 + velY_);
		colliedRect.w = std::round(kPlayerWidth);
		colliedRect.h = std::round(kPlayerHeight / 2);

		for (const SDL_Rect& tile : rects) {
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

		for (const SDL_Rect& tile : rects) {
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

		for (const SDL_Rect& tile : rects) {
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

		for (const SDL_Rect& tile : rects) {
			if (SDL_HasIntersection(&tile, &colliedRect)) {
				velY_ = 0.f;
				accY_ = 0.f;
				posY_ = tile.y - kPlayerHeight;
			}
		}
	}
}

void
Player::setNextState(enum PlayerState nextState)
{
	switch (nextState) {
	case STATE_STANDING:
		nextState_ = &standingState_;
		break;
	case STATE_JUMPING:
		nextState_ = &jumpingState_;
		break;
	case STATE_FALLING:
		nextState_ = &fallingState_;
		break;
	default:
		nextState_ = nullptr;
		break;
	};
}

void
Player::suicide()
{
	posX_ = checkPointX_;
	posY_ = checkPointY_;

	velX_ = 0;
	velY_ = 0;
	accX_ = 0;
	accY_ = 0;
}

bool
Player::isOnGround() const
{
	return isOnGround_;
}

bool
Player::isBesideRightWall() const
{
	return isBesideRightWall_;
}

bool
Player::isBesideLeftWall() const
{
	return isBesideLeftWall_;
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
Player::updateX_(const MapTileLayer& tileLayer)
{
	std::vector<SDL_Rect> tiles;
	SDL_Rect colliedRect;

	if (isOnGround_)
		accX_ += velX_ * -0.2;
	else
		accX_ += velX_ * -0.05;

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

	accY_ += kGravityAcc;

	velY_ += accY_;
	velY_ = std::min(velY_, 2 * kMoveMaxSpeedX);
	velY_ = std::max(velY_, -2 * kMoveMaxSpeedX);

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

void
Player::checkWallAround_(const MapTileLayer& tileLayer)
{
	std::vector<SDL_Rect> tiles;
	SDL_Rect colliedRect;

	/* Check if standing on floor */
	colliedRect.x = std::round(posX_);
	colliedRect.y = std::round(posY_ + kPlayerHeight / 2) + 1;
	colliedRect.w = std::round(kPlayerWidth);
	colliedRect.h = std::round(kPlayerHeight / 2);

	tiles = tileLayer.getCollidedTiles(colliedRect);
	if (tiles.size() > 0)
		isOnGround_ = true;
	else
		isOnGround_ = false;

	/* Check if left side is wall */
	colliedRect.x = std::round(posX_) - 1;
	colliedRect.y = std::round(posY_);
	colliedRect.w = std::round(kPlayerWidth / 2);
	colliedRect.h = std::round(kPlayerHeight);

	tiles = tileLayer.getCollidedTiles(colliedRect);
	if (tiles.size() > 0)
		isBesideLeftWall_ = true;
	else
		isBesideLeftWall_ = false;

	/* Check if right side is wall */
	colliedRect.x = std::round(posX_ + kPlayerWidth / 2) + 1;
	colliedRect.y = std::round(posY_);
	colliedRect.w = std::round(kPlayerWidth / 2);
	colliedRect.h = std::round(kPlayerHeight);

	tiles = tileLayer.getCollidedTiles(colliedRect);
	if (tiles.size() > 0)
		isBesideRightWall_ = true;
	else
		isBesideRightWall_ = false;
}
