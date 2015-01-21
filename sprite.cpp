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
	load(graphics, filePath, clipRect);
}

Sprite::~Sprite()
{
}

void
Sprite::load(Graphics& graphics, const std::string& filePath,
	     const SDL_Rect& clipRect)
{
	spriteSheet_ = graphics.loadSprite(filePath);

	clipRect_.x = clipRect.x;
	clipRect_.y = clipRect.y;
	clipRect_.w = clipRect.w;
	clipRect_.h = clipRect.h;
}
void
Sprite::update()
{
}

void
Sprite::render(const Graphics& graphics, SDL_Rect& dstRect)
{
	graphics.render(spriteSheet_.get(), clipRect_, dstRect);
}
