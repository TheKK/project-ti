#include <SDL2/SDL.h>
#include <stdexcept>

#include "timer.h"

#include "game.h"

Game::Game()
{
	graphics_.init(window_);
}

Game::~Game()
{
	for (auto& e : entities_)
		delete e;
	entities_.clear();
}

int
Game::execute()
{
	SDL_Event event;
	Timer timer;

	graphics_.init(window_);

	loadMap_("./map.json");

	while (appIsRunning_) {
		timer.start();

		while (SDL_PollEvent(&event))
			eventHandler_(event);

		update_();

		render_();

		if (timer.getTicks() < (1000.0 / (double) 60)) {
			SDL_Delay((1000.0 / (double) 60) -
				  timer.getTicks());
		}
	}

	return 0;
}

void
Game::eventHandler_(const SDL_Event& event)
{
	controller_.eventHandler(event);

	switch (event.type) {
	case SDL_QUIT:
		appIsRunning_ = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.repeat)
			break;

		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_R:
			loadMap_("./map.json");
			break;
		default:
			break;
		};
		break;
	case SDL_DROPFILE:
		loadMap_(event.drop.file);
		break;
	};
}

void
Game::update_()
{
	player_.update(controller_, backLayer_);
	camera_.update();

	for (auto& e : entities_)
		e->update(player_);

	if (controller_.ifButtonPressed(BUTTON_START))
		appIsRunning_ = false;

	if (controller_.ifButtonPressed(BUTTON_SELECT))
		loadMap_("./map.json");

	controller_.stateClear();
}

void
Game::render_()
{
	graphics_.clear();

	//SDL_Rect viewport;

	//viewport.x = 0;
	//viewport.y = 0;
	//viewport.w = backLayer_.getTileWidth() * backLayer_.getMapWidth();
	//viewport.h = backLayer_.getTileHeight() * backLayer_.getMapHeight();

	//SDL_SetRenderDrawColor(graphics, 0x44, 0x33, 0x33, 0xff);
	//SDL_RenderFillRect(graphics, &viewport);

	backLayer_.render(graphics_, camera_);
	player_.render(graphics_, camera_);
	foreLayer_.render(graphics_, camera_);

	for (auto& e : entities_)
		e->render(graphics_, camera_);

	graphics_.present();
}

void
Game::loadMap_(const std::string& mapFile)
{
	MapLoader mapLoader;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Reload map file: %s", mapFile.c_str());
	mapLoader.load(mapFile);

	/* Graphics part */
	backLayer_.cleanUp();
	backLayer_.load(graphics_, mapLoader, "layer_one");

	foreLayer_.cleanUp();
	foreLayer_.load(graphics_, mapLoader, "layer_two");

	/* Player part */
	objectLayer_.cleanUp();
	objectLayer_.load(mapLoader, "object");

	const Json::Value& playerStart = objectLayer_.getObject("startPoint");
	player_.setX(playerStart["x"].asInt());
	player_.setY(playerStart["y"].asInt());

	/* Events part */ /* TODO This should not apear in loadMap_() */
	objectLayer_.cleanUp();
	objectLayer_.load(mapLoader, "events");

	for (Entity* e : entities_)
		delete e;
	entities_.clear();

	for (Json::Value& event : objectLayer_) {
		if (event["name"].asString() == "transportEvent") {
			SDL_Rect eventPosRect;
			TransportEvent* te = new TransportEvent();

			eventPosRect.x = event["x"].asInt();
			eventPosRect.y = event["y"].asInt();
			eventPosRect.w = event["width"].asInt();
			eventPosRect.h = event["height"].asInt();

			te->setEventPosRect(eventPosRect);
			te->setTransportDestination(
				atoi(event["properties"]["destX"].asCString()),
				atoi(event["properties"]["destY"].asCString()));

			entities_.push_back(te);
		}
	}

	camera_.setup(graphics_,
		     backLayer_.getMapWidth() * backLayer_.getTileWidth(),
		     backLayer_.getMapHeight() * backLayer_.getTileHeight(),
		     //200, 200);
		     150, 150);
	camera_.lookAt(player_);
}
