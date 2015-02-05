#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <SDL2/SDL.h>

class Graphics;
class Controller;

class GameScene
{
public:
	virtual ~GameScene() {};

	virtual void eventHandler(const SDL_Event& event) = 0;
	virtual void update(const Controller& controller) = 0;
	virtual void render() = 0;
};

#endif /* GAME_SCENE_H */
