#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window
{
public:
	Window(const std::string& windowTitle, int w, int h);
	~Window();

	SDL_Window* getWindow() const;
private:
	SDL_Window* window_ = nullptr;
};

#endif /* WINDOW_H */
