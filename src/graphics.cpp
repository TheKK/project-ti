#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

#include "window.h"

#include "graphics.h"

Graphics::Graphics(const Window& window)
{
	init(window);
}

Graphics::~Graphics()
{
	IMG_Quit();
	TTF_Quit();
}

void
Graphics::init(const Window& window)
{
	if (renderer_)
		return;

	renderer_ = SDL_CreateRenderer(
		window.getWindow(),
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer_)
		throw std::runtime_error(SDL_GetError());

	if (IMG_Init(IMG_INIT_PNG) < 0)
		throw std::runtime_error(IMG_GetError());

	if (TTF_Init() < 0)
		throw std::runtime_error(TTF_GetError());
}

std::shared_ptr<SDL_Texture>
Graphics::loadSprite(const std::string& filePath)
{
	std::shared_ptr<SDL_Texture> toReturn;

	if (loadedTextures_.count(filePath) == 0) {
		std::string msg = "create: " + filePath;

		toReturn.reset(
			loadTextureFromFile_(filePath),
			[this, filePath](SDL_Texture* t)
			{
				std::string msg = "delete: " + filePath;

				SDL_DestroyTexture(t);
				this->loadedTextures_.erase(filePath);
				SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
					    msg.c_str());
			});
		loadedTextures_[filePath] = toReturn;
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, msg.c_str());
	} else {
		toReturn = loadedTextures_[filePath].lock();
	}

	return toReturn;
}

std::shared_ptr<SDL_Texture>
Graphics::loadText(TTF_Font* font, const std::string& text, SDL_Color& color)
{
	std::shared_ptr<SDL_Texture> toReturn;
	std::string msg = "create textlabel: " + text;

	toReturn.reset(
		createTextTexture_(font, text, color),
		[this, text](SDL_Texture* t)
		{
			std::string msg = "delete: " + text;

			SDL_DestroyTexture(t);
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
				    msg.c_str());
		});

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, msg.c_str());

	return toReturn;
}

void
Graphics::render(SDL_Texture* source,const SDL_Rect* clipRect,
		 const SDL_Rect* dstRect) const
{
	SDL_RenderCopy(renderer_, source, clipRect, dstRect);
}

void
Graphics::drawRect(SDL_Rect& dstRect) const
{
	SDL_SetRenderDrawColor(renderer_, 0xff, 0x00, 0x00, 0xff);
	SDL_RenderFillRect(renderer_, &dstRect);
}

void
Graphics::clear() const
{
	SDL_SetRenderDrawColor(renderer_, 0x33, 0x33, 0x33, 0xff);
	SDL_RenderClear(renderer_);
}

void
Graphics::present() const
{
	SDL_RenderPresent(renderer_);
}

void
Graphics::setLogicalSize(int w, int h) const
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
	if (!loadedImage) {
		std::string errMsg;

		errMsg = "IMG error: ";
		errMsg += IMG_GetError();

		throw std::runtime_error(errMsg);;
	}

	tex = SDL_CreateTextureFromSurface(renderer_, loadedImage);
	if (!tex) {
		std::string errMsg;

		errMsg = "SDL error: ";
		errMsg += SDL_GetError();

		throw std::runtime_error(errMsg);;
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(loadedImage);

	return tex;
}

SDL_Texture*
Graphics::createTextTexture_(TTF_Font* font, const std::string& text,
			     SDL_Color& color)
{
	SDL_Surface* tmpSurface = nullptr;
	SDL_Texture* tex = nullptr;

	tmpSurface = TTF_RenderText_Blended(font, text.c_str(), color);
	if (!tmpSurface) {
		std::string errMsg;

		errMsg = "TTF error: ";
		errMsg += TTF_GetError();

		throw std::runtime_error(errMsg);;
	}

	tex = SDL_CreateTextureFromSurface(renderer_, tmpSurface);
	if (!tex) {
		std::string errMsg;

		errMsg = "SDL error: ";
		errMsg += SDL_GetError();

		throw std::runtime_error(errMsg);;
	}

	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(tmpSurface);

	return tex;
}
