#include <SDL2/SDL.h>
#include <stdexcept>

#include "sdlSystem.h"

SDLSystem::SDLSystem()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error(SDL_GetError());
}

SDLSystem::~SDLSystem()
{
	SDL_Quit();
}
