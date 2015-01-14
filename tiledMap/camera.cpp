#include <algorithm>

#include "player.h"

#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void
Camera::setup(SDL_Renderer* renderer,
	      int worldWidth, int worldHeight, int viewWidth, int viewHeight)
{
	worldRect_ = {
		0, 0,
		worldWidth, worldHeight
	};

	viewRect_ = {
		0, 0,
		viewWidth, viewHeight
	};

	deadZoneRect_ = {
		viewRect_.w / 2 - 40 / 2, viewRect_.h / 2 - 40 / 2,
		40, 40
	};

	SDL_RenderSetLogicalSize(renderer, viewWidth, viewHeight);
}

void
Camera::update()
{
	if (!target_)
		return;

	int dx = 0, dy = 0;
	SDL_Rect targetRect_ = target_->rect();

	if (targetRect_.x < viewRect_.x + deadZoneRect_.x)
		dx = targetRect_.x - (viewRect_.x + deadZoneRect_.x);
	else if (targetRect_.x + targetRect_.w > viewRect_.x + deadZoneRect_.x + deadZoneRect_.w)
		dx = (targetRect_.x + targetRect_.w) -
			(viewRect_.x + deadZoneRect_.x + deadZoneRect_.w);

	if (targetRect_.y < viewRect_.y + deadZoneRect_.y)
		dy = targetRect_.y - (viewRect_.y + deadZoneRect_.y);
	else if (targetRect_.y + targetRect_.h > viewRect_.y + deadZoneRect_.y + deadZoneRect_.h)
		dy = (targetRect_.y + targetRect_.h) -
			(viewRect_.y + deadZoneRect_.y + deadZoneRect_.h);

	viewRect_.x += dx;
	viewRect_.y += dy;

	viewRect_.x = std::max(viewRect_.x, 0);
	viewRect_.x = std::min(viewRect_.x, worldRect_.w - viewRect_.w);
	viewRect_.y = std::max(viewRect_.y, 0);
	viewRect_.y = std::min(viewRect_.y, worldRect_.h - viewRect_.h);
}

const SDL_Rect&
Camera::getViewRect() const
{
	return viewRect_;
}

void
Camera::lookAt(const Player& target)
{
	target_ = &target;
}

