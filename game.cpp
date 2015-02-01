#include <SDL2/SDL.h>

#include "timer.h"
#include "gameScene.h"
#include "gameSceneManager.h"

#include "game.h"

Game::Game():
	graphics_(window_)
{
	GameSceneManager::pushScene(graphics_, MAIN_GAME_SCENE);
}

Game::~Game()
{
	GameSceneManager::quit();
}

int
Game::execute()
{
	SDL_Event event;
	Timer timer;
	GameScene& currentScene = GameSceneManager::currentScene(graphics_);

	while (GameSceneManager::isRunning()) {
		timer.start();
		currentScene = GameSceneManager::currentScene(graphics_);

		while (SDL_PollEvent(&event))
			currentScene.eventHandler(graphics_, event);

		currentScene.update(graphics_);

		currentScene.render(graphics_);

		if (timer.getTicks() < (1000.0 / (double) 60))
			SDL_Delay((1000.0 / (double) 60) - timer.getTicks());
	}

	return 0;
}

