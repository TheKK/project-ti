#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window
{
public:
	Window();
	~Window();

	SDL_Window* getWindow() const;
private:
	SDL_Window* window_ = nullptr;
};

#endif /* WINDOW_H */
