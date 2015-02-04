#include "moveCamera_eventScript.h"

#include "mainGame_gameScene.h"
#include "camera.h"

MoveCamera_eventScript::MoveCamera_eventScript(MainGame_GameScene& gameScene,
					       Camera& camera,
					       const SDL_Point* dstPoint)
	: gameScene_(gameScene),
	  camera_(camera),
	  dstPoint_(*dstPoint)
{
}

void
MoveCamera_eventScript::update()
{
	camera_.setX(dstPoint_.x);
	camera_.setY(dstPoint_.y);

	gameScene_.popEventScript();
}
