#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>

#include "graphics.h"

void
Graphics::init(SDL_Window* window)
{
	if (renderer_)
		return;

	renderer_ = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer_)
		throw std::runtime_error(SDL_GetError());
}

std::shared_ptr<SDL_Texture>
Graphics::loadSprite(const std::string& filePath)
{
	std::shared_ptr<SDL_Texture> toReturn;

	if (loadedTextures_.count(filePath) == 0) {
		toReturn.reset(
			loadTextureFromFile_(filePath),
			[this, filePath](SDL_Texture* t)
			{
				SDL_DestroyTexture(t);
				this->loadedTextures_.erase(filePath);
				std::cout << "delete: " << filePath << "\n";
			});
		loadedTextures_[filePath] = toReturn;
		std::cout << "create: " << filePath << "\n";
	} else {
		toReturn = loadedTextures_[filePath].lock();
	}

	return toReturn;
}

void
Graphics::render(const std::shared_ptr<SDL_Texture>& source, SDL_Rect& clip,
		 SDL_Rect& dest)
{
	SDL_RenderCopy(renderer_, source.get(), &clip, &dest);
}

void
Graphics::drawRect(SDL_Rect& dest)
{
	SDL_SetRenderDrawColor(renderer_, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer_, &dest);
}

void
Graphics::clear()
{
	SDL_SetRenderDrawColor(renderer_, 0x33, 0x33, 0x33, 0xff);
	SDL_RenderClear(renderer_);
}

void
Graphics::present()
{
	SDL_RenderPresent(renderer_);
}

void
Graphics::setLogicalSize(int w, int h)
{
	SDL_RenderSetLogicalSize(renderer_, w, h);
}

SDL_Texture*
Graphics::loadTextureFromFile_(const std::string& filePath)
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

	tex = SDL_CreateTextureFromSurface(renderer_, loadedImage);
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
