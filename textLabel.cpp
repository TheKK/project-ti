#include "textLabel.h"

#include <SDL2/SDL_ttf.h>
#include "graphics.h"

TextLabel::TextLabel(Graphics& graphics, const std::string& text,
		     const std::string& fontPath, int fontSize, SDL_Color color)
{
	load(graphics, text, fontPath, fontSize, color);
}

void
TextLabel::load(Graphics& graphics, const std::string& text,
		const std::string& fontPath, int fontSize, SDL_Color color)
{
	TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (!font) {
		std::string errMsg;

		errMsg = "TTF error: ";
		errMsg += TTF_GetError();

		throw std::runtime_error(errMsg);
	}

	spriteSheet_ = graphics.loadText(font, text, color);

	TTF_CloseFont(font);
}

void
TextLabel::update()
{
}

void
TextLabel::render(const Graphics& graphics, const SDL_Rect* dstRect)
{
	graphics.render(spriteSheet_.get(), nullptr, dstRect);
}
