#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>

class Window;

class Graphics
{
public:
	~Graphics();

	void init(const Window& window);
	std::shared_ptr<SDL_Texture> loadSprite(const std::string& filePath);

	void render(const std::shared_ptr<SDL_Texture>& source, SDL_Rect& clip,
		    SDL_Rect& dest) const;

	void drawRect(SDL_Rect& dest) const;

	void clear() const;
	void present() const;

	void setLogicalSize(int w, int h) const;
private:
	SDL_Renderer* renderer_ = nullptr;

	std::map<std::string, std::weak_ptr<SDL_Texture>> loadedTextures_;

	SDL_Texture* loadTextureFromFile_(const std::string& filePath);
};

#endif /*  GRAPHICS_H */
