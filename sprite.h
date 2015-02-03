#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <string>
#include <memory>

class Graphics;

class Sprite
{
public:
	Sprite(Graphics& graphics, const std::string& filePath,
	       const SDL_Rect& clipRect);

	void load(Graphics& graphics, const std::string& filePath,
		  const SDL_Rect& clipRect);

	void update();
	void render(const Graphics& graphics, SDL_Rect& dstRect);

	void setAlpha(Graphics& graphics, uint8_t value);
private:
	std::shared_ptr<SDL_Texture> spriteSheet_;
	SDL_Rect clipRect_;
};
#endif /* SPRITE_H */
