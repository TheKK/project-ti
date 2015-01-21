#include "mainGame_gameScene.h"

#include "gameSceneManager.h"

enum TotalGameScene GameSceneManager::swapTarget_;
std::stack<GameScene*> GameSceneManager::gameSceneStack_;

GameScene&
GameSceneManager::currentScene(Graphics& graphics)
{
	if (swapTarget_ != NULL_SCENE) {
		gameSceneStack_.pop();
		pushScene(graphics, swapTarget_);
		swapTarget_ = NULL_SCENE;
	}

	return *gameSceneStack_.top();
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
GameSceneManager::swapScene(Graphics& graphics, enum TotalGameScene toWhich)
{
	swapTarget_ = toWhich;
}
