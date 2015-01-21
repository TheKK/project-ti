#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H

#include <stack>

class GameScene;
class Graphics;

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
	static void swapScene(Graphics& graphics, enum TotalGameScene toWhich);
private:
	GameSceneManager();
	~GameSceneManager();

	static enum TotalGameScene swapTarget_;
	static std::stack<GameScene*> gameSceneStack_;
};

#endif /* GAME_SCENE_MANAGER_H */
