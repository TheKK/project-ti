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
	clipRect_ = clipRect;
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
