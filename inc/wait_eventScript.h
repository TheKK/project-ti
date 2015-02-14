#ifndef WAIT_EVENT_SCRIPT_H
#define WAIT_EVENT_SCRIPT_H

#include <SDL2/SDL.h>

#include "eventScript.h"

class MainGame_GameScene;

class Wait_eventScript : public EventScript
{
public:
	Wait_eventScript(MainGame_GameScene& gameScene, int frameToWait);

	virtual void update() override;
private:
	MainGame_GameScene& gameScene_;

	int currentWait_;
	int frameToWait_;
};

#endif /* WAIT_EVENT_SCRIPT_H */
