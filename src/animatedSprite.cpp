#include "animatedSprite.h"

#include <SDL2/SDL_image.h>
#include <stdexcept>

#include "graphics.h"

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath,
			       const SDL_Rect& startClipRect, int frameNum,
			       int period):
	spriteSheet_(graphics.loadSprite(filePath)),
	startClipRect_(startClipRect),
	drawClipRect_(startClipRect),
	totoalFrameNum_(frameNum),
	currentFrame_(0),
	periodMax_(period),
	periodCounter_(0)
{
}

AnimatedSprite::~AnimatedSprite()
{
}

void
AnimatedSprite::load(Graphics& graphics, const std::string& filePath,
		     const SDL_Rect& startClipRect, int frameNum, int period)
{
	spriteSheet_ = graphics.loadSprite(filePath);

	startClipRect_ = startClipRect;
	drawClipRect_ = startClipRect;

	periodMax_ = period;
	periodCounter_ = 0;
}
void
AnimatedSprite::update()
{
	++periodCounter_;
	if (periodCounter_ == periodMax_) {
		periodCounter_ = 0;

		++currentFrame_;
		if (currentFrame_ == totoalFrameNum_)
			currentFrame_ = 0;

		drawClipRect_.x = (startClipRect_.x +
				   startClipRect_.w * currentFrame_);
	}
}

void
AnimatedSprite::render(const Graphics& graphics, const SDL_Rect* dstRect)
{
	graphics.render(spriteSheet_.get(), &drawClipRect_, dstRect);
}
