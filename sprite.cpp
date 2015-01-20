#include "sprite.h"

#include <SDL2/SDL_image.h>
#include <stdexcept>

#include "graphics.h"

Sprite::Sprite()
{
}

Sprite::Sprite(Graphics& graphics, const std::string& filePath,
	       const SDL_Rect& clipRect)
{
	init(graphics, filePath, clipRect);
}

Sprite::~Sprite()
{
}

void
Sprite::init(Graphics& graphics, const std::string& filePath,
	     const SDL_Rect& clipRect)
{
	spriteSheet_ = graphics.loadSprite(filePath);
	SDL_assert(spriteSheet_.get() != nullptr);

	clipRect_.x = clipRect.x;
	clipRect_.y = clipRect.y;
	clipRect_.w = clipRect.w;
	clipRect_.h = clipRect.h;
}
void
Sprite::update()
{
	SDL_assert(spriteSheet_.get() != nullptr);
}

void
Sprite::render(const Graphics& graphics, SDL_Rect& dstRect)
{
	SDL_assert(spriteSheet_.get() != nullptr);
	graphics.render(spriteSheet_.get(), clipRect_, dstRect);
}
