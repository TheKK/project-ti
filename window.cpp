#include <stdexcept>

#include "window.h"

Window::Window()
{
	window_ = SDL_CreateWindow(
		"test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		600, 600,
		SDL_WINDOW_SHOWN);
	if (!window_)
		throw std::runtime_error(SDL_GetError());
}

Window::~Window()
{
	SDL_DestroyWindow(window_);
	window_ = nullptr;
}

SDL_Window*
Window::getWindow() const
{
	return window_;
}
