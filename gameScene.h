#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <SDL2/SDL.h>

class Graphics;

class GameScene
{
public:
	virtual ~GameScene() {};

	/* TODO Fix "Graphics" hell */
	virtual void eventHandler(Graphics& graphics,
				  const SDL_Event& event) = 0;
	virtual void update(Graphics& graphics) = 0;
	virtual void render(Graphics& graphics) = 0;
};

#endif /* GAME_SCENE_H */
