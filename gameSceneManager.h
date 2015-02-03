#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H

#include <stack>
#include <memory>

#include "gameScene.h"

class Graphics;
class Controller;

class Null_GameScene : public GameScene
{
public:
	virtual void eventHandler(Graphics& graphics,
				  const SDL_Event& event) override {};
	virtual void update(Graphics& graphics,
			    const Controller& controller) override {};
	virtual void render(Graphics& graphics) override {};
};

enum TotalGameScene
{
	NULL_SCENE = 0x00,

	MAIN_GAME_SCENE
};

class GameSceneManager
{
public:
	static GameScene* currentScene(Graphics& graphics);
	static void pushScene(Graphics& graphics, enum TotalGameScene which);
	static void popScene();
	static void changeScene(Graphics& graphics,
				enum TotalGameScene toWhich);

	static void shutdown();

	static void quit();
private:
	GameSceneManager();
	~GameSceneManager();

	static bool isRunning_;
	static bool needPop_;
	static enum TotalGameScene sceneToChangeTo_;
	static std::stack<std::unique_ptr<GameScene>> gameSceneStack_;
	static Null_GameScene null_gameScene_;
};

#endif /* GAME_SCENE_MANAGER_H */
