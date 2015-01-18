#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>

class Graphics
{
public:
	void init(SDL_Window* window);
	std::shared_ptr<SDL_Texture> loadSprite(const std::string& filePath);

	void render(const std::shared_ptr<SDL_Texture>& source, SDL_Rect& clip,
		    SDL_Rect& dest);

	void drawRect(SDL_Rect& dest);

	void clear();
	void present();

	void setLogicalSize(int w, int h);
private:
	SDL_Renderer* renderer_ = nullptr;

	std::map<std::string, SDL_Texture*> loadedTextures_;

	SDL_Texture* loadTextureFromFile_(const std::string& filePath);
};

#endif /*  GRAPHICS_H */
