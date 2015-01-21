#ifndef MAIN_GAME_GAME_SCENE_H
#define MAIN_GAME_GAME_SCENE_H

#include <vector>

#include "map.h"
#include "sprite.h"
#include "controller.h"
#include "camera.h"
#include "player.h"
#include "graphics.h"
#include "window.h"
#include "sdlSystem.h"
#include "entity.h"

#include "gameScene.h"

class Graphics;

class MainGame_GameScene : public GameScene
{
public:
	MainGame_GameScene(Graphics& graphics);
	virtual ~MainGame_GameScene() override;

	virtual void eventHandler(Graphics& graphics, const SDL_Event& event) override;
	virtual void update(Graphics& graphics) override;
	virtual void render(Graphics& graphics) override;
private:
	MapTileLayer backLayer_;
	MapTileLayer foreLayer_;

	std::vector<Entity*> entities_;

	Camera camera_;
	Controller controller_;
	Player player_;

	int currentMap_;

	int bulletTimeCounter_;

	void cleanMap_();
	void loadMap_(Graphics& graphics, const std::string& filePath);
};

#endif /* MAIN_GAME_GAME_SCENE_H */
