#include <stdexcept>

#include "transportEvent.h"
#include "deadlyFloor.h"
#include "gameSceneManager.h"

#include "mainGame_gameScene.h"

namespace
{
	const std::vector<std::string> kGameMaps = {
		"map.json",
		"newMap.json"
	};

	const int kCameraWidth = 150;
	const int kCameraHeight = 150;

	const int kBulletTimeMax = 2;
}

MainGame_GameScene::MainGame_GameScene(Graphics& graphics):
	player_(graphics),
	currentMap_(0)
{
	cleanMap_();
	loadMap_(graphics, kGameMaps[currentMap_]);
}

MainGame_GameScene::~MainGame_GameScene()
{
	for (auto& e : entities_)
		delete e;
	entities_.clear();
}

void
MainGame_GameScene::eventHandler(Graphics& graphics, const SDL_Event& event)
{
	controller_.eventHandler(event);

	switch (event.type) {
	case SDL_QUIT:
		GameSceneManager::shutdown();
		break;
	case SDL_KEYDOWN:
		if (event.key.repeat)
			break;
		break;
	case SDL_DROPFILE:
		cleanMap_();
		loadMap_(graphics, event.drop.file);
		break;
	};
}

void
MainGame_GameScene::update(Graphics& graphics)
{
	if (controller_.getButtonState(BUTTON_L2)) {
		++bulletTimeCounter_;
		if (bulletTimeCounter_ < kBulletTimeMax)
			return;
		else
			bulletTimeCounter_ = 0;
	}

	if (controller_.ifButtonPressed(BUTTON_START))
		GameSceneManager::shutdown();

	if (controller_.ifButtonPressed(BUTTON_SELECT)) {
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}

	if (controller_.ifButtonPressed(BUTTON_R)) {
		++currentMap_;
		if (currentMap_ == kGameMaps.size())
			currentMap_ = 0;
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}

	if (controller_.ifButtonPressed(BUTTON_L)) {
		--currentMap_;
		if (currentMap_ < 0)
			currentMap_ = kGameMaps.size() - 1;
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}

	player_.update(controller_, backLayer_);
	camera_.update();

	for (auto& e : entities_)
		e->update(player_);

	controller_.stateClear();
}

void
MainGame_GameScene::render(Graphics& graphics)
{
	graphics.clear();

	backLayer_.render(graphics, camera_);
	player_.render(graphics, camera_);
	foreLayer_.render(graphics, camera_);

	for (auto& e : entities_)
		e->render(graphics, camera_);

	graphics.present();
}

void
MainGame_GameScene::cleanMap_()
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clean map");

	backLayer_.cleanUp();
	foreLayer_.cleanUp();

	for (Entity* e : entities_)
		delete e;
	entities_.clear();
}

void
MainGame_GameScene::loadMap_(Graphics& graphics, const std::string& mapFile)
{
	MapLoader mapLoader;
	MapObjectLayer objectLayer;

	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
		    "Reload map file: %s", mapFile.c_str());

	mapLoader.load(mapFile);
	backLayer_.load(graphics, mapLoader, "layer_one");
	foreLayer_.load(graphics, mapLoader, "layer_two");
	objectLayer.load(mapLoader, "events");

	for (Json::Value& event : objectLayer) {
		if (event["name"].asString() == "startPoint") {
			player_.setX(event["x"].asInt());
			player_.setY(event["y"].asInt());
			player_.setCheckpoint(event["x"].asInt(),
					      event["y"].asInt());
		}

		if (event["name"].asString() == "transportEvent") {
			SDL_Rect eventPosRect;
			TransportEvent* transportEvent = new TransportEvent();

			eventPosRect.x = event["x"].asInt();
			eventPosRect.y = event["y"].asInt();
			eventPosRect.w = event["width"].asInt();
			eventPosRect.h = event["height"].asInt();

			transportEvent->setEventPosRect(eventPosRect);
			transportEvent->setTransportDestination(
				atoi(event["properties"]["destX"].asCString()),
				atoi(event["properties"]["destY"].asCString()));

			entities_.push_back(transportEvent);
		}

		if (event["name"].asString() == "deadlyFloor") {
			SDL_Rect floorPosRect;
			DeadlyFloor* deadlyFloor = nullptr;

			floorPosRect.x = event["x"].asInt();
			floorPosRect.y = event["y"].asInt();
			floorPosRect.w = event["width"].asInt();
			floorPosRect.h = event["height"].asInt();

			deadlyFloor = new DeadlyFloor(graphics, floorPosRect);

			entities_.push_back(deadlyFloor);
		}
	}

	camera_.setup(graphics,
		     backLayer_.getMapWidth() * backLayer_.getTileWidth(),
		     backLayer_.getMapHeight() * backLayer_.getTileHeight(),
		     kCameraWidth, kCameraHeight);
	camera_.lookAt(player_);
}
