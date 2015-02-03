#include <stdexcept>

#include "transportEvent.h"
#include "deadlyFloor.h"
#include "gameSceneManager.h"
#include "controller.h"

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

void
MainGame_GameScene::eventHandler(Graphics& graphics, const SDL_Event& event)
{
	switch (event.type) {
	case SDL_QUIT:
		GameSceneManager::popScene();
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.scancode == SDL_SCANCODE_W)
			GameSceneManager::pushScene(graphics, MAIN_GAME_SCENE);
		break;
	};
}

void
MainGame_GameScene::update(Graphics& graphics, const Controller& controller)
{
	globalUpdate_(graphics, controller);

	if (bulletTimeCounter_ != 0)
		return;

	player_.update(controller, backLayer_);
	camera_.update();

	for (auto& e : entities_)
		e->update(player_);

	for (auto& e : emitters_)
		e.second->update(player_, controller);

	for (auto& e : recievers_)
		e->update(player_, controller);
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

	for (auto& e : emitters_)
		e.second->render(graphics, camera_);

	for (auto& e : recievers_)
		e->render(graphics, camera_);

	graphics.present();
}

void
MainGame_GameScene::cleanMap_()
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Clean map");

	backLayer_.cleanUp();
	foreLayer_.cleanUp();

	entities_.clear();
	emitters_.clear();
	recievers_.clear();
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
			std::unique_ptr<TransportEvent> transportEvent(
				new TransportEvent());

			eventPosRect.x = event["x"].asInt();
			eventPosRect.y = event["y"].asInt();
			eventPosRect.w = event["width"].asInt();
			eventPosRect.h = event["height"].asInt();

			transportEvent->setEventPosRect(eventPosRect);
			transportEvent->setTransportDestination(
				atoi(event["properties"]["destX"].asCString()),
				atoi(event["properties"]["destY"].asCString()));

			entities_.push_back(std::move(transportEvent));
		}

		if (event["name"].asString() == "deadlyFloor") {
			SDL_Rect floorPosRect;
			std::unique_ptr<DeadlyFloor> deadlyFloor;

			floorPosRect.x = event["x"].asInt();
			floorPosRect.y = event["y"].asInt();
			floorPosRect.w = event["width"].asInt();
			floorPosRect.h = event["height"].asInt();

			deadlyFloor = std::unique_ptr<DeadlyFloor>(
				new DeadlyFloor(graphics, floorPosRect));

			entities_.push_back(std::move(deadlyFloor));
		}
	}

	objectLayer.load(mapLoader, "emitters");
	for (Json::Value& emitter : objectLayer) {
		std::unique_ptr<CollideEvent_signalEmitter> collideEvent;
		const std::string name = emitter["name"].asString();	
		SDL_Rect posRect;

		posRect.x = emitter["x"].asInt();
		posRect.y = emitter["y"].asInt();
		posRect.w = emitter["width"].asInt();
		posRect.h = emitter["height"].asInt();

		collideEvent = std::unique_ptr<CollideEvent_signalEmitter>(
			new CollideEvent_signalEmitter(posRect));

		emitters_[name] = std::move(collideEvent);
	}

	objectLayer.load(mapLoader, "recievers");
	for (Json::Value& reciever : objectLayer) {
		std::unique_ptr<MovingGraound_signalReciever> movingGround;
		SDL_Rect posRect;

		posRect.x = reciever["x"].asInt();
		posRect.y = reciever["y"].asInt();
		posRect.w = reciever["width"].asInt();
		posRect.h = reciever["height"].asInt();

		movingGround = std::unique_ptr<MovingGraound_signalReciever>(
			new MovingGraound_signalReciever(graphics, posRect));

		for (auto& property : reciever["properties"]) {
			if (emitters_.count(property.asString()) == 0) {
				std::string msg =
					"Loading reciever: no emitter named "
					+ property.asString() + " in this map";

				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
					    msg.c_str());

				continue;
			}

			emitters_[property.asString()]->addReceiver(
				*movingGround);
		}

		recievers_.push_back(std::move(movingGround));
	}

	camera_.setup(graphics,
		      backLayer_.getMapWidth() * backLayer_.getTileWidth(),
		      backLayer_.getMapHeight() * backLayer_.getTileHeight(),
		      kCameraWidth, kCameraHeight);
	camera_.lookAt(player_);
}

void
MainGame_GameScene::globalUpdate_(Graphics& graphics,
				  const Controller& controller)
{
	if (controller.getButtonState(BUTTON_L2)) {
		++bulletTimeCounter_;
		if (bulletTimeCounter_ < kBulletTimeMax)
			return;
		else
			bulletTimeCounter_ = 0;
	} else {
		bulletTimeCounter_ = 0;
	}

	if (controller.ifButtonPressed(BUTTON_START))
		GameSceneManager::shutdown();

	if (controller.ifButtonPressed(BUTTON_SELECT)) {
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}

	if (controller.ifButtonPressed(BUTTON_R)) {
		++currentMap_;
		if (currentMap_ == kGameMaps.size())
			currentMap_ = 0;
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}

	if (controller.ifButtonPressed(BUTTON_L)) {
		--currentMap_;
		if (currentMap_ < 0)
			currentMap_ = kGameMaps.size() - 1;
		cleanMap_();
		loadMap_(graphics, kGameMaps[currentMap_]);
	}
}
