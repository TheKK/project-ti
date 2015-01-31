#include "mainGame_gameScene.h"

#include "gameSceneManager.h"

bool GameSceneManager::isRunning_ = true;
enum TotalGameScene GameSceneManager::sceneToChangeTo_;
std::stack<GameScene*> GameSceneManager::gameSceneStack_;
Null_gameScene GameSceneManager::null_gameScene_;

GameScene&
GameSceneManager::currentScene(Graphics& graphics)
{
	if (sceneToChangeTo_ != NULL_SCENE) {
		gameSceneStack_.pop();
		pushScene(graphics, sceneToChangeTo_);
		sceneToChangeTo_ = NULL_SCENE;
	}

	if (gameSceneStack_.size() > 0)
		return *gameSceneStack_.top();
	else
		return null_gameScene_;
}

void
GameSceneManager::pushScene(Graphics& graphics, enum TotalGameScene which)
{

	GameScene* newGameScene = nullptr;

	switch (which) {
	case MAIN_GAME_SCENE:
		newGameScene = new MainGame_GameScene(graphics);
		gameSceneStack_.push(newGameScene);
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
GameSceneManager::shutdown()
{
	isRunning_ = false;
}

bool
GameSceneManager::isRunning()
{
	return isRunning_;
}

void
GameSceneManager::quit()
{
	while (!gameSceneStack_.empty()) {
		delete gameSceneStack_.top();
		gameSceneStack_.pop();
	}
}
