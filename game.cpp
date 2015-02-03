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
	GameScene& currentScene = GameSceneManager::currentScene(graphics_);

	while (GameSceneManager::isRunning()) {
		timer.start();
		currentScene = GameSceneManager::currentScene(graphics_);

		controller_.stateClear();
		while (SDL_PollEvent(&event)) {
			currentScene.eventHandler(graphics_, event);
			controller_.eventHandler(event);
		}

		currentScene.update(graphics_, controller_);
		currentScene.render(graphics_);

		if (timer.getTicks() < (1000.0 / kGameFps))
			SDL_Delay((1000.0 / kGameFps) - timer.getTicks());
	}

	return 0;
}
