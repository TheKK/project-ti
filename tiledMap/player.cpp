#include "controller.h"
#include "camera.h"

#include "player.h"

namespace
{
	int kPlayerWidth = 10;
	int kPlayerHeight = 10;

	int kMoveSpeed = 1;
}

Player::Player()
{
	posRect_.x = 0;
	posRect_.y = 0;
	posRect_.w = kPlayerWidth;
	posRect_.h = kPlayerHeight;
}

Player::~Player()
{
}

void
Player::update(const Controller& controller, const MapTileLayer& tiles)
{
	if (controller.getButtonState(BUTTON_UP) == BUTTON_STATE_PRESSED)
		posRect_.y -= kMoveSpeed;
	if (controller.getButtonState(BUTTON_DOWN) == BUTTON_STATE_PRESSED)
		posRect_.y += kMoveSpeed;
	if (controller.getButtonState(BUTTON_LEFT) == BUTTON_STATE_PRESSED)
		posRect_.x -= kMoveSpeed;
	if (controller.getButtonState(BUTTON_RIGHT) == BUTTON_STATE_PRESSED)
		posRect_.x += kMoveSpeed;
}

void 
Player::render(SDL_Renderer* renderer, const Camera& camera)
{
	SDL_Rect cameraRect = camera.getViewRect();
	SDL_Rect posToDraw = posRect_;

	posToDraw.x = posRect_.x - cameraRect.x;
	posToDraw.y = posRect_.y - cameraRect.y;

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer, &posToDraw);
}

void 
Player::setPos(int x, int y)
{
	posRect_.x = x;
	posRect_.y = y;
}

SDL_Rect
Player::rect() const
{
	return posRect_;
}
