#ifndef GAME_H
#define GAME_H

#include <vector>

#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"

class Game
{
public:
	Game();
	~Game();

	int execute();
private:
	bool appIsRunning_ = true;

	SDLSystem sdlSystem_;
	Window window_;
	Graphics graphics_;
};

#endif /* GAME_H */
