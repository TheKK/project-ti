#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Sprite
{
public:
	Sprite();
	Sprite(SDL_Renderer* renderer, const std::string& filePath,
	       int width, int height, int frameDuration);
	~Sprite();

	void init(SDL_Renderer* renderer, const std::string& filePath,
		  int width, int height, int frameDuration);

	void update();
	void render(SDL_Renderer* renderer, const SDL_Rect* pos);
private:
	SDL_Texture* spriteSheet_ = nullptr;
	std::vector<SDL_Rect> clipRects_;

	int currentFrame_ = 0;
	int frameDuration_ = 0;
	int framwDurationCounter_ = 0;

	SDL_Texture* loadTexture_(SDL_Renderer* renderer,
				  const std::string& filePath);
};
#endif /* SPRITE_H */
