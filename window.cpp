#include <stdexcept>

#include "window.h"

Window::Window(const std::string& windowTitle, int w, int h)
{
	window_ = SDL_CreateWindow(
		windowTitle.c_str(),	
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h,
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
