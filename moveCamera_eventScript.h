#ifndef MOVE_CAMERA_EVENT_SCRIPT_H
#define MOVE_CAMERA_EVENT_SCRIPT_H

#include <SDL2/SDL.h>

#include "eventScript.h"

class Camera;
class MainGame_GameScene;

class MoveCamera_eventScript : public EventScript
{
public:
	MoveCamera_eventScript(MainGame_GameScene& gameScene, Camera& camera,
			       const SDL_Point* dstPoint);

	virtual void update() override;
private:
	MainGame_GameScene& gameScene_;
	Camera& camera_;
	SDL_Point dstPoint_;
};

#endif /* MOVE_CAMERA_EVENT_SCRIPT_H */
