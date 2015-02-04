#ifndef MAIN_GAME_GAME_SCENE_H
#define MAIN_GAME_GAME_SCENE_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <queue>

#include "map.h"
#include "sprite.h"
#include "camera.h"
#include "player.h"
#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"
#include "textLabel.h"
#include "entity.h"
#include "eventScript.h"

#include "collideEvent_signalEmitter.h"
#include "movingGround_signalReciever.h"

#include "gameScene.h"

class Graphics;

class MainGame_GameScene : public GameScene
{
public:
	MainGame_GameScene(Graphics& graphics);

	virtual void eventHandler(Graphics& graphics,
				  const SDL_Event& event) override;
	virtual void update(Graphics& graphics,
			    const Controller& controller) override;
	virtual void render(Graphics& graphics) override;

	void popEventScript();
private:
	enum PlayingState
	{
		RUNNING = 0x00,
		PAUSING
	};
	enum PlayingState playingState_ = RUNNING;

	TextLabel testLabel_;

	MapTileLayer backLayer_;
	MapTileLayer foreLayer_;

	std::vector<std::unique_ptr<Entity>> entities_;
	std::map<std::string, std::unique_ptr<SignalEmitter>> emitters_;
	std::vector<std::unique_ptr<SignalReceiver>> recievers_;

	Camera camera_;
	Player player_;

	int currentMap_;

	int bulletTimeCounter_;

	std::queue<std::unique_ptr<EventScript>> eventScriptQueue_;

	void cleanMap_();
	void loadMap_(Graphics& graphics, const std::string& filePath);
	void globalUpdate_(Graphics& graphics, const Controller& controller);
};

#endif /* MAIN_GAME_GAME_SCENE_H */
