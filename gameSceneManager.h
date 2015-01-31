#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H

#include <stack>

#include "gameScene.h"

class Graphics;

class Null_gameScene : public GameScene
{
public:
	virtual ~Null_gameScene() override {};

	virtual void eventHandler(Graphics& graphics,
				  const SDL_Event& event) override {};
	virtual void update(Graphics& graphics) override {};
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
	static GameScene& currentScene(Graphics& graphics);
	static void pushScene(Graphics& graphics, enum TotalGameScene which);
	static void changeScene(Graphics& graphics,
				enum TotalGameScene toWhich);

	static void shutdown();
	static bool isRunning();

	static void quit();
private:
	GameSceneManager();
	~GameSceneManager();

	static bool isRunning_;
	static enum TotalGameScene sceneToChangeTo_;
	static std::stack<GameScene*> gameSceneStack_;
	static Null_gameScene null_gameScene_;
};

#endif /* GAME_SCENE_MANAGER_H */
