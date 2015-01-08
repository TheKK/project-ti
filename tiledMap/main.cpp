#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <jsoncpp/json/json.h>
#include <stdexcept>

#include "map.h"
#include "sprite.h"
#include "controller.h"
#include "camera.h"

static SDL_Window* mainWindow = nullptr;
static SDL_Renderer* mainRenderer = nullptr;
static bool appIsRunning = true;

static MapTileLayer backLayer;
static MapTileLayer foreLayer;;
static Camera camera;

static Controller controller;

void
reloadMap()
{
	MapLoader mapLoader;

	mapLoader.load("map.json");

	backLayer.cleanUp();
	backLayer.load(mainRenderer,
			  mapLoader.getTileLayer("layer_one"),
			  mapLoader.getTileSets());

	foreLayer.cleanUp();
	foreLayer.load(mainRenderer,
		       mapLoader.getTileLayer("layer_two"),
		       mapLoader.getTileSets());
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
	backLayer.load(mainRenderer,
		       mapLoader.getTileLayer("layer_one"),
		       mapLoader.getTileSets());

	foreLayer.cleanUp();
	foreLayer.load(mainRenderer,
		       mapLoader.getTileLayer("layer_two"),
		       mapLoader.getTileSets());

	camera.setup(mainRenderer,
		     backLayer.getMapWidth() * backLayer.getTileWidth(),
		     backLayer.getMapHeight() * backLayer.getTileHeight(),
		     200, 200);
		     //150, 150);

	//camera.lookAt(&mouse);

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
	{
		backLayer.render(mainRenderer, camera);
		foreLayer.render(mainRenderer, camera);
	}
	SDL_RenderPresent(mainRenderer);
}

int
main(int argc, char* argv[])
{
	SDL_Event event;

	if (init() < 0)
		return 1;

	try {
		while (appIsRunning) {
			while (SDL_PollEvent(&event))
				eventHandler(&event);

			update();

			render();

			SDL_Delay(30);
		}

	} catch (std::runtime_error e) {
		std::cout << e.what() << "\n";
	} catch (std::exception e) {
		std::cout << e.what() << "\n";
	}

	quit();

	return 0;
}
