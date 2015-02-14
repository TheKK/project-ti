#include "wait_eventScript.h"

#include "mainGame_gameScene.h"

Wait_eventScript::Wait_eventScript(MainGame_GameScene& gameScene,
				   int frameToWait)
	: gameScene_(gameScene),
	  currentWait_(0),
	  frameToWait_(frameToWait)
{
}

void
Wait_eventScript::update()
{
	++currentWait_;
	if (currentWait_ == frameToWait_)
		gameScene_.popEventScript();
}
