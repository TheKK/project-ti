#include "mainGame_gameScene.h"

#include "gameSceneManager.h"

bool GameSceneManager::isRunning_ = true;
bool GameSceneManager::needPop_ = false;
enum TotalGameScene GameSceneManager::sceneToChangeTo_;
std::stack<std::unique_ptr<GameScene>> GameSceneManager::gameSceneStack_;
Null_GameScene GameSceneManager::null_gameScene_;

GameScene*
GameSceneManager::currentScene(Graphics& graphics)
{
	/* TODO We need more elegant */
	if (sceneToChangeTo_ != NULL_SCENE) {
		gameSceneStack_.pop();
		pushScene(graphics, sceneToChangeTo_);
		sceneToChangeTo_ = NULL_SCENE;
	}

	if (needPop_) {
		gameSceneStack_.pop();
		needPop_ = false;
	}

	if (gameSceneStack_.size() == 0)
		shutdown();

	if (!isRunning_)
		return nullptr;

	return gameSceneStack_.top().get();
}

void
GameSceneManager::pushScene(Graphics& graphics, enum TotalGameScene which)
{
	switch (which) {
	case NULL_SCENE:
		gameSceneStack_.push(std::unique_ptr<GameScene>(new Null_GameScene()));
		break;
	case MAIN_GAME_SCENE:
		gameSceneStack_.push(std::unique_ptr<GameScene>(new MainGame_GameScene(graphics)));
		break;
	default:
		break;
	}
}

void
GameSceneManager::changeScene(Graphics& graphics, enum TotalGameScene toWhich)
{
	sceneToChangeTo_ = toWhich;
}

void
GameSceneManager::popScene()
{
	needPop_ = true;
}

void
GameSceneManager::shutdown()
{
	isRunning_ = false;
}
