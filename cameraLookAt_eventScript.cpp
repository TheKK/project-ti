#include "cameraLookAt_eventScript.h"

#include "mainGame_gameScene.h"

CameraLookAt_eventScript::CameraLookAt_eventScript(
	MainGame_GameScene& gameScene, Camera& camera, const Player* player)
	: gameScene_(gameScene),
	  camera_(camera),
	  player_(player)
{
}

void
CameraLookAt_eventScript::update()
{
	camera_.lookAt(player_);
	gameScene_.popEventScript();
}
