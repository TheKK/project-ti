#include <SDL2/SDL.h>
#include <stdexcept>

#include "timer.h"

#include "game.h"

namespace
{
	const std::vector<std::string> kGameMaps = {
		"./map.json",
		"./newMap.json"
	};

	const int kCameraWidth = 150;
	const int kCameraHeight = 150;
}

Game::Game():
	graphics_(window_),
	player_(graphics_),
	currentMap_(0)
{
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

	cleanMap_();
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
		break;
	case SDL_DROPFILE:
		cleanMap_();
		loadMap_(event.drop.file);
		break;
	};
}

void
Game::update_()
{
	if (controller_.ifButtonPressed(BUTTON_START))
		appIsRunning_ = false;

	if (controller_.ifButtonPressed(BUTTON_SELECT)) {
		cleanMap_();
		loadMap_(kGameMaps[currentMap_]);
	}

	if (controller_.ifButtonPressed(BUTTON_R)) {
		++currentMap_;
		if (currentMap_ == kGameMaps.size())
			currentMap_ = 0;
		cleanMap_();
		loadMap_(kGameMaps[currentMap_]);
	}

	if (controller_.ifButtonPressed(BUTTON_L)) {
		--currentMap_;
		if (currentMap_ < 0)
			currentMap_ = kGameMaps.size() - 1;
		cleanMap_();
		loadMap_(kGameMaps[currentMap_]);
	}

	player_.update(controller_, backLayer_);
	camera_.update();

	for (auto& e : entities_)
		e->update(player_);

	controller_.stateClear();
}

void
Game::render_()
{
	graphics_.clear();

	backLayer_.render(graphics_, camera_);
	player_.render(graphics_, camera_);
	foreLayer_.render(graphics_, camera_);

	for (auto& e : entities_)
		e->render(graphics_, camera_);

	graphics_.present();
}

void
Game::cleanMap_()
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clean map");

	backLayer_.cleanUp();
	foreLayer_.cleanUp();

	for (Entity* e : entities_)
		delete e;
	entities_.clear();
}

void
Game::loadMap_(const std::string& mapFile)
{
	MapLoader mapLoader;
	MapObjectLayer objectLayer;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Reload map file: %s", mapFile.c_str());

	mapLoader.load(mapFile);
	backLayer_.load(graphics_, mapLoader, "layer_one");
	foreLayer_.load(graphics_, mapLoader, "layer_two");
	objectLayer.load(mapLoader, "events");

	for (Json::Value& event : objectLayer) {
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

		if (event["name"].asString() == "startPoint") {
			player_.setX(event["x"].asInt());
			player_.setY(event["y"].asInt());
		}
	}

	camera_.setup(graphics_,
		     backLayer_.getMapWidth() * backLayer_.getTileWidth(),
		     backLayer_.getMapHeight() * backLayer_.getTileHeight(),
		     kCameraWidth, kCameraHeight);
	camera_.lookAt(player_);
}
