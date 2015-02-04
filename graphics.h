#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Window;

class Graphics
{
public:
	Graphics(const Window& window);
	~Graphics();

	void init(const Window& window);
	void render(SDL_Texture* source,
		    const SDL_Rect* clipRect, const SDL_Rect* dstRect) const;

	void drawRect(SDL_Rect& dstRect) const;

	void clear() const;
	void present() const;

	std::shared_ptr<SDL_Texture> loadSprite(const std::string& filePath);
	std::shared_ptr<SDL_Texture> loadText(TTF_Font* font,
					      const std::string& text,
					      SDL_Color& color);

	void setLogicalSize(int w, int h) const;
private:
	SDL_Renderer* renderer_ = nullptr;

	std::map<std::string, std::weak_ptr<SDL_Texture>> loadedTextures_;

	SDL_Texture* loadTextureFromFile_(const std::string& filePath);
	SDL_Texture* createTextTexture_(TTF_Font* font, const std::string& text,
					SDL_Color& color);
};

#endif /*  GRAPHICS_H */
