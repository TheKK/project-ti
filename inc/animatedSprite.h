#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SDL2/SDL.h>
#include <string>
#include <memory>

class Graphics;

/*
 * TODO Currently this only accept horizontal sprite,
 * need to be more flexible
 */
class AnimatedSprite
{
public:
	AnimatedSprite(Graphics& graphics, const std::string& filePath,
		       const SDL_Rect& startClipRect, int frameNum, int period);
	~AnimatedSprite();

	void load(Graphics& graphics, const std::string& filePath,
		  const SDL_Rect& startClipRect, int frameNum, int period);

	void update();
	void render(const Graphics& graphics, const SDL_Rect* dstRect);
private:
	std::shared_ptr<SDL_Texture> spriteSheet_;
	SDL_Rect startClipRect_;
	SDL_Rect drawClipRect_;

	const int totoalFrameNum_;
	int currentFrame_;

	int periodMax_;
	int periodCounter_;
};
#endif /* ANIMATED_SPRITE_H */
