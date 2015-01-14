#include "sprite.h"

#include <SDL2/SDL_image.h>
#include <stdexcept>

Sprite::Sprite()
{
}

Sprite::Sprite(SDL_Renderer* renderer, const std::string& filePath,
	       int width, int height, int frameDuration)
{
	init(renderer, filePath, width, height, frameDuration);
}

Sprite::~Sprite()
{
	if (spriteSheet_)
		SDL_DestroyTexture(spriteSheet_);
	spriteSheet_ = nullptr;
}

void
Sprite::init(SDL_Renderer* renderer, const std::string& filePath,
	     int width, int height, int frameDuration)
{
	int sheetWidth, sheetHeight;
	SDL_Rect clipRect;

	spriteSheet_ = loadTexture_(renderer, "./light.png");
	if (!spriteSheet_)
		throw std::runtime_error("Loadnig error");

	SDL_SetTextureBlendMode(spriteSheet_, SDL_BLENDMODE_ADD);

	SDL_QueryTexture(spriteSheet_, nullptr, nullptr,
			 &sheetWidth, &sheetHeight);

	for (int i = 0; i * width < sheetWidth; ++i) {
		clipRect = {
			i * width, 0,
			width, height
		};

		clipRects_.push_back(clipRect);
	}

	frameDuration_ = frameDuration;
}
void
Sprite::update()
{
	++framwDurationCounter_;
	if (framwDurationCounter_ == frameDuration_) {
		++currentFrame_;
		if (currentFrame_ == clipRects_.size())
			currentFrame_ = 0;

		framwDurationCounter_ = 0;
	}
}

void
Sprite::render(SDL_Renderer* renderer, const SDL_Rect* pos)
{
	SDL_RenderCopy(renderer, spriteSheet_, &clipRects_[currentFrame_], pos);
}

SDL_Texture*
Sprite::loadTexture_(SDL_Renderer* renderer, const std::string& filePath)
{
	SDL_Surface* loadedImage = nullptr;
	SDL_Texture* tex = nullptr;
	char* basePath = nullptr;
	std::string fullPath;

	basePath = SDL_GetBasePath();
	fullPath += basePath;
	fullPath += filePath;

	SDL_free(basePath);
	basePath = nullptr;

	loadedImage = IMG_Load(fullPath.c_str());
	if (loadedImage == nullptr) {
		std::string errMsg("IMG error while opening: ");
		errMsg += IMG_GetError();
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, errMsg.c_str());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, loadedImage);
	if (tex == nullptr) {
		std::string errMsg("SDL error while converting surface: ");
		errMsg += SDL_GetError();
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, errMsg.c_str());
		return nullptr;
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(loadedImage);

	return tex;
}
