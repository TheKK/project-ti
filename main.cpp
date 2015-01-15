#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>

#include "timer.h"
#include "map.h"
#include "sprite.h"
#include "controller.h"
#include "camera.h"
#include "player.h"

namespace
{
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* mainRenderer = nullptr;
	bool appIsRunning = true;

	MapTileLayer backLayer;
	MapTileLayer foreLayer;
	MapObjectLayer objectLayer;

	Camera camera;

	Controller controller;

	Player player;
}

void
reloadMap()
{
	MapLoader mapLoader;

	mapLoader.load("map.json");

	backLayer.cleanUp();
	backLayer.load(mainRenderer, mapLoader, "layer_one");

	foreLayer.cleanUp();
	foreLayer.load(mainRenderer, mapLoader, "layer_two");

	objectLayer.cleanUp();
	objectLayer.load(mapLoader, "object");

	const Json::Value& playerStart = objectLayer.getObject("startPoint");
	player.setX(playerStart["x"].asInt());
	player.setY(playerStart["y"].asInt());
}

int
init()
{
	MapLoader mapLoader;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		std::cout << IMG_GetError() << "\n";
		return -1;
	}

	mainWindow = SDL_CreateWindow(
		"test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		600, 600,
		SDL_WINDOW_SHOWN);
	if (!mainWindow)
		return -1;

	mainRenderer = SDL_CreateRenderer(
		mainWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mainRenderer)
		return -1;

	mapLoader.load("map.json");

	backLayer.cleanUp();
	backLayer.load(mainRenderer, mapLoader, "layer_one");

	foreLayer.cleanUp();
	foreLayer.load(mainRenderer, mapLoader, "layer_two");

	objectLayer.cleanUp();
	objectLayer.load(mapLoader, "object");

	const Json::Value& playerStart = objectLayer.getObject("startPoint");
	player.setX(playerStart["x"].asInt());
	player.setY(playerStart["y"].asInt());

	camera.setup(mainRenderer,
		     backLayer.getMapWidth() * backLayer.getTileWidth(),
		     backLayer.getMapHeight() * backLayer.getTileHeight(),
		     //200, 200);
		     150, 150);
	camera.lookAt(player);

	return 0;
}

void
quit()
{
	SDL_DestroyRenderer(mainRenderer);
	mainRenderer = nullptr;

	SDL_DestroyWindow(mainWindow);
	mainWindow = nullptr;

	SDL_Quit();
	IMG_Quit();
}

void
eventHandler(const SDL_Event* event)
{
	controller.eventHandler(event);

	switch (event->type) {
	case SDL_QUIT:
		appIsRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event->key.repeat)
			break;

		switch (event->key.keysym.scancode) {
		case SDL_SCANCODE_R:
			reloadMap();
			break;
		default:
			break;
		};
		break;
	};
}

void
update()
{
	player.update(controller, backLayer);
	camera.update();

	if (controller.ifButtonPressed(BUTTON_START))
		appIsRunning = false;

	controller.stateClear();
}

void
render()
{
	SDL_SetRenderDrawColor(mainRenderer, 0x33, 0x33, 0x33, 0xff);
	SDL_RenderClear(mainRenderer);

	SDL_Rect viewport;

	viewport.x = 0;
	viewport.y = 0;
	viewport.w = backLayer.getTileWidth() * backLayer.getMapWidth();
	viewport.h = backLayer.getTileHeight() * backLayer.getMapHeight();

	SDL_SetRenderDrawColor(mainRenderer, 0x44, 0x33, 0x33, 0xff);
	SDL_RenderFillRect(mainRenderer, &viewport);

	backLayer.render(mainRenderer, camera);
	player.render(mainRenderer, camera);
	foreLayer.render(mainRenderer, camera);

	SDL_RenderPresent(mainRenderer);
}

int
main(int argc, char* argv[])
{
	SDL_Event event;
	Timer timer;

	if (init() < 0)
		return 1;

	try {
		while (appIsRunning) {
			timer.start();

			while (SDL_PollEvent(&event))
				eventHandler(&event);

			update();

			render();

			if (timer.getTicks() < (1000.0 / (double) 60)) {
				SDL_Delay((1000.0 / (double) 60) -
					  timer.getTicks());
			}
		}

	} catch (std::runtime_error e) {
		std::cout << e.what() << "\n";
	} catch (std::exception e) {
		std::cout << e.what() << "\n";
	}

	quit();

	return 0;
}
