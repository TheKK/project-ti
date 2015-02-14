#include <SDL2/SDL.h>

#include "timer.h"
#include "gameScene.h"

#include "game.h"

namespace
{
	const double kGameFps = 60.0;

	const std::string kGameTitle = "test";
	const int kGameWindowWidth = 600;
	const int kGameWindowHeight = 600;
}

Game::Game():
	window_(kGameTitle, kGameWindowWidth, kGameWindowHeight),
	graphics_(window_)
{
}

Game::~Game()
{
	GameSceneManager::quit();
}

int
Game::execute(enum TotalGameScene firstScene)
{
	GameSceneManager::pushScene(graphics_, firstScene);

	SDL_Event event;
	Timer timer;
	GameScene* currentScene;

	while ((currentScene = GameSceneManager::currentScene(graphics_))) {
		timer.start();

		controller_.stateClear();
		while (SDL_PollEvent(&event)) {
			currentScene->eventHandler(event);
			controller_.eventHandler(event);
		}

		currentScene->update(controller_);
		currentScene->render();

		if (timer.getTicks() < (1000.0 / kGameFps))
			SDL_Delay((1000.0 / kGameFps) - timer.getTicks());
	}

	return 0;
}
