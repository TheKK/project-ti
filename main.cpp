#include <iostream>
#include <SDL2/SDL.h>
#include <stdexcept>

#include "timer.h"
#include "map.h"
#include "sprite.h"
#include "controller.h"
#include "camera.h"
#include "player.h"
#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"

namespace
{
	const char kMapFile[] = "map.json";

	SDLSystem sdlSystem;
	Window window;
	Graphics graphics;
	bool appIsRunning = true;

	MapTileLayer backLayer;
	MapTileLayer foreLayer;
	MapObjectLayer objectLayer;

	Camera camera;

	Controller controller;

	Player player;
}

void
loadMap(const std::string& mapFile)
{
	MapLoader mapLoader;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Reload map file: %s", mapFile.c_str());
	mapLoader.load(mapFile);

	backLayer.cleanUp();
	backLayer.load(graphics, mapLoader, "layer_one");

	foreLayer.cleanUp();
	foreLayer.load(graphics, mapLoader, "layer_two");

	objectLayer.cleanUp();
	objectLayer.load(mapLoader, "object");

	const Json::Value& playerStart = objectLayer.getObject("startPoint");
	player.setX(playerStart["x"].asInt());
	player.setY(playerStart["y"].asInt());

	camera.setup(graphics,
		     backLayer.getMapWidth() * backLayer.getTileWidth(),
		     backLayer.getMapHeight() * backLayer.getTileHeight(),
		     //200, 200);
		     150, 150);
	camera.lookAt(player);
}

void
quit()
{

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
			loadMap("./map.json");
			break;
		default:
			break;
		};
		break;
	case SDL_DROPFILE:
		loadMap(event->drop.file);
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
	graphics.clear();

	SDL_Rect viewport;

	viewport.x = 0;
	viewport.y = 0;
	viewport.w = backLayer.getTileWidth() * backLayer.getMapWidth();
	viewport.h = backLayer.getTileHeight() * backLayer.getMapHeight();

	//SDL_SetRenderDrawColor(graphics, 0x44, 0x33, 0x33, 0xff);
	//SDL_RenderFillRect(graphics, &viewport);

	backLayer.render(graphics, camera);
	player.render(graphics, camera);
	foreLayer.render(graphics, camera);

	// present
	graphics.present();
}

#include <memory>
int
main(int argc, char* argv[])
{
	try {
		SDL_Event event;
		Timer timer;

		graphics.init(window);

		loadMap("./map.json");

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
