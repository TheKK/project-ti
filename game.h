#ifndef GAME_H
#define GAME_H

#include <vector>

#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"
#include "controller.h"
#include "gameSceneManager.h"

class Game
{
public:
	Game();
	~Game();

	int execute(enum TotalGameScene firstScene);
private:
	SDLSystem sdlSystem_;
	Window window_;
	Graphics graphics_;
	Controller controller_;
};

#endif /* GAME_H */
