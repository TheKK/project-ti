#include <stdexcept>

#include "transportEvent.h"
#include "deadlyFloor.h"
#include "gameSceneManager.h"
#include "controller.h"
#include "moveCamera_eventScript.h"
#include "wait_eventScript.h"
#include "cameraLookAt_eventScript.h"

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
	testLabel_(graphics, "Hello World!",
		   "assets/fonts/djb_almost_perfect/DJB Almost Perfect.ttf",
		   60, SDL_Color({0x00, 0x00, 0x00})),
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
		GameSceneManager::shutdown();
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.scancode == SDL_SCANCODE_W)
			GameSceneManager::pushScene(graphics, MAIN_GAME_SCENE);

		if (event.key.keysym.scancode == SDL_SCANCODE_U) {
			SDL_Point p;

			for (int i = 0; i < 5; ++i) {
				p = {rand() % 600, rand() % 600};
				eventScriptQueue_.push(std::unique_ptr<EventScript>(new MoveCamera_eventScript(*this, camera_, &p)));
				eventScriptQueue_.push(std::unique_ptr<EventScript>(new Wait_eventScript(*this, 60)));
			}

			eventScriptQueue_.push(std::unique_ptr<EventScript>(new CameraLookAt_eventScript(*this, camera_, &player_)));
		}

		break;
	};
}

void
MainGame_GameScene::update(Graphics& graphics, const Controller& controller)
{
	if (!eventScriptQueue_.empty()) {
		EventScript& eventScript = *(eventScriptQueue_.front());

		eventScript.update();
	} else {
		globalUpdate_(graphics, controller);

		if (bulletTimeCounter_ != 0)
			return;

		player_.update(controller, backLayer_);
		camera_.update();

		for (auto& e : entities_)
			e->update();

		for (auto& e : emitters_)
			e.second->update(player_, controller);

		for (auto& e : recievers_)
			e->update(player_, controller);
	}
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

	testLabel_.render(graphics, nullptr);

	graphics.present();
}

void
MainGame_GameScene::popEventScript()
{
	eventScriptQueue_.pop();
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
		    "Load map file: %s", mapFile.c_str());

	mapLoader.load(mapFile);
	backLayer_.load(graphics, mapLoader, "layer_one");
	foreLayer_.load(graphics, mapLoader, "layer_two");

	objectLayer.load(mapLoader, "events");
	for (Json::Value& event : objectLayer) {
		const std::string& eventName = event["name"].asString();

		if (eventName == "startPoint") {
			player_.setX(event["x"].asInt());
			player_.setY(event["y"].asInt());
			player_.setCheckpoint(event["x"].asInt(),
					      event["y"].asInt());

		} else if (eventName == "transportEvent") {
			SDL_Rect eventPosRect;
			SDL_Point dstPoint;
			const Json::Value& properties = event["properties"];
			std::unique_ptr<TransportEvent> transportEvent;

			eventPosRect.x = event["x"].asInt();
			eventPosRect.y = event["y"].asInt();
			eventPosRect.w = event["width"].asInt();
			eventPosRect.h = event["height"].asInt();

			dstPoint.x = atoi(properties["destX"].asCString());
			dstPoint.y = atoi(properties["destY"].asCString());

			transportEvent.reset(new TransportEvent(eventPosRect,
								player_,
								dstPoint));

			entities_.push_back(std::move(transportEvent));

		} else if (eventName == "deadlyFloor") {
			SDL_Rect eventPosRect;
			std::unique_ptr<DeadlyFloor> deadlyFloor;

			eventPosRect.x = event["x"].asInt();
			eventPosRect.y = event["y"].asInt();
			eventPosRect.w = event["width"].asInt();
			eventPosRect.h = event["height"].asInt();

			deadlyFloor.reset(new DeadlyFloor(graphics,
							  eventPosRect,
							  player_));

			entities_.push_back(std::move(deadlyFloor));

		} else {
			std::string warnMsg;

			warnMsg = "[loadMap] event " + eventName;
			warnMsg += " didn't have implemtation";
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
				    warnMsg.c_str());
		}
	}

	/* TODO fix naming problem */
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

	/* TODO fix naming problem */
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
	camera_.lookAt(&player_);
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
