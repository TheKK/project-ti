#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include <SDL2/SDL.h>
#include <memory>
#include <string>

class Graphics;

class TextLabel
{
public:
	TextLabel(Graphics& graphics, const std::string& text,
		  const std::string& fontPath, int fontSize, SDL_Color color);

	void load(Graphics& graphics, const std::string& text,
		  const std::string& fontPath, int fontSize, SDL_Color color);

	void update();
	void render(const Graphics& graphics, const SDL_Rect* dstRect);
private:
	std::shared_ptr<SDL_Texture> spriteSheet_;
};

#endif /* TEXT_LABEL_H */
